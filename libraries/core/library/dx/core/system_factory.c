#include "dx/core/system_factory.h"

Core_defineObjectType("Core.SystemFactory",
                      Core_SystemFactory,
                      Core_Object);

static void Core_SystemFactory_destruct(Core_SystemFactory* SELF)
{/*Intentionally empty.*/}

static void Core_SystemFactory_constructDispatch(Core_SystemFactory_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_SystemFactory_construct(Core_SystemFactory* SELF) {
  Core_BeginConstructor(Core_SystemFactory);
  Core_EndConstructor(Core_SystemFactory);
}
