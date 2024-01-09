#include "Core/Tests/Signals/ObjectA.h"

Core_defineObjectType("Core.Tests.Signals.ObjectA",
                      Core_Tests_Signals_ObjectA,
                      Core_Object);

static void Core_Tests_Signals_ObjectA_destruct(Core_Tests_Signals_ObjectA* SELF)
{/*Intentionally empty.*/}

static void Core_Tests_Signals_ObjectA_constructDispatch(Core_Tests_Signals_ObjectA_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Tests_Signals_ObjectA_construct(Core_Tests_Signals_ObjectA* SELF) {
  DX_CONSTRUCT_PREFIX(Core_Tests_Signals_ObjectA);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_Tests_Signals_ObjectA_create(Core_Tests_Signals_ObjectA** RETURN) {
  DX_CREATE_PREFIX(Core_Tests_Signals_ObjectA);
  if (Core_Tests_Signals_ObjectA_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}