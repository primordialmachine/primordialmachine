#if !defined(DX_ASSETS_SYSTEM_H_INCLUDED)
#define DX_ASSETS_SYSTEM_H_INCLUDED

#include "dx/core.h"
typedef struct dx_assets_context dx_assets_context;

/// @brief A system for assets.
/// @extends dx_system
DX_DECLARE_OBJECT_TYPE("dx.assets.system",
                       dx_assets_system,
                       dx_system);

static inline dx_assets_system* DX_ASSETS_SYSTEM(void* p) {
  return (dx_assets_system*)p;
}

struct dx_assets_system {
  dx_system _parent;
};

static inline dx_assets_system_dispatch* DX_ASSETS_SYSTEM_DISPATCH(void* p) {
  return (dx_assets_system_dispatch*)p;
}

struct dx_assets_system_dispatch {
  dx_system_dispatch _parent;
  dx_result(*get_context)(dx_assets_context**, dx_assets_system*);
};

/// @brief Construct this system.
/// @constructor{dx_assets_system}
dx_result dx_assets_system_construct(dx_assets_system* SELF);

/// @create-operator{dx_assets_system}
dx_result dx_assets_system_create(dx_assets_system** RETURN);

/// @brief Get the Assets context.
/// @param RETURN A pointer to a <code>dx_assets_context*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the Assets context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this Assets system is not started.
/// @method{dx_assets_system}
static inline dx_result dx_assets_system_get_context(dx_assets_context** RETURN, dx_assets_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_assets_system, get_context, RETURN, SELF);
}

#endif // DX_ASSETS_SYSTEM_H_INCLUDED
