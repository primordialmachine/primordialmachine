#include "Core/Tests/Signals/ObjectB.h"

DX_DEFINE_OBJECT_TYPE("Core.Tests.Signals.ObjectB",
                      Core_Tests_Signals_ObjectB,
                      Core_Tests_Signals_ObjectA);

static void Core_Tests_Signals_ObjectB_destruct(Core_Tests_Signals_ObjectB* SELF)
{/*Intentionally empty.*/}

static void Core_Tests_Signals_ObjectB_constructDispatch(Core_Tests_Signals_ObjectB_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Tests_Signals_ObjectB_construct(Core_Tests_Signals_ObjectB* SELF) {
  DX_CONSTRUCT_PREFIX(Core_Tests_Signals_ObjectB);
  if (Core_Tests_Signals_ObjectA_construct(CORE_TESTS_SIGNALS_OBJECTA(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_Tests_Signals_ObjectB_create(Core_Tests_Signals_ObjectB** RETURN) {
  DX_CREATE_PREFIX(Core_Tests_Signals_ObjectB);
  if (Core_Tests_Signals_ObjectB_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
