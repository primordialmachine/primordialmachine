#include "Core/Audials/SystemFactory.h"

Core_defineObjectType("Core.Audials.SystemFactory",
                      Core_Audials_SystemFactory,
                      Core_SystemFactory);

static void Core_Audials_SystemFactory_destruct(Core_Audials_SystemFactory* SELF)
{/*Intentionally empty.*/}

static void Core_Audials_SystemFactory_constructDispatch(Core_Audials_SystemFactory_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Audials_SystemFactory_construct(Core_Audials_SystemFactory* SELF) {
  Core_BeginConstructor(Core_Audials_SystemFactory);
  if (Core_SystemFactory_construct(CORE_SYSTEMFACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Audials_SystemFactory);
}
