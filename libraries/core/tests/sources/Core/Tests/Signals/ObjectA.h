#if !defined(CORE_TESTS_SIGNALS_OBJECTA_H_INCLUDED)
#define CORE_TESTS_SIGNALS_OBJECTA_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("Core.Tests.Signals.ObjectA",
                       Core_Tests_Signals_ObjectA,
                       Core_Object);

static inline Core_Tests_Signals_ObjectA* CORE_TESTS_SIGNALS_OBJECTA(void* p) {
  return (Core_Tests_Signals_ObjectA*)p;
}

struct Core_Tests_Signals_ObjectA {
  Core_Object _parent;
};

static inline Core_Tests_Signals_ObjectA_dispatch* CORE_TESTS_SIGNALS_OBJECTA_DISPATCH(void* p) {
  return (Core_Tests_Signals_ObjectA_dispatch*)p;
}

struct Core_Tests_Signals_ObjectA_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_Tests_Signals_ObjectA_construct(Core_Tests_Signals_ObjectA* SELF);

Core_Result Core_Tests_Signals_ObjectA_create(Core_Tests_Signals_ObjectA** RETURN);

#endif // CORE_TESTS_SIGNALS_OBJECTA_H_INCLUDED
