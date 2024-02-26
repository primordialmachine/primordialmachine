#include "Core/Visuals/SystemFactory.h"

Core_defineObjectType("Core.Visuals.SystemFactory",
                      Core_Visuals_SystemFactory,
                      Core_SystemFactory);

static void Core_Visuals_SystemFactory_destruct(Core_Visuals_SystemFactory* SELF)
{/*Intentionally empty.*/}

static void Core_Visuals_SystemFactory_constructDispatch(Core_Visuals_SystemFactory_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Visuals_SystemFactory_construct(Core_Visuals_SystemFactory* SELF) {
  Core_BeginConstructor(Core_Visuals_SystemFactory);
  if (Core_SystemFactory_construct(CORE_SYSTEMFACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Visuals_SystemFactory);
}
