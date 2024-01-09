#if !defined(DX_CORE_CONTEXT_H_INCLUDED)
#define DX_CORE_CONTEXT_H_INCLUDED

#include "dx/core/string.h"

/// @brief Base of all contexts. A context is a provides access to functionality.
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
