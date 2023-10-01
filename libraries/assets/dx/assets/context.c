#include "dx/assets/context.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.context",
                      dx_assets_context,
                      dx_context);

static void dx_assets_context_destruct(dx_assets_context* SELF)
{/*Intentionally empty.*/}

static void dx_assets_context_dispatch_construct(dx_assets_context_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_context_construct(dx_assets_context* SELF) {
  dx_rti_type* TYPE = dx_assets_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_context_construct(DX_CONTEXT(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_context_create(dx_assets_context** RETURN) {
  DX_CREATE_PREFIX(dx_assets_context)
  if (dx_assets_context_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
