#include "dx/assets/system_factory.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.system_factory",
                      dx_assets_system_factory,
                      dx_system_factory);

static void dx_assets_system_factory_destruct(dx_assets_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_assets_system_factory_dispatch_construct(dx_assets_system_factory_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_system_factory_construct(dx_assets_system_factory* SELF) {
  dx_rti_type* TYPE = dx_assets_system_factory_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_system_factory_create(dx_assets_system_factory** RETURN) {
  DX_CREATE_PREFIX(dx_assets_system_factory)
  if (dx_assets_system_factory_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
