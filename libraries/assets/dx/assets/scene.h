#if !defined(DX_ASSETS_SCENE_H_INCLUDED)
#define DX_ASSETS_SCENE_H_INCLUDED

#include "dx/core.h"

/// @brief A scene asset.
DX_DECLARE_OBJECT_TYPE("dx.assets.scene",
                       dx_assets_scene,
                       dx_object);

static inline dx_assets_scene* DX_ASSETS_SCENE(void* p) {
    return (dx_assets_scene*)p;
}

struct dx_assets_scene {
  dx_object _parent;
  /// @brief The list of assets in this scene.
  dx_inline_object_array assets;
};

struct dx_assets_scene_dispatch {
  dx_object_dispatch _parent;
};

/// @constructor{dx_assets_scene}.
dx_result dx_assets_scene_construct(dx_assets_scene* SELF);

dx_result dx_assets_scene_create(dx_assets_scene** RETURN);

#endif // DX_ASSETS_SCENE_H_INCLUDED
