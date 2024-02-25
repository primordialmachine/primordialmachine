// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_SYSTEM_H_INCLUDED)
#define CORE_ASSETS_SYSTEM_H_INCLUDED

#include "dx/core.h"
typedef struct Core_Assets_Context Core_Assets_Context;

/// @brief A system for assets.
/// @extends Core_System
Core_declareObjectType("Core.Assets.System",
                       Core_Assets_System,
                       Core_System);

static inline Core_Assets_System* CORE_ASSETS_SYSTEM(void* p) {
  return (Core_Assets_System*)p;
}

struct Core_Assets_System {
  Core_System _parent;
};

static inline Core_Assets_System_Dispatch* CORE_ASSETS_SYSTEM_DISPATCH(void* p) {
  return (Core_Assets_System_Dispatch*)p;
}

struct Core_Assets_System_Dispatch {
  Core_System_Dispatch _parent;
  Core_Result(*getContext)(Core_Assets_Context**, Core_Assets_System*);
};

/// @constructor{Core_Assets_System}
Core_Result Core_Assets_System_construct(Core_Assets_System* SELF);

/// @create-operator{Core_Assets_System}
Core_Result Core_Assets_System_create(Core_Assets_System** RETURN);

/// @brief Get the context.
/// @param RETURN A pointer to a <code>Core_Assets_Context*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this system is not started.
/// @method{Core_Assets_System}
static inline Core_Result Core_Assets_System_getContext(Core_Assets_Context** RETURN, Core_Assets_System* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Assets_System, getContext, RETURN, SELF);
}

#endif // CORE_ASSETS_SYSTEM_H_INCLUDED
