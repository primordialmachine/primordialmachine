#if !defined(DX_ASSET_MATERIAL_CONTROLLER_H_INCLUDED)
#define DX_ASSET_MATERIAL_CONTROLLER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_asset_material dx_asset_material;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.material_controller",
                       dx_asset_material_controller,
                       dx_object);

static inline dx_asset_material_controller* DX_ASSET_MATERIAL_CONTROLLER(void* p) {
  return (dx_asset_material_controller*)p;
}

struct dx_asset_material_controller {
  dx_object parent;
};

static inline dx_asset_material_controller_dispatch* DX_ASSET_MATERIAL_CONTROLLER_DISPATCH(void* p) {
  return (dx_asset_material_controller_dispatch*)p;
}

struct dx_asset_material_controller_dispatch {
  dx_object_dispatch parent;
  int (*update)(dx_asset_material_controller* SELF, dx_asset_material* material, dx_f32 delta_seconds);
};

int dx_asset_material_controller_construct(dx_asset_material_controller* SELF);

static inline int dx_asset_material_controller_update(dx_asset_material_controller* SELF, dx_asset_material* material, dx_f32 delta_seconds) {
  DX_OBJECT_VIRTUALCALL(dx_asset_material_controller, update, SELF, material, delta_seconds);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Controller that interpolates the ambient color of a material.
DX_DECLARE_OBJECT_TYPE("dx.asset.material_controllers.ambient_color",
                       dx_asset_material_controllers_ambient_color,
                       dx_asset_material_controller);

static inline dx_asset_material_controllers_ambient_color* DX_ASSET_MATERIAL_CONTROLLERS_AMBIENT_COLOR(void* p) {
  return (dx_asset_material_controllers_ambient_color*)p;
}

struct dx_asset_material_controllers_ambient_color {
  dx_asset_material_controller _parent;
  /// @brief The current time.
  dx_f32 time;
  /// @brief  The current color.
  DX_RGB_N8 current;
};

static inline dx_asset_material_controllers_ambient_color_dispatch* DX_ASSET_MATERIAL_CONTROLLERS_AMBIENT_COLOR_DISPATCH(void* p) {
  return (dx_asset_material_controllers_ambient_color_dispatch*)p;
}

struct dx_asset_material_controllers_ambient_color_dispatch {
  dx_asset_material_controller_dispatch _parent;
};

/// @brief Construct this controller with default values.
/// @param self A pointer to this controller.
/// @remarks The default values are start <code>black</code>, end <code>white</code>, speed <code>0.1 per second</code>, and position <code>0</code>.
/// @default-return
int dx_asset_material_controllers_ambient_color_construct(dx_asset_material_controllers_ambient_color* self);

dx_asset_material_controllers_ambient_color* dx_asset_material_controllers_ambient_color_create();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSET_MATERIAL_CONTROLLER_H_INCLUDED
