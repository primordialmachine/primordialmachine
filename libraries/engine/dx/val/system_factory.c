#include "dx/val/system_factory.h"

Core_defineObjectType("dx.val.system_factory",
                      dx_val_system_factory,
                      Core_SystemFactory);

static void dx_val_system_factory_destruct(dx_val_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_val_system_factory_constructDispatch(dx_val_system_factory_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_system_factory_construct(dx_val_system_factory* SELF) {
  Core_BeginConstructor(dx_val_system_factory);
  if (Core_SystemFactory_construct(CORE_SYSTEMFACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(dx_val_system_factory);
}
