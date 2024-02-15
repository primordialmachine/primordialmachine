#include "dx/aal/al/system_factory.h"

#include "dx/aal/al/system.h"

Core_defineObjectType("Core.Audials.Al.SystemFactory",
                      Core_Audials_Al_SystemFactory,
                      Core_Audials_SystemFactory);

static Core_Result createSystem(dx_aal_al_system** RETURN, Core_Audials_Al_SystemFactory* SELF, Core_MessageQueue* msg_queue);

static Core_Result createSystem(dx_aal_al_system** RETURN, Core_Audials_Al_SystemFactory* SELF, Core_MessageQueue* msg_queue) {
  return dx_aal_al_system_create(RETURN, msg_queue);
}

static void Core_Audials_Al_SystemFactory_destruct(Core_Audials_Al_SystemFactory* SELF)
{/*Intentionally empty.*/}

static void Core_Audials_Al_SystemFactory_constructDispatch(Core_Audials_Al_SystemFactory_Dispatch* SELF) {
  CORE_SYSTEMFACTORY_DISPATCH(SELF)->createSystem = (Core_Result(*)(Core_System**, Core_SystemFactory*, Core_MessageQueue*)) & createSystem;
}

Core_Result Core_Audials_Al_SystemFactory_construct(Core_Audials_Al_SystemFactory* SELF) {
  Core_BeginConstructor(Core_Audials_Al_SystemFactory);
  if (Core_Audials_SystemFactory_construct(CORE_AUDIALS_SYSTEMFACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Audials_Al_SystemFactory);
}

Core_Result Core_Audials_Al_SystemFactory_create(Core_Audials_Al_SystemFactory** RETURN) {
  DX_CREATE_PREFIX(Core_Audials_Al_SystemFactory);
  if (Core_Audials_Al_SystemFactory_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
