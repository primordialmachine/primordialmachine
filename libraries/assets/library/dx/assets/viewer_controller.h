#if !defined(DX_ASSETS_VIEWER_CONTROLLER_H_INCLUDED)
#define DX_ASSETS_VIEWER_CONTROLLER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_assets_viewer dx_assets_viewer;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.assets.viewer_controller",
                       dx_assets_viewer_controller,
                       Core_Object);

static inline dx_assets_viewer_controller* DX_ASSETS_VIEWER_CONTROLLER(void* p) {
  return (dx_assets_viewer_controller*)p;
}

struct dx_assets_viewer_controller {
  Core_Object _parent;
  Core_Result (*update)(dx_assets_viewer_controller* SELF, dx_assets_viewer* viewer, Core_Real32 delta_seconds);
};

struct dx_assets_viewer_controller_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_assets_viewer_controller_construct(dx_assets_viewer_controller* SELF);

Core_Result dx_assets_viewer_controller_update(dx_assets_viewer_controller* SELF, dx_assets_viewer* viewer, Core_Real32 delta_seconds);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Controller that positions the viewer at a certain point and rotates the viewer around the y-axis.
/// The viewer is looking at a specified point.
Core_declareObjectType("dx.asset.viewer_controllers.rotate_y",
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSETS_VIEWER_CONTROLLER_H_INCLUDED
