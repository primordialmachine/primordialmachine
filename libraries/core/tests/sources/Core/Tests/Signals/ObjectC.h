#if !defined(CORE_TESTS_SIGNALS_OBJECTC_H_INCLUDED)
#define CORE_TESTS_SIGNALS_OBJECTC_H_INCLUDED

#include "Core/Tests/Signals/ObjectB.h"

DX_DECLARE_OBJECT_TYPE("Core.Tests.Signals.ObjectC",
                       Core_Tests_Signals_ObjectC,
                       Core_Tests_Signals_ObjectB);

static inline Core_Tests_Signals_ObjectC* CORE_TESTS_SIGNALS_OBJECTC(void* p) {
  return (Core_Tests_Signals_ObjectC*)p;
}

struct Core_Tests_Signals_ObjectC {
  Core_Tests_Signals_ObjectB _parent;
};

static inline Core_Tests_Signals_ObjectC_dispatch* CORE_TESTS_SIGNALS_OBJECTC_DISPATCH(void* p) {
  return (Core_Tests_Signals_ObjectC_dispatch*)p;
}

struct Core_Tests_Signals_ObjectC_dispatch {
  Core_Tests_Signals_ObjectB_dispatch _parent;
};

Core_Result Core_Tests_Signals_ObjectC_construct(Core_Tests_Signals_ObjectC* SELF);

Core_Result Core_Tests_Signals_ObjectC_create(Core_Tests_Signals_ObjectC** RETURN);

#endif // CORE_TESTS_SIGNALS_OBJECTC_H_INCLUDED
