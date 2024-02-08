#include "Core/Tests/Signals/ObjectB.h"

Core_defineObjectType("Core.Tests.Signals.ObjectB",
                      Core_Tests_Signals_ObjectB,
                      Core_Tests_Signals_ObjectA);

static void Core_Tests_Signals_ObjectB_destruct(Core_Tests_Signals_ObjectB* SELF)
{/*Intentionally empty.*/}

static void Core_Tests_Signals_ObjectB_constructDispatch(Core_Tests_Signals_ObjectB_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Tests_Signals_ObjectB_construct(Core_Tests_Signals_ObjectB* SELF) {
  Core_BeginConstructor(Core_Tests_Signals_ObjectB);
  if (Core_Tests_Signals_ObjectA_construct(CORE_TESTS_SIGNALS_OBJECTA(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Tests_Signals_ObjectB);
}

Core_Result Core_Tests_Signals_ObjectB_create(Core_Tests_Signals_ObjectB** RETURN) {
  DX_CREATE_PREFIX(Core_Tests_Signals_ObjectB);
  if (Core_Tests_Signals_ObjectB_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
