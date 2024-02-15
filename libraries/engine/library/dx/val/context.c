#include "dx/val/context.h"

Core_defineObjectType("Core.Visuals.Context",
                      Core_Visuals_Context,
                      Core_Context);

static void Core_Visuals_Context_destruct(Core_Visuals_Context* SELF)
{/*Intentionally empty.*/}

static void Core_Visuals_Context_constructDispatch(Core_Visuals_Context_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Visuals_Context_construct(Core_Visuals_Context* SELF) {
  Core_BeginConstructor(Core_Visuals_Context);
  if (Core_Context_construct(CORE_CONTEXT(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Visuals_Context);
}
