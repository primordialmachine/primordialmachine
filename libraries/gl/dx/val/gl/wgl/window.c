#include "dx/val/gl/wgl/window.h"

#include "dx/application.h"
#include "dx/val/gl/wgl/system.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.wgl.window",
                      dx_val_gl_wgl_window,
                      dx_object);

static dx_result get_canvas_size(dx_val_gl_wgl_window* SELF, dx_i32* width, dx_i32* height);

static void dx_val_gl_wgl_window_destruct(dx_val_gl_wgl_window* SELF) {
  if (SELF->dc) {
    ReleaseDC(SELF->wnd, SELF->dc);
    SELF->dc = NULL;
  }
  if (SELF->wnd) {
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
  }
  DX_DEBUG_ASSERT(NULL != SELF->application);
  DX_UNREFERENCE(SELF->application);
  SELF->application = NULL;
}

static void dx_val_gl_wgl_window_dispatch_construct(dx_val_gl_wgl_window_dispatch* SELF) {
  DX_VAL_GL_WINDOW_DISPATCH(SELF)->get_canvas_size = (dx_result(*)(dx_val_gl_window*,dx_i32*,dx_i32*)) & get_canvas_size;
}

dx_result dx_val_gl_wgl_window_construct(dx_val_gl_wgl_window* SELF, dx_gl_wgl_application* application) {
  dx_rti_type* TYPE = dx_val_gl_wgl_window_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!application) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (dx_val_gl_window_construct(DX_VAL_GL_WINDOW(SELF))) {
    return DX_FAILURE;
  }
  SELF->application = application;
  DX_REFERENCE(application);
  SELF->wnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                             DX_VAL_GL_WGL_SYSTEM(DX_APPLICATION(application)->val_system)->class_name,
                             "Media Evaluation Framework",
                             WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             NULL,
                             NULL,
                             DX_VAL_GL_WGL_SYSTEM(DX_APPLICATION(application)->val_system)->instance_handle,
                             NULL);
  if (!SELF->wnd) {
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create window\n", sizeof("unable to create window\n") - 1);
    return DX_FAILURE;
  }
  SELF->dc = GetDC(SELF->wnd);
  if (!SELF->dc) {
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n") - 1);
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_gl_wgl_window_create(dx_val_gl_wgl_window** RETURN, dx_gl_wgl_application* application) {
  dx_val_gl_wgl_window* SELF = DX_VAL_GL_WGL_WINDOW(dx_object_alloc(sizeof(dx_val_gl_wgl_window)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_val_gl_wgl_window_construct(SELF, application)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result get_canvas_size(dx_val_gl_wgl_window* SELF, dx_i32* width, dx_i32* height) {
  if (!SELF || !width || !height) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  RECT rect;
  GetClientRect(SELF->wnd, &rect);
  *width = rect.right - rect.left;
  *height = rect.bottom - rect.top;
  return DX_SUCCESS;
}
