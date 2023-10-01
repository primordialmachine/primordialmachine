#if !defined(DX_ASSET_VIEWER_INSTANCE_H_INCLUDED)
#define DX_ASSET_VIEWER_INSTANCE_H_INCLUDED

#include "dx/assets/viewer.h"
#include "dx/assets/reference.h"

/// @brief A viewer instance asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.viewer_instance",
                       dx_asset_viewer_instance,
                       dx_object);

static inline dx_asset_viewer_instance* DX_ASSET_VIEWER_INSTANCE(void* p) {
    return (dx_asset_viewer_instance*)p;
}

struct dx_asset_viewer_instance {
  dx_object _parent;
  /// @brief Reference to the viewer of this viewer instance.
  dx_asset_reference* viewer_reference;
  /// @brief The world matrix of this viewer instance.
  DX_MAT4 world_matrix;
};

static inline dx_asset_viewer_instance_dispatch* DX_ASSET_VIEWER_INSTANCE_DISPATCH(void* p) {
  return (dx_asset_viewer_instance_dispatch*)p;
}

struct dx_asset_viewer_instance_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this dx_asset_viewer_instance object.
/// @param SELF A pointer to this dx_asset_viewer_instance object.
/// @param viewer_reference A pointer to the viewer reference of this viewer instance.
/// @error #DX_ERROR_INVALID_ARGUMENT @a viewer_reference is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @method-call
int dx_asset_viewer_instance_construct(dx_asset_viewer_instance* SELF, dx_asset_reference* viewer_reference);

/// @param viewer_reference A pointer to the viewer reference of this viewer instance.
/// @error #DX_ERROR_INVALID_ARGUMENT @a viewer_reference is a null pointer
/// @create-operator{dx_asset_viewer_instance}
dx_result dx_asset_viewer_instance_create(dx_asset_viewer_instance** RETURN, dx_asset_reference* viewer_reference);

#endif // DX_ASSET_VIEWER_INSTANCE_H_INCLUDED
