#include "Core/Assets/SystemFactory.h"

#include "Core/Assets/System.h"

Core_defineObjectType("Core.Assets.SystemFactory",
                      Core_Assets_SystemFactory,
                      Core_SystemFactory);

static Core_Result createSystem(Core_Assets_System** RETURN, Core_Assets_SystemFactory* SELF, Core_MessageQueue* msg_queue);

static Core_Result createSystem(Core_Assets_System** RETURN, Core_Assets_SystemFactory* SELF, Core_MessageQueue* msg_queue) {
  return Core_Assets_System_create(RETURN);
}

static void Core_Assets_SystemFactory_destruct(Core_Assets_SystemFactory* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_SystemFactory_constructDispatch(Core_Assets_SystemFactory_Dispatch* SELF) {
  CORE_SYSTEMFACTORY_DISPATCH(SELF)->createSystem = (Core_Result(*)(Core_System**, Core_SystemFactory*, Core_MessageQueue*)) & createSystem;
}

Core_Result Core_Assets_SystemFactory_construct(Core_Assets_SystemFactory* SELF) {
  Core_BeginConstructor(Core_Assets_SystemFactory);
  Core_EndConstructor(Core_Assets_SystemFactory);
}

Core_Result Core_Assets_SystemFactory_create(Core_Assets_SystemFactory** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_SystemFactory);
  if (Core_Assets_SystemFactory_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
