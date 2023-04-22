#include "dx/gl/wgl/window.h"

#include "dx/gl/wgl/application.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.window",
                      dx_gl_wgl_window,
                      dx_object);

static void dx_gl_wgl_window_destruct(dx_gl_wgl_window* SELF) {
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

static void dx_gl_wgl_window_dispatch_construct(dx_gl_wgl_window_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_gl_wgl_window_construct(dx_gl_wgl_window* self, dx_gl_wgl_application* application) {
  dx_rti_type* TYPE = dx_gl_wgl_window_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!application) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  self->application = application;
  DX_REFERENCE(application);
  //WS_CLIPSIBLINGS |           // Required Window Style
  //WS_CLIPCHILDREN |           // Required Window Style
  self->wnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                             application->class_name,
                             "Media Evaluation Framework",
                             WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             NULL,
                             NULL,
                             application->instance_handle,
                             NULL);
  if (!self->wnd) {
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create window\n", sizeof("unable to create window\n"));
    return DX_FAILURE;
  }
  self->dc = GetDC(self->wnd);
  if (!self->dc) {
    DestroyWindow(self->wnd);
    self->wnd = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n"));
    return DX_FAILURE;
  }
  DX_OBJECT(self)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_gl_wgl_window_create(dx_gl_wgl_window** RETURN, dx_gl_wgl_application* application) {
  dx_gl_wgl_window* SELF = DX_GL_WGL_WINDOW(dx_object_alloc(sizeof(dx_gl_wgl_window)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_gl_wgl_window_construct(SELF, application)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_gl_wgl_window_get_canvas_size(dx_gl_wgl_window* SELF, dx_i32* width, dx_i32* height) {
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
