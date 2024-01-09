#if !defined(DX_ASSETS_SCENE_H_INCLUDED)
#define DX_ASSETS_SCENE_H_INCLUDED

#include "dx/core.h"

/// @brief A scene asset.
Core_declareObjectType("dx.assets.scene",
                       dx_assets_scene,
                       Core_Object);

static inline dx_assets_scene* DX_ASSETS_SCENE(void* p) {
    return (dx_assets_scene*)p;
}

struct dx_assets_scene {
  Core_Object _parent;
  /// @brief The list of assets in this scene.
  dx_inline_object_array assets;
};

struct dx_assets_scene_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @constructor{dx_assets_scene}.
Core_Result dx_assets_scene_construct(dx_assets_scene* SELF);

Core_Result dx_assets_scene_create(dx_assets_scene** RETURN);

#endif // DX_ASSETS_SCENE_H_INCLUDED
