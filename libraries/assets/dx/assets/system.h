#if !defined(DX_ASSETS_SYSTEM_H_INCLUDED)
#define DX_ASSETS_SYSTEM_H_INCLUDED

#include "dx/core.h"
typedef struct Core_Assets_Context Core_Assets_Context;

/// @brief A system for assets.
/// @extends Core_System
Core_declareObjectType("dx.assets.system",
                       dx_assets_system,
                       Core_System);

static inline dx_assets_system* DX_ASSETS_SYSTEM(void* p) {
  return (dx_assets_system*)p;
}

struct dx_assets_system {
  Core_System _parent;
};

static inline dx_assets_system_Dispatch* DX_ASSETS_SYSTEM_DISPATCH(void* p) {
  return (dx_assets_system_Dispatch*)p;
}

struct dx_assets_system_Dispatch {
  Core_System_Dispatch _parent;
  Core_Result(*get_context)(Core_Assets_Context**, dx_assets_system*);
};

/// @brief Construct this system.
/// @constructor{dx_assets_system}
Core_Result dx_assets_system_construct(dx_assets_system* SELF);

/// @create-operator{dx_assets_system}
Core_Result dx_assets_system_create(dx_assets_system** RETURN);

/// @brief Get the Assets context.
/// @param RETURN A pointer to a <code>Core_Assets_Context*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the Assets context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this Assets system is not started.
/// @method{dx_assets_system}
static inline Core_Result dx_assets_system_get_context(Core_Assets_Context** RETURN, dx_assets_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_assets_system, get_context, RETURN, SELF);
}

#endif // DX_ASSETS_SYSTEM_H_INCLUDED
