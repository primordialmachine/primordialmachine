#include "dx/val/gl/system.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.system",
                      dx_val_gl_system,
                      dx_val_system);

static void dx_val_gl_system_destruct(dx_val_gl_system* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_system_dispatch_construct(dx_val_gl_system_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_gl_system_construct(dx_val_gl_system* SELF, dx_msg_queue* msg_queue) {
  dx_rti_type* TYPE = dx_val_gl_system_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_system_construct(DX_VAL_SYSTEM(SELF), msg_queue)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
