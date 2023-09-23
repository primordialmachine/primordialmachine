#include "dx/val/gl/window.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.window",
                      dx_val_gl_window,
                      dx_object);

static void dx_val_gl_window_destruct(dx_val_gl_window* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_window_dispatch_construct(dx_val_gl_window_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_gl_window_construct(dx_val_gl_window* self) {
  dx_rti_type* TYPE = dx_val_gl_window_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(self)->type = TYPE;
  return DX_SUCCESS;
}
