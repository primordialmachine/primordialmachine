// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/System.h"

#include "Core/Assets/Context.h"

static Core_Assets_Context* g_context = NULL;

Core_defineObjectType("Core.Assets.System",
                      Core_Assets_System,
                      Core_System);

static Core_Result startup(Core_Assets_System* SELF);

static Core_Result shutdown(Core_Assets_System* SELF);

static Core_Result getContext(Core_Assets_Context** RETURN, Core_Assets_System* SELF);

static Core_Result startup(Core_Assets_System* SELF) {
  if (Core_Assets_Context_create(&g_context)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown(Core_Assets_System* SELF) {
  CORE_UNREFERENCE(g_context);
  g_context = NULL;
  return Core_Success;
}

static Core_Result getContext(Core_Assets_Context** RETURN, Core_Assets_System* SELF) {
  if (!g_context) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  CORE_REFERENCE(g_context);
  *RETURN = g_context;
  return Core_Success;
}

static void Core_Assets_System_destruct(Core_Assets_System* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_System_constructDispatch(Core_Assets_System_Dispatch* SELF) {
  CORE_SYSTEM_DISPATCH(SELF)->startup = (Core_Result(*)(Core_System*)) & startup;
  CORE_SYSTEM_DISPATCH(SELF)->shutdown = (Core_Result(*)(Core_System*)) & shutdown;
  SELF->getContext = &getContext;
}

Core_Result Core_Assets_System_construct(Core_Assets_System* SELF) {
  Core_BeginConstructor(Core_Assets_System);
  Core_EndConstructor(Core_Assets_System);
}

Core_Result Core_Assets_System_create(Core_Assets_System** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_System);
  if (Core_Assets_System_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
