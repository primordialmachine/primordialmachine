#include "Core/Audials/Context.h"

Core_defineObjectType("Core.Audials.Context",
                      Core_Audials_Context,
                      Core_Context);

static void Core_Audials_Context_destruct(Core_Audials_Context* SELF)
{/*Intentionally empty.*/}

static void Core_Audials_Context_constructDispatch(Core_Audials_Context_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Audials_Context_construct(Core_Audials_Context* SELF) {
  Core_BeginConstructor(Core_Audials_Context);
  if (Core_Context_construct(CORE_CONTEXT(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Audials_Context);
}
