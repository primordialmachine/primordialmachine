#include "dx/assets/system.h"

#include "dx/assets/context.h"

static Core_Assets_Context* g_context = NULL;

Core_defineObjectType("dx.assets.system",
                      dx_assets_system,
                      Core_System);

static Core_Result startup(dx_assets_system* SELF);

static Core_Result shutdown(dx_assets_system* SELF);

static Core_Result get_context(Core_Assets_Context** RETURN, dx_assets_system* SELF);

static Core_Result startup(dx_assets_system* SELF) {
  if (Core_Assets_Context_create(&g_context)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown(dx_assets_system* SELF) {
  CORE_UNREFERENCE(g_context);
  g_context = NULL;
  return Core_Success;
}

static Core_Result get_context(Core_Assets_Context** RETURN, dx_assets_system* SELF) {
  if (!g_context) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  CORE_REFERENCE(g_context);
  *RETURN = g_context;
  return Core_Success;
}

static void dx_assets_system_destruct(dx_assets_system* SELF)
{/*Intentionally empty.*/}

static void dx_assets_system_constructDispatch(dx_assets_system_Dispatch* SELF) {
  CORE_SYSTEM_DISPATCH(SELF)->startup = (Core_Result(*)(Core_System*)) & startup;
  CORE_SYSTEM_DISPATCH(SELF)->shutdown = (Core_Result(*)(Core_System*)) & shutdown;
  SELF->get_context = &get_context;
}

Core_Result dx_assets_system_construct(dx_assets_system* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_system);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_system_create(dx_assets_system** RETURN) {
  DX_CREATE_PREFIX(dx_assets_system);
  if (dx_assets_system_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
