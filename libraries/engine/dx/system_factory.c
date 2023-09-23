#include "dx/system_factory.h"

DX_DEFINE_OBJECT_TYPE("dx.system_factory",
                      dx_system_factory,
                      dx_object);

static void dx_system_factory_destruct(dx_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_system_factory_dispatch_construct(dx_system_factory_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_system_factory_construct(dx_system_factory* SELF) {
  dx_rti_type* TYPE = dx_system_factory_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
