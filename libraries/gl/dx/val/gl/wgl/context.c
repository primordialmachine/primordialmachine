#include "dx/val/gl/wgl/context.h"

#include "dx/val/gl/wgl/window.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.context",
                      dx_gl_wgl_context,
                      dx_val_gl_context);

static Core_Result link(void** RETURN, char const* name);

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

static Core_Result enter_frame(dx_gl_wgl_context* SELF) {
  Core_Type* _parent_type = NULL;
  Core_Type_getParent(&_parent_type, CORE_OBJECT(SELF)->type);
  dx_gl_wgl_context_dispatch* dispatch = (dx_gl_wgl_context_dispatch*)dx_rti_type_get_dispatch(_parent_type);
  if (DX_VAL_CONTEXT_DISPATCH(dispatch)->enter_frame(DX_VAL_CONTEXT(SELF))) {
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
  return dx_val_gl_window_get_canvas_size(DX_VAL_GL_WINDOW(SELF->window), width, height);
}

static Core_Result get_vsync_enabled(Core_Boolean* RETURN, dx_gl_wgl_context* SELF) {
  *RETURN = (1 == SELF->wglGetSwapIntervalEXT());
  return Core_Success;
}

static Core_Result set_vsync_enabled(dx_gl_wgl_context* SELF, Core_Boolean is_vsync_enabled) {
  SELF->wglSwapIntervalEXT(is_vsync_enabled ? 1 : 0);
  return Core_Success;
}

static Core_Result get_canvas_dpi(dx_gl_wgl_context* SELF, Core_Integer32* x, Core_Integer32* y) {
  UINT z = GetDpiForWindow(SELF->window->wnd);
  *x = z;
  *y = z;
  return Core_Success;
}

Core_Result dx_gl_wgl_context_construct(dx_gl_wgl_context* SELF, dx_val_gl_wgl_window* window) {
  DX_CONSTRUCT_PREFIX(dx_gl_wgl_context);
  if (dx_val_gl_context_construct(DX_VAL_GL_CONTEXT(SELF), &link)) {
    return Core_Failure;
  }
  if (!window) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->window = window;
  DX_REFERENCE(window);
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE

#define DEFINE(TYPE, NAME, EXTENSION_NAME) \
  if (link((void**)&(SELF)->NAME, #NAME)) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n")); \
    DX_UNREFERENCE(SELF->window); \
    SELF->window = NULL; \
    return Core_Failure; \
  }
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_gl_wgl_context_destruct(dx_gl_wgl_context* SELF) {
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE
  DX_UNREFERENCE(SELF->window);
  SELF->window = NULL;
}

static void dx_gl_wgl_context_constructDispatch(dx_gl_wgl_context_dispatch* SELF) {
  DX_VAL_CONTEXT_DISPATCH(SELF)->enter_frame = (Core_Result (*)(dx_val_context*)) & enter_frame;
  DX_VAL_CONTEXT_DISPATCH(SELF)->leave_frame = (Core_Result(*)(dx_val_context*)) & leave_frame;
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_canvas_size = (Core_Result(*)(dx_val_context*, Core_Integer32*, Core_Integer32*)) & get_canvas_size;
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_vsync_enabled = (Core_Result(*)(Core_Boolean*, dx_val_context*)) & get_vsync_enabled;
  DX_VAL_CONTEXT_DISPATCH(SELF)->set_vsync_enabled = (Core_Result(*)(dx_val_context*, Core_Boolean)) & set_vsync_enabled;
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_canvas_dpi = (Core_Result(*)(dx_val_context*, Core_Integer32*, Core_Integer32*)) & get_canvas_dpi;
}

Core_Result dx_gl_wgl_context_create(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_window* window) {
  DX_CREATE_PREFIX(dx_gl_wgl_context);
  if (dx_gl_wgl_context_construct(SELF, window)) {
    DX_UNREFERENCE(SELF);
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
