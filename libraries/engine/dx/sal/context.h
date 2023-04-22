#if !defined(DX_SAL_CONTEXT_H_INCLUDED)
#define DX_SAL_CONTEXT_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.sal.context",
                       dx_sal_context,
                       dx_object)

static inline dx_sal_context* DX_SAL_CONTEXT(void* p) {
  return (dx_sal_context*)p;
}

struct dx_sal_context {
  dx_object _parent;
};

static inline dx_sal_context_dispatch* DX_SAL_CONTEXT_DISPATCH(void* p) {
  return (dx_sal_context_dispatch*)p;
}

struct dx_sal_context_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_sal_context_construct(dx_sal_context* SELF);

#endif // DX_SAL_CONTEXT_H_INCLUDED
