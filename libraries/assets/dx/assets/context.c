#include "dx/assets/context.h"

Core_defineObjectType("dx.assets.context",
                      dx_assets_context,
                      dx_context);

static void dx_assets_context_destruct(dx_assets_context* SELF)
{/*Intentionally empty.*/}

static void dx_assets_context_constructDispatch(dx_assets_context_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_context_construct(dx_assets_context* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_context);
  if (dx_context_construct(DX_CONTEXT(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_context_create(dx_assets_context** RETURN) {
  DX_CREATE_PREFIX(dx_assets_context);
  if (dx_assets_context_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
