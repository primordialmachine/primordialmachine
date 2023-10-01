#include "dx/val/gl/wgl/context.h"

#include "dx/val/gl/wgl/window.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.context",
                      dx_gl_wgl_context,
                      dx_val_gl_context)

static void* link(char const* name);

static dx_result enter_frame(dx_gl_wgl_context* SELF);

static dx_result leave_frame(dx_gl_wgl_context* SELF);

static dx_result get_canvas_size(dx_gl_wgl_context* SELF, dx_i32* width, dx_i32* height);

static void* link(char const* name) {
  void* p = (void*)wglGetProcAddress(name);
  if (!p) {
    // This technique is required for OpenGL 1.0 core functions.
    HMODULE module = LoadLibraryA("opengl32.dll");
    if (!module) {
      return NULL;
    }
    p = (void*)GetProcAddress(module, name);
  }
  return p;
}

static dx_result enter_frame(dx_gl_wgl_context* SELF) {
  dx_rti_type* _parent_type = dx_rti_type_get_parent(DX_OBJECT(SELF)->type);
  dx_gl_wgl_context_dispatch* dispatch = (dx_gl_wgl_context_dispatch*)dx_rti_type_get_dispatch(_parent_type);
  if (DX_VAL_CONTEXT_DISPATCH(dispatch)->enter_frame(DX_VAL_CONTEXT(SELF))) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result leave_frame(dx_gl_wgl_context* SELF) {
  DX_VAL_GL_CONTEXT(SELF)->glFlush();
  SwapBuffers(SELF->window->dc);
  return DX_SUCCESS;
}

static dx_result get_canvas_size(dx_gl_wgl_context* SELF, dx_i32* width, dx_i32* height) {
  return dx_val_gl_window_get_canvas_size(DX_VAL_GL_WINDOW(SELF->window), width, height);
}

static dx_result get_vsync_enabled(dx_bool* RETURN, dx_gl_wgl_context* SELF) {
  *RETURN = (1 == SELF->wglGetSwapIntervalEXT());
  return DX_SUCCESS;
}

static dx_result set_vsync_enabled(dx_gl_wgl_context* SELF, dx_bool is_vsync_enabled) {
  SELF->wglSwapIntervalEXT(is_vsync_enabled ? 1 : 0);
  return DX_SUCCESS;
}

static dx_result get_dpi(dx_gl_wgl_context* SELF, dx_i32* x, dx_i32* y) {
  UINT z = GetDpiForWindow(SELF->window->wnd);
  *x = z;
  *y = z;
  return DX_SUCCESS;
}

dx_result dx_gl_wgl_context_construct(dx_gl_wgl_context* SELF, dx_val_gl_wgl_window* window) {
  dx_rti_type* TYPE = dx_gl_wgl_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_gl_context_construct(DX_VAL_GL_CONTEXT(SELF), &link)) {
    return DX_FAILURE;
  }
  if (!window) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->window = window;
  DX_REFERENCE(window);
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE

#define DEFINE(TYPE, NAME, EXTENSION_NAME) \
  (SELF)->NAME = (TYPE)link(#NAME); \
  if (!(SELF)->NAME) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n")); \
    return DX_FAILURE; \
  }
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_gl_wgl_context_destruct(dx_gl_wgl_context* SELF) {
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE
  DX_UNREFERENCE(SELF->window);
  SELF->window = NULL;
}

static void dx_gl_wgl_context_dispatch_construct(dx_gl_wgl_context_dispatch* SELF) {
  DX_VAL_CONTEXT_DISPATCH(SELF)->enter_frame = (dx_result (*)(dx_val_context*)) & enter_frame;
  DX_VAL_CONTEXT_DISPATCH(SELF)->leave_frame = (dx_result(*)(dx_val_context*)) & leave_frame;
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_canvas_size = (dx_result(*)(dx_val_context*, dx_i32*, dx_i32*)) & get_canvas_size;
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_vsync_enabled = (dx_result(*)(dx_bool*, dx_val_context*)) & get_vsync_enabled;
  DX_VAL_CONTEXT_DISPATCH(SELF)->set_vsync_enabled = (dx_result(*)(dx_val_context*, dx_bool)) & set_vsync_enabled;
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_dpi = (dx_result(*)(dx_val_context*, dx_i32*, dx_i32*)) & get_dpi;
}

dx_result dx_gl_wgl_context_create(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_window* window) {
  DX_CREATE_PREFIX(dx_gl_wgl_context)
  if (dx_gl_wgl_context_construct(SELF, window)) {
    DX_UNREFERENCE(SELF);
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
