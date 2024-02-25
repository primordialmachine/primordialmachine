#if !defined(DX_ASSETS_VIEWERCONTROLLERS_ROTATEY_H_INCLUDED)
#define DX_ASSETS_VIEWERCONTROLLERS_ROTATEY_H_INCLUDED

#include "Core/Assets/ViewerController.h"

/// @brief Controller that positions the viewer at a certain point and rotates the viewer around the y-axis.
/// The viewer is looking at a specified point.
Core_declareObjectType("Core.Assets.ViewerControllers.RotateY",
                       dx_asset_viewer_controllers_rotate_y,
                       dx_assets_viewer_controller);

static inline dx_asset_viewer_controllers_rotate_y* DX_ASSET_VIEWER_CONTROLLERS_ROTATE_Y(void* p) {
  return (dx_asset_viewer_controllers_rotate_y*)p;
}

struct dx_asset_viewer_controllers_rotate_y {
  dx_assets_viewer_controller _parent;
  /// @brief The number of degrees per second to rotate the viewer by.
  Core_Real32 degrees_per_second;
  /// @brief The position the viewer is looking from.
  DX_VEC3 source;
  /// @brief The position the viewer is looking at.
  DX_VEC3 target;
  /// @brief The up vector of the viewer.
  DX_VEC3 up;
  /// @brief The current degrees to rotate the viewer by.
  Core_Real32 degrees;
};

static inline dx_asset_viewer_controllers_rotate_y_Dispatch* DX_ASSET_VIEWER_CONTROLLERS_ROTATE_Y_DISPATCH(void* p) {
  return (dx_asset_viewer_controllers_rotate_y_Dispatch*)p;
}

struct dx_asset_viewer_controllers_rotate_y_Dispatch {
  dx_assets_viewer_controller_Dispatch _parent;
};

/// @brief Construct this controller with default values.
/// @param SELF A pointer to this controller.
/// @remarks The default values are up <code>(0,1,0)</code>, source <code>(0,0,1)</code>, target <code>(0,0,0)</code>, and degrees per second <code>16</code>.
/// @default-return
/// @todo Fixme.
Core_Result dx_asset_viewer_controllers_rotate_y_construct(dx_asset_viewer_controllers_rotate_y* SELF);

Core_Result dx_asset_viewer_controllers_rotate_y_create(dx_asset_viewer_controllers_rotate_y** RETURN);

#endif // DX_ASSETS_VIEWERCONTROLLERS_ROTATEY_H_INCLUDED
