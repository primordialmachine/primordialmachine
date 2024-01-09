#if !defined(CORE_TESTS_SIGNALS_OBJECTB_H_INCLUDED)
#define CORE_TESTS_SIGNALS_OBJECTB_H_INCLUDED

#include "Core/Tests/Signals/ObjectA.h"

Core_declareObjectType("Core.Tests.Signals.ObjectB",
                       Core_Tests_Signals_ObjectB,
                       Core_Tests_Signals_ObjectA);

static inline Core_Tests_Signals_ObjectB* CORE_TESTS_SIGNALS_OBJECTB(void* p) {
  return (Core_Tests_Signals_ObjectB*)p;
}

struct Core_Tests_Signals_ObjectB {
  Core_Tests_Signals_ObjectA _parent;
};

static inline Core_Tests_Signals_ObjectB_Dispatch* CORE_TESTS_SIGNALS_OBJECTB_DISPATCH(void* p) {
  return (Core_Tests_Signals_ObjectB_Dispatch*)p;
}

struct Core_Tests_Signals_ObjectB_Dispatch {
  Core_Tests_Signals_ObjectA_Dispatch _parent;
};

Core_Result Core_Tests_Signals_ObjectB_construct(Core_Tests_Signals_ObjectB* SELF);

Core_Result Core_Tests_Signals_ObjectB_create(Core_Tests_Signals_ObjectB** RETURN);

#endif // CORE_TESTS_SIGNALS_OBJECTB_H_INCLUDED
