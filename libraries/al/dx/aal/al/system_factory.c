#include "dx/aal/al/system_factory.h"

#include "dx/aal/al/system.h"

Core_defineObjectType("dx.aal.al.system_factory",
                      dx_aal_al_system_factory,
                      dx_aal_system_factory);

static Core_Result create_system(dx_aal_al_system** RETURN, dx_aal_al_system_factory* SELF, Core_MessageQueue* msg_queue);

static Core_Result create_system(dx_aal_al_system** RETURN, dx_aal_al_system_factory* SELF, Core_MessageQueue* msg_queue) {
  return dx_aal_al_system_create(RETURN, msg_queue);
}

static void dx_aal_al_system_factory_destruct(dx_aal_al_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_aal_al_system_factory_constructDispatch(dx_aal_al_system_factory_Dispatch* SELF) {
  DX_SYSTEM_FACTORY_DISPATCH(SELF)->create_system = (Core_Result(*)(dx_system**, dx_system_factory*, Core_MessageQueue*)) & create_system;
}

Core_Result dx_aal_al_system_factory_construct(dx_aal_al_system_factory* SELF) {
  Core_BeginConstructor(dx_aal_al_system_factory);
  if (dx_aal_system_factory_construct(DX_AAL_SYSTEM_FACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(dx_aal_al_system_factory);
}

Core_Result dx_aal_al_system_factory_create(dx_aal_al_system_factory** RETURN) {
  DX_CREATE_PREFIX(dx_aal_al_system_factory);
  if (dx_aal_al_system_factory_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
