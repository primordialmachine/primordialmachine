#include "dx/assets/system.h"

static dx_assets_system* g_singleton = NULL;

DX_DEFINE_OBJECT_TYPE("dx.assets.system",
                      dx_assets_system,
                      dx_system);

static void dx_assets_system_destruct(dx_assets_system* SELF)
{/*Intentionally empty.*/}

static void dx_assets_system_dispatch_construct(dx_assets_system_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_system_construct(dx_assets_system* SELF) {
  dx_rti_type* TYPE = dx_assets_system_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_system_create(dx_assets_system** RETURN) {
  DX_CREATE_PREFIX(dx_assets_system)
  if (dx_assets_system_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
