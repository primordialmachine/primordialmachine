#if !defined(DX_ASSETS_CONTEXT_H_INCLUDED)
#define DX_ASSETS_CONTEXT_H_INCLUDED

#include "dx/core.h"

/// @extends dx_context
Core_declareObjectType("dx.assets.context",
                       dx_assets_context,
                       dx_context);

static inline dx_assets_context* DX_ASSETS_CONTEXT(void* p) {
  return (dx_assets_context*)p;
}

struct dx_assets_context {
  dx_context _parent;
};

static inline dx_assets_context_Dispatch* DX_ASSETS_CONTEXT_DISPATCH(void* p) {
  return (dx_assets_context_Dispatch*)p;
}

struct dx_assets_context_Dispatch {
  dx_context_Dispatch _parent;
};

/// @constructor{dx_assets_context}
Core_Result dx_assets_context_construct(dx_assets_context* SELF);

/// @constructor-operator{dx_assets_context}
Core_Result dx_assets_context_create(dx_assets_context** RETURN);

#endif  // DX_ASSETS_CONTEXT_H_INCLUDED
