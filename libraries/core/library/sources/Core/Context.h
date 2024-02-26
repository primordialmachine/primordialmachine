#if !defined(CORE_CONTEXT_H_INCLUDED)
#define CORE_CONTEXT_H_INCLUDED

#include "Core/String.h"

/// @brief
/// Base of all contexts.
/// @details
/// Contexts are acquired for systems (Core.System).
/// A context represents an isolation of state.
/// Frequently, systems only have one single context.
/// Some system, however, allow to create multiple contexts.
/// Althought these contexts are create for the same system, they behave "as if" they were isolated from each other.
Core_declareObjectType("Core.Context",
                       Core_Context,
                       Core_Object);

static inline Core_Context* CORE_CONTEXT(void* p) {
  return (Core_Context*)p;
}

struct Core_Context {
  Core_Object _parent;
};

static inline Core_Context_Dispatch* CORE_CONTEXT_DISPATCH(void* p) {
  return (Core_Context_Dispatch*)p;
}

struct Core_Context_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @contructor{Core_Context}
Core_Result Core_Context_construct(Core_Context* SELF);

#endif // CORE_CONTEXT_H_INCLUDED
