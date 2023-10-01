#include "dx/assets/system.h"

#include "dx/assets/context.h"

static dx_assets_context* g_context = NULL;

DX_DEFINE_OBJECT_TYPE("dx.assets.system",
                      dx_assets_system,
                      dx_system);

static dx_result startup(dx_assets_system* SELF);

static dx_result shutdown(dx_assets_system* SELF);

static dx_result get_context(dx_assets_context** RETURN, dx_assets_system* SELF);

static dx_result startup(dx_assets_system* SELF) {
  if (dx_assets_context_create(&g_context)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result shutdown(dx_assets_system* SELF) {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  return DX_SUCCESS;
}

static dx_result get_context(dx_assets_context** RETURN, dx_assets_system* SELF) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}

static void dx_assets_system_destruct(dx_assets_system* SELF)
{/*Intentionally empty.*/}

static void dx_assets_system_dispatch_construct(dx_assets_system_dispatch* SELF) {
  DX_SYSTEM_DISPATCH(SELF)->startup = (dx_result(*)(dx_system*)) & startup;
  DX_SYSTEM_DISPATCH(SELF)->shutdown = (dx_result(*)(dx_system*)) & shutdown;
  SELF->get_context = &get_context;
}

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
