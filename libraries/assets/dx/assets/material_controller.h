#if !defined(DX_ASSETS_MATERIAL_CONTROLLER_H_INCLUDED)
#define DX_ASSETS_MATERIAL_CONTROLLER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_assets_material dx_assets_material;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.assets.material_controller",
                       dx_assets_material_controller,
                       Core_Object);

static inline dx_assets_material_controller* DX_ASSETS_MATERIAL_CONTROLLER(void* p) {
  return (dx_assets_material_controller*)p;
}

struct dx_assets_material_controller {
  Core_Object _parent;
};

static inline dx_assets_material_controller_dispatch* DX_ASSETS_MATERIAL_CONTROLLER_DISPATCH(void* p) {
  return (dx_assets_material_controller_dispatch*)p;
}

struct dx_assets_material_controller_dispatch {
  Core_Object_Dispatch parent;
  Core_Result (*update)(dx_assets_material_controller* SELF, dx_assets_material* material, Core_Real32 delta_seconds);
};

/// @constructor{dx_assets_material_controller}
Core_Result dx_assets_material_controller_construct(dx_assets_material_controller* SELF);

static inline Core_Result dx_assets_material_controller_update(dx_assets_material_controller* SELF, dx_assets_material* material, Core_Real32 delta_seconds) {
  DX_OBJECT_VIRTUALCALL(dx_assets_material_controller, update, SELF, material, delta_seconds);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Controller that interpolates the ambient color of a material.
DX_DECLARE_OBJECT_TYPE("dx.assets.material_controllers.ambient_color",
                       dx_assets_material_controllers_ambient_color,
                       dx_assets_material_controller);

static inline dx_assets_material_controllers_ambient_color* DX_ASSETS_MATERIAL_CONTROLLERS_AMBIENT_COLOR(void* p) {
  return (dx_assets_material_controllers_ambient_color*)p;
}

struct dx_assets_material_controllers_ambient_color {
  dx_assets_material_controller _parent;
  /// @brief The current time.
  Core_Real32 time;
  /// @brief  The current color.
  /// @todo Fixme.
  DX_RGB_N8 current;
};

static inline dx_assets_material_controllers_ambient_color_dispatch* DX_ASSETS_MATERIAL_CONTROLLERS_AMBIENT_COLOR_DISPATCH(void* p) {
  return (dx_assets_material_controllers_ambient_color_dispatch*)p;
}

struct dx_assets_material_controllers_ambient_color_dispatch {
  dx_assets_material_controller_dispatch _parent;
};

/// @detail
/// The object is constructed with default values.
/// The default values are start <code>black</code>, end <code>white</code>, speed <code>0.1 per second</code>, and position <code>0</code>.
/// @constructor{dx_assets_material_controllers_ambient_color}
Core_Result dx_assets_material_controllers_ambient_color_construct(dx_assets_material_controllers_ambient_color* SELF);

/// @create-operator{dx_assets_material_controllers_ambient_color}
Core_Result dx_assets_material_controllers_ambient_color_create(dx_assets_material_controllers_ambient_color** RETURN);

#endif // DX_ASSETS_MATERIAL_CONTROLLER_H_INCLUDED
