#include "Core/Assets/Context.h"

Core_defineObjectType("Core.Assets.Context",
                      Core_Assets_Context,
                      Core_Context);

static void Core_Assets_Context_destruct(Core_Assets_Context* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_Context_constructDispatch(Core_Assets_Context_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Context_construct(Core_Assets_Context* SELF) {
  Core_BeginConstructor(Core_Assets_Context);
  if (Core_Context_construct(CORE_CONTEXT(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Assets_Context);
}

Core_Result Core_Assets_Context_create(Core_Assets_Context** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_Context);
  if (Core_Assets_Context_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
