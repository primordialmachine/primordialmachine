#if !defined(CORE_TESTS_SIGNALS_OBJECTA_H_INCLUDED)
#define CORE_TESTS_SIGNALS_OBJECTA_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("Core.Tests.Signals.ObjectA",
                       Core_Tests_Signals_ObjectA,
                       Core_Object);

static inline Core_Tests_Signals_ObjectA* CORE_TESTS_SIGNALS_OBJECTA(void* p) {
  return (Core_Tests_Signals_ObjectA*)p;
}

struct Core_Tests_Signals_ObjectA {
  Core_Object _parent;
};

static inline Core_Tests_Signals_ObjectA_Dispatch* CORE_TESTS_SIGNALS_OBJECTA_DISPATCH(void* p) {
  return (Core_Tests_Signals_ObjectA_Dispatch*)p;
}

struct Core_Tests_Signals_ObjectA_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_Tests_Signals_ObjectA_construct(Core_Tests_Signals_ObjectA* SELF);

Core_Result Core_Tests_Signals_ObjectA_create(Core_Tests_Signals_ObjectA** RETURN);

#endif // CORE_TESTS_SIGNALS_OBJECTA_H_INCLUDED
