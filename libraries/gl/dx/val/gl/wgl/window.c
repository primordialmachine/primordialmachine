#include "dx/val/gl/wgl/window.h"

#include "dx/application.h"
#include "dx/val/gl/wgl/system.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.wgl.window",
                      dx_val_gl_wgl_window,
                      Core_Object);

static Core_Result get_canvas_size(dx_val_gl_wgl_window* SELF, Core_Integer32* width, Core_Integer32* height);

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

static void dx_val_gl_wgl_window_constructDispatch(dx_val_gl_wgl_window_dispatch* SELF) {
  DX_VAL_GL_WINDOW_DISPATCH(SELF)->get_canvas_size = (Core_Result(*)(dx_val_gl_window*,Core_Integer32*,Core_Integer32*)) & get_canvas_size;
}

Core_Result dx_val_gl_wgl_window_construct(dx_val_gl_wgl_window* SELF, dx_gl_wgl_application* application) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_wgl_window);
  if (!application) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (dx_val_gl_window_construct(DX_VAL_GL_WINDOW(SELF))) {
    return Core_Failure;
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
    return Core_Failure;
  }
  SELF->dc = GetDC(SELF->wnd);
  if (!SELF->dc) {
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n") - 1);
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_gl_wgl_window_create(dx_val_gl_wgl_window** RETURN, dx_gl_wgl_application* application) {
  DX_CREATE_PREFIX(dx_val_gl_wgl_window);
  if (dx_val_gl_wgl_window_construct(SELF, application)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result get_canvas_size(dx_val_gl_wgl_window* SELF, Core_Integer32* width, Core_Integer32* height) {
  if (!SELF || !width || !height) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  RECT rect;
  GetClientRect(SELF->wnd, &rect);
  *width = rect.right - rect.left;
  *height = rect.bottom - rect.top;
  return Core_Success;
}
