// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(DX_ASSETS_CONTEXT_H_INCLUDED)
#define DX_ASSETS_CONTEXT_H_INCLUDED

#include "dx/core.h"

/// A context for the Core_Assets_System.
/// Fully qualified names must be unique in a context.
/// However, different contexts can contain defs of the same fully qualified name.
/// @extends Core_Context
Core_declareObjectType("Core.Assets.Context",
                       Core_Assets_Context,
                       Core_Context);

static inline Core_Assets_Context* CORE_ASSETS_CONTEXT(void* p) {
  return (Core_Assets_Context*)p;
}

struct Core_Assets_Context {
  Core_Context _parent;
};

static inline Core_Assets_Context_Dispatch* CORE_ASSETS_CONTEXT_DISPATCH(void* p) {
  return (Core_Assets_Context_Dispatch*)p;
}

struct Core_Assets_Context_Dispatch {
  Core_Context_Dispatch _parent;
};

/// @constructor{Core_Assets_Context}
Core_Result Core_Assets_Context_construct(Core_Assets_Context* SELF);

/// @constructor-operator{Core_Assets_Context}
Core_Result Core_Assets_Context_create(Core_Assets_Context** RETURN);

#endif  // CORE_ASSETS_CONTEXT_H_INCLUDED
