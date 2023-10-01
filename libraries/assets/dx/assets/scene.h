#if !defined(DX_ASSET_SCENE_H_INCLUDED)
#define DX_ASSET_SCENE_H_INCLUDED

#include "dx/core.h"
//#include "dx/adl/symbols.h"

/// @brief A scene asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.scene",
                       dx_asset_scene,
                       dx_object)

static inline dx_asset_scene* DX_ASSET_SCENE(void* p) {
    return (dx_asset_scene*)p;
}

struct dx_asset_scene {
  dx_object _parent;
  /// @brief The list of assets in this scene.
  dx_inline_object_array assets;
};

struct dx_asset_scene_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this dx_asset_scene object with default values.
/// @param SELF A pointer to this dx_asset_scene object.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @todo Fixme.
int dx_asset_scene_construct(dx_asset_scene* SELF);

/// @brief Create a dx_asset_scene object with default values.
/// @return A pointer to the dx_asset_material object on success. The null pointer failure.
/// @default-failure
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @todo Fixme.
dx_result dx_asset_scene_create(dx_asset_scene** RETURN);

#endif // DX_ASSET_SCENE_H_INCLUDED
