#include "dx/val/gl/system.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.system",
                      dx_val_gl_system,
                      dx_val_system);

static void dx_val_gl_system_destruct(dx_val_gl_system* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_system_constructDispatch(dx_val_gl_system_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_gl_system_construct(dx_val_gl_system* SELF, dx_msg_queue* msg_queue) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_system);
  if (dx_val_system_construct(DX_VAL_SYSTEM(SELF), msg_queue)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
