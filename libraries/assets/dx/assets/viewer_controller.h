#if !defined(DX_ASSET_VIEWER_CONTROLLER_H_INCLUDED)
#define DX_ASSET_VIEWER_CONTROLLER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_asset_viewer dx_asset_viewer;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.viewer_controller",
                       dx_asset_viewer_controller,
                       dx_object);

static inline dx_asset_viewer_controller* DX_ASSET_VIEWER_CONTROLLER(void* p) {
  return (dx_asset_viewer_controller*)p;
}

struct dx_asset_viewer_controller {
  dx_object _parent;
  int (*update)(dx_asset_viewer_controller* SELF, dx_asset_viewer* viewer, dx_f32 delta_seconds);
};

struct dx_asset_viewer_controller_dispatch {
  dx_object_dispatch _parent;
};

int dx_asset_viewer_controller_construct(dx_asset_viewer_controller* SELF);

int dx_asset_viewer_controller_update(dx_asset_viewer_controller* SELF, dx_asset_viewer* viewer, dx_f32 delta_seconds);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Controller that positions the viewer at a certain point and rotates the viewer around the y-axis.
/// The viewer is looking at a specified point.
DX_DECLARE_OBJECT_TYPE("dx.asset.viewer_controllers.rotate_y",
                       dx_asset_viewer_controllers_rotate_y,
                       dx_asset_viewer_controller);

static inline dx_asset_viewer_controllers_rotate_y* DX_ASSET_VIEWER_CONTROLLERS_ROTATE_Y(void* p) {
  return (dx_asset_viewer_controllers_rotate_y*)p;
}

struct dx_asset_viewer_controllers_rotate_y {
  dx_asset_viewer_controller _parent;
  /// @brief The number of degrees per second to rotate the viewer by.
  dx_f32 degrees_per_second;
  /// @brief The position the viewer is looking from.
  DX_VEC3 source;
  /// @brief The position the viewer is looking at.
  DX_VEC3 target;
  /// @brief The up vector of the viewer.
  DX_VEC3 up;
  /// @brief The current degrees to rotate the viewer by.
  dx_f32 degrees;
};

struct dx_asset_viewer_controllers_rotate_y_dispatch {
  dx_asset_viewer_controller_dispatch _parent;
};

/// @brief Construct this controller with default values.
/// @param SELF A pointer to this controller.
/// @remarks The default values are up <code>(0,1,0)</code>, source <code>(0,0,1)</code>, target <code>(0,0,0)</code>, and degrees per second <code>16</code>.
/// @default-return
int dx_asset_viewer_controllers_rotate_y_construct(dx_asset_viewer_controllers_rotate_y* SELF);

dx_asset_viewer_controllers_rotate_y* dx_asset_viewer_controllers_rotate_y_create();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSET_VIEWER_CONTROLLER_H_INCLUDED
