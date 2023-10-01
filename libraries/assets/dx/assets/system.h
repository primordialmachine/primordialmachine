#if !defined(DX_ASSETS_SYSTEM_H_INCLUDED)
#define DX_ASSETS_SYSTEM_H_INCLUDED

#include "dx/core.h"

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
};

/// @brief Construct this system.
/// @param SELF A pointer to this system.
/// @method-call
dx_result dx_assets_system_construct(dx_assets_system* SELF);

dx_result dx_assets_system_create(dx_assets_system** RETURN);

#endif // DX_ASSETS_SYSTEM_H_INCLUDED
