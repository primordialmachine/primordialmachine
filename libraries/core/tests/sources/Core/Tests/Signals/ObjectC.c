#include "Core/Tests/Signals/ObjectC.h"

Core_defineObjectType("Core.Tests.Signals.ObjectC",
                      Core_Tests_Signals_ObjectC,
                      Core_Tests_Signals_ObjectB);

static void Core_Tests_Signals_ObjectC_destruct(Core_Tests_Signals_ObjectC* SELF)
{/*Intentionally empty.*/}

static void Core_Tests_Signals_ObjectC_constructDispatch(Core_Tests_Signals_ObjectC_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Tests_Signals_ObjectC_construct(Core_Tests_Signals_ObjectC* SELF) {
  DX_CONSTRUCT_PREFIX(Core_Tests_Signals_ObjectC);
  if (Core_Tests_Signals_ObjectB_construct(CORE_TESTS_SIGNALS_OBJECTB(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_Tests_Signals_ObjectC_create(Core_Tests_Signals_ObjectC** RETURN) {
  DX_CREATE_PREFIX(Core_Tests_Signals_ObjectC);
  if (Core_Tests_Signals_ObjectC_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}