/// @file dx/asset/viewer.h
/// @brief Representation of a "viewer" asset.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_ASSET_VIEWER_H_INCLUDED)
#define DX_ASSET_VIEWER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_assets_optics dx_assets_optics;
typedef struct dx_assets_viewer_controller dx_assets_viewer_controller;

/// @brief A viewer asset.
Core_declareObjectType("dx.assets.viewer",
                       dx_assets_viewer,
                       Core_Object);

static inline dx_assets_viewer* DX_ASSETS_VIEWER(void* p) {
  return (dx_assets_viewer*)p;
}

struct dx_assets_viewer {
  Core_Object _parent;
  /// @brief A pointer to the optics of this viewer.
  /// @default dx_assets_optics_perspective with default values.
  dx_assets_optics* optics;
  /// @brief A pointer to the ADL name of this viewer.
  Core_String* name;
  /// @brief The up vector.
  DX_VEC3 up;
  /// @brief The source position.
  DX_VEC3 source;
  /// @brief The target position.
  DX_VEC3 target;
  /// @brief A pointer to the viewer controller or a null pointer.
  dx_assets_viewer_controller* controller;
};

static inline dx_assets_viewer_Dispatch* DX_ASSETS_VIEWER_DISPATCH(void* p) {
  return (dx_assets_viewer_Dispatch*)p;
}

struct dx_assets_viewer_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @default
/// The object is constructed with default values.
/// The default values are up <code>(0,1,0)</code>, source <code>(0,0,0)</code>, and target <code>(0,0,-1)</code>.
/// @param name A pointer to the "Asset Definition Language" name of this viewer object.
/// @constructor{dx_assets_viewer}
Core_Result dx_assets_viewer_construct(dx_assets_viewer* SELF, Core_String* name);

/// @create-operator{dx_assets_viewer}
Core_Result dx_assets_viewer_create(dx_assets_viewer** RETURN, Core_String* name);

#endif // DX_ASSETS_VIEWER_H_INCLUDED
