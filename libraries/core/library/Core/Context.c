#include "Core/Context.h"

Core_defineObjectType("Core.Context",
                      Core_Context,
                      Core_Object);

static void Core_Context_destruct(Core_Context* SELF)
{/*Intentionally empty.*/}

static void Core_Context_constructDispatch(Core_Context_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Context_construct(Core_Context* SELF) {
  Core_BeginConstructor(Core_Context);
  Core_EndConstructor(Core_Context);
}
