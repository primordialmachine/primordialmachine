#include "dx/val/gl/system_factory.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.system_factory",
                      dx_val_gl_system_factory,
                      dx_val_system_factory);

static void dx_val_gl_system_factory_destruct(dx_val_gl_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_system_factory_constructDispatch(dx_val_gl_system_factory_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_gl_system_factory_construct(dx_val_gl_system_factory* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_system_factory);
  if (dx_val_system_factory_construct(DX_VAL_SYSTEM_FACTORY(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
