#if !defined(DX_AAL_CONTEXT_H_INCLUDED)
#define DX_AAL_CONTEXT_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.aal.context",
                       dx_aal_context,
                       dx_object)

static inline dx_aal_context* DX_AAL_CONTEXT(void* p) {
  return (dx_aal_context*)p;
}

struct dx_aal_context {
  dx_object _parent;
};

static inline dx_aal_context_dispatch* DX_AAL_CONTEXT_DISPATCH(void* p) {
  return (dx_aal_context_dispatch*)p;
}

struct dx_aal_context_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_aal_context_construct(dx_aal_context* SELF);

#endif // DX_AAL_CONTEXT_H_INCLUDED
