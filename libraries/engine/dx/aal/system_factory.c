#include "dx/aal/system_factory.h"

Core_defineObjectType("dx.aal.system_factory",
                      dx_aal_system_factory,
                      Core_SystemFactory);

static void dx_aal_system_factory_destruct(dx_aal_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_aal_system_factory_constructDispatch(dx_aal_system_factory_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_aal_system_factory_construct(dx_aal_system_factory* SELF) {
  Core_BeginConstructor(dx_aal_system_factory);
  if (Core_SystemFactory_construct(CORE_SYSTEMFACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(dx_aal_system_factory);
}
