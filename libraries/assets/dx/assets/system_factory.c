#include "dx/assets/system_factory.h"

#include "dx/assets/system.h"

Core_defineObjectType("dx.assets.system_factory",
                      dx_assets_system_factory,
                      Core_SystemFactory);

static Core_Result create_system(dx_assets_system** RETURN, dx_assets_system_factory* SELF, Core_MessageQueue* msg_queue);

static Core_Result create_system(dx_assets_system** RETURN, dx_assets_system_factory* SELF, Core_MessageQueue* msg_queue) {
  return dx_assets_system_create(RETURN);
}

static void dx_assets_system_factory_destruct(dx_assets_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_assets_system_factory_constructDispatch(dx_assets_system_factory_Dispatch* SELF) {
  CORE_SYSTEMFACTORY_DISPATCH(SELF)->create_system = (Core_Result(*)(Core_System**, Core_SystemFactory*, Core_MessageQueue*)) & create_system;
}

Core_Result dx_assets_system_factory_construct(dx_assets_system_factory* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_system_factory);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_system_factory_create(dx_assets_system_factory** RETURN) {
  DX_CREATE_PREFIX(dx_assets_system_factory);
  if (dx_assets_system_factory_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
