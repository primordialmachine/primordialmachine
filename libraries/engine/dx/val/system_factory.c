#include "dx/val/system_factory.h"

DX_DEFINE_OBJECT_TYPE("dx.val.system_factory",
                      dx_val_system_factory,
                      dx_system_factory);

static void dx_val_system_factory_destruct(dx_val_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_val_system_factory_dispatch_construct(dx_val_system_factory_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_system_factory_construct(dx_val_system_factory* SELF) {
  dx_rti_type* TYPE = dx_val_system_factory_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_system_factory_construct(DX_SYSTEM_FACTORY(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
