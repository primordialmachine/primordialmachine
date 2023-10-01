#if !defined(DX_CORE_CONTEXT_H_INCLUDED)
#define DX_CORE_CONTEXT_H_INCLUDED

#include "dx/core.h"

/// @brief Base of all contexts. A context is a provides access to functionality.
DX_DECLARE_OBJECT_TYPE("dx.context",
                       dx_context,
                       dx_object);

static inline dx_context* DX_CONTEXT(void* p) {
  return (dx_context*)p;
}

struct dx_context {
  dx_object _parent;
};

static inline dx_context_dispatch* DX_CONTEXT_DISPATCH(void* p) {
  return (dx_context_dispatch*)p;
}

struct dx_context_dispatch {
  dx_object_dispatch _parent;
};

/// @contructor{dx_context}
dx_result dx_context_construct(dx_context* SELF);

#endif // DX_CORE_CONTEXT_H_INCLUDED
