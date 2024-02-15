#include "dx/val/gl/wgl/context.h"

#include "dx/val/gl/wgl/window.h"

Core_defineObjectType("dx.gl.wgl.context",
                      dx_gl_wgl_context,
                      dx_val_gl_context);

static Core_Result link(void** RETURN, char const* name);

static Core_Result isExtensionSupported(Core_Boolean* RETURN, dx_gl_wgl_context* SELF, char const* extension);

static Core_Result enter_frame(dx_gl_wgl_context* SELF);

static Core_Result leave_frame(dx_gl_wgl_context* SELF);

static Core_Result get_canvas_size(dx_gl_wgl_context* SELF, Core_Integer32* width, Core_Integer32* height);

static Core_Result get_vsync_enabled(Core_Boolean* RETURN, dx_gl_wgl_context* SELF);

static Core_Result set_vsync_enabled(dx_gl_wgl_context* SELF, Core_Boolean is_vsync_enabled);

static Core_Result get_canvas_dpi(dx_gl_wgl_context* SELF, Core_Integer32* x, Core_Integer32* y);

static Core_Result link(void** RETURN, char const* name) {
  void* p = (void*)wglGetProcAddress(name);
  if (!p) {
    // This technique is required for OpenGL 1.0 core functions.
    HMODULE module = LoadLibraryA("opengl32.dll");
    if (!module) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    p = (void*)GetProcAddress(module, name);
  }
  if (!p) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = p;
  return Core_Success;
}

static Core_Result isExtensionSupported(Core_Boolean* RETURN, dx_gl_wgl_context* SELF, char const* extension) {
  GLint n;
  DX_VAL_GL_CONTEXT(SELF)->glGetIntegerv(GL_NUM_EXTENSIONS, &n);
  for (GLint i = 0; i < n; ++i) {
    GLubyte const* p = DX_VAL_GL_CONTEXT(SELF)->glGetStringi(GL_EXTENSIONS, i);
    if (!p) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    if (!strcmp(extension, p)) {
      *RETURN = Core_True;
      return Core_Success;
    }
  }
  *RETURN = Core_False;
  return Core_Success;
}

static Core_Result enter_frame(dx_gl_wgl_context* SELF) {
  Core_Type* parentType = NULL;
  Core_Type_getParent(&parentType, CORE_OBJECT(SELF)->type);
  dx_gl_wgl_context_Dispatch* dispatch = NULL;
  if (Core_Type_getDispatch(&dispatch, parentType)) {
    return Core_Failure;
  }
  if (CORE_VISUALS_CONTEXT_DISPATCH(dispatch)->enterFrame(CORE_VISUALS_CONTEXT(SELF))) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result leave_frame(dx_gl_wgl_context* SELF) {
  DX_VAL_GL_CONTEXT(SELF)->glFlush();
  SwapBuffers(SELF->window->dc);
  return Core_Success;
}

static Core_Result get_canvas_size(dx_gl_wgl_context* SELF, Core_Integer32* width, Core_Integer32* height) {
  return dx_val_gl_window_getCanvasSize(DX_VAL_GL_WINDOW(SELF->window), width, height);
}

static Core_Result get_vsync_enabled(Core_Boolean* RETURN, dx_gl_wgl_context* SELF) {
  *RETURN = (1 == SELF->functions.wglGetSwapIntervalEXT());
  return Core_Success;
}

static Core_Result set_vsync_enabled(dx_gl_wgl_context* SELF, Core_Boolean is_vsync_enabled) {
  SELF->functions.wglSwapIntervalEXT(is_vsync_enabled ? 1 : 0);
  return Core_Success;
}

static Core_Result get_canvas_dpi(dx_gl_wgl_context* SELF, Core_Integer32* x, Core_Integer32* y) {
  UINT z = GetDpiForWindow(SELF->window->wnd);
  *x = z;
  *y = z;
  return Core_Success;
}

Core_Result dx_gl_wgl_context_construct(dx_gl_wgl_context* SELF, Core_Val_Gl_Wgl_Window* window) {
  DX_CONSTRUCT_PREFIX(dx_gl_wgl_context);
  if (dx_val_gl_context_construct(DX_VAL_GL_CONTEXT(SELF), &link)) {
    return Core_Failure;
  }
  if (!window) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->window = window;
  CORE_REFERENCE(window);
 
#define Define(NAME, STRING) (SELF)->extensions.with_NAME = Core_False;
#include "dx/val/gl/wgl/extensions.i"
#undef Define

#define Define(TYPE, NAME, EXTENSION_NAME) (SELF)->functions.NAME = NULL;
#include "dx/val/gl/wgl/functions.i"
#undef Define

#define Define(NAME, STRING) \
  if (isExtensionSupported(&SELF->extensions.with_NAME, SELF, STRING)) { \
    return Core_Failure; \
  }
#include "dx/val/gl/wgl/extensions.i"
#undef Define

#define Define(TYPE, NAME, EXTENSION_NAME) \
  if (link((void**)&(SELF)->functions.NAME, #NAME)) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n")); \
    CORE_UNREFERENCE(SELF->window); \
    SELF->window = NULL; \
    return Core_Failure; \
  }
#include "dx/val/gl/wgl/functions.i"
#undef Define
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_gl_wgl_context_destruct(dx_gl_wgl_context* SELF) {
#define Define(TYPE, NAME, EXTENSION_NAME) (SELF)->functions.NAME = NULL;
#include "dx/val/gl/wgl/functions.i"
#undef Define
  CORE_UNREFERENCE(SELF->window);
  SELF->window = NULL;
}

static void dx_gl_wgl_context_constructDispatch(dx_gl_wgl_context_Dispatch* SELF) {
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->enterFrame = (Core_Result (*)(Core_Visuals_Context*)) & enter_frame;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->leaveFrame = (Core_Result(*)(Core_Visuals_Context*)) & leave_frame;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->getCanvasSize = (Core_Result(*)(Core_Visuals_Context*, Core_Integer32*, Core_Integer32*)) & get_canvas_size;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->getVsyncEnabled = (Core_Result(*)(Core_Boolean*, Core_Visuals_Context*)) & get_vsync_enabled;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->setVsyncEnabled = (Core_Result(*)(Core_Visuals_Context*, Core_Boolean)) & set_vsync_enabled;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->getCanvasDpi = (Core_Result(*)(Core_Visuals_Context*, Core_Integer32*, Core_Integer32*)) & get_canvas_dpi;
}

Core_Result dx_gl_wgl_context_create(dx_gl_wgl_context** RETURN, Core_Val_Gl_Wgl_Window* window) {
  DX_CREATE_PREFIX(dx_gl_wgl_context);
  if (dx_gl_wgl_context_construct(SELF, window)) {
    CORE_UNREFERENCE(SELF);
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
