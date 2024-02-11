#if !defined(DX_CORE_CONTEXT_H_INCLUDED)
#define DX_CORE_CONTEXT_H_INCLUDED

#include "Core/String.h"

/// @brief
/// Base of all contexts.
/// @details
/// Contexts are acquired for systems (Core.System).
/// A context represents an isolation of state.
/// Frequently, systems only have one single context.
/// Some system, however, allow to create multiple contexts.
/// Althought these contexts are create for the same system, they behave "as if" they were isolated from each other.
Core_declareObjectType("dx.context",
                       dx_context,
                       Core_Object);

static inline dx_context* DX_CONTEXT(void* p) {
  return (dx_context*)p;
}

struct dx_context {
  Core_Object _parent;
};

static inline dx_context_Dispatch* DX_CONTEXT_DISPATCH(void* p) {
  return (dx_context_Dispatch*)p;
}

struct dx_context_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @contructor{dx_context}
Core_Result dx_context_construct(dx_context* SELF);

#endif // DX_CORE_CONTEXT_H_INCLUDED
