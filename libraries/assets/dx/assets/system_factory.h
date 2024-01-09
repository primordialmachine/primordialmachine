#if !defined(DX_ASSETS_SYSTEM_FACTORY_H_INCLUDED)
#define DX_ASSETS_SYSTEM_FACTORY_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("dx.assets.system_factory",
                       dx_assets_system_factory,
                       dx_system_factory);

static inline dx_assets_system_factory* DX_ASSETS_SYSTEM_FACTORY(void* p) {
  return (dx_assets_system_factory*)p;
}

struct dx_assets_system_factory {
  dx_system_factory _parent;
};

static inline dx_assets_system_factory_Dispatch* DX_ASSETS_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_assets_system_factory_Dispatch*)p;
}

struct dx_assets_system_factory_Dispatch {
  dx_system_factory_Dispatch _parent;
};

/// @brief Construct this assets system factory.
/// @param SELF A pointer to this assets system factory.
/// @method-call
Core_Result dx_assets_system_factory_construct(dx_assets_system_factory* SELF);

Core_Result dx_assets_system_factory_create(dx_assets_system_factory** RETURN);

#endif // DX_ASSETS_SYSTEM_FACTORY_H_INCLUDED
