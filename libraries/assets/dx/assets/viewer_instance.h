#if !defined(DX_ASSETS_VIEWER_INSTANCE_H_INCLUDED)
#define DX_ASSETS_VIEWER_INSTANCE_H_INCLUDED

#include "dx/assets/viewer.h"
#include "dx/assets/reference.h"

/// @brief A viewer instance asset.
DX_DECLARE_OBJECT_TYPE("dx.assets.viewer_instance",
                       dx_assets_viewer_instance,
                       Core_Object);

static inline dx_assets_viewer_instance* DX_ASSETS_VIEWER_INSTANCE(void* p) {
    return (dx_assets_viewer_instance*)p;
}

struct dx_assets_viewer_instance {
  Core_Object _parent;
  /// @brief Reference to the viewer of this viewer instance.
  dx_asset_reference* viewer_reference;
  /// @brief The world matrix of this viewer instance.
  DX_MAT4 world_matrix;
};

static inline dx_assets_viewer_instance_dispatch* DX_ASSETS_VIEWER_INSTANCE_DISPATCH(void* p) {
  return (dx_assets_viewer_instance_dispatch*)p;
}

struct dx_assets_viewer_instance_dispatch {
  Core_Object_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @param viewer_reference A pointer to the viewer reference of this viewer instance.
/// @error #Core_Error_ArgumentInvalid @a viewer_reference is a null pointer.
/// @constructor{dx_asset_viewer_instance}
Core_Result dx_assets_viewer_instance_construct(dx_assets_viewer_instance* SELF, dx_asset_reference* viewer_reference);

/// @param viewer_reference A pointer to the viewer reference of this viewer instance.
/// @error #Core_Error_ArgumentInvalid @a viewer_reference is a null pointer
/// @create-operator{dx_asset_viewer_instance}
Core_Result dx_assets_viewer_instance_create(dx_assets_viewer_instance** RETURN, dx_asset_reference* viewer_reference);

#endif // DX_ASSETS_VIEWER_INSTANCE_H_INCLUDED
