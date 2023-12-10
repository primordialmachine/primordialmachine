#if !defined(DX_ASSETS_SYSTEM_FACTORY_H_INCLUDED)
#define DX_ASSETS_SYSTEM_FACTORY_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.assets.system_factory",
                       dx_assets_system_factory,
                       dx_system_factory);

static inline dx_assets_system_factory* DX_ASSETS_SYSTEM_FACTORY(void* p) {
  return (dx_assets_system_factory*)p;
}

struct dx_assets_system_factory {
  dx_system_factory _parent;
};

static inline dx_assets_system_factory_dispatch* DX_ASSETS_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_assets_system_factory_dispatch*)p;
}

struct dx_assets_system_factory_dispatch {
  dx_system_factory_dispatch _parent;
};

/// @brief Construct this assets system factory.
/// @param SELF A pointer to this assets system factory.
/// @method-call
Core_Result dx_assets_system_factory_construct(dx_assets_system_factory* SELF);

Core_Result dx_assets_system_factory_create(dx_assets_system_factory** RETURN);

#endif // DX_ASSETS_SYSTEM_FACTORY_H_INCLUDED
