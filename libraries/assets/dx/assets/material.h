#if !defined(DX_ASSETS_MATERIAL_H_INCLUDED)
#define DX_ASSETS_MATERIAL_H_INCLUDED

#include "dx/assets/reference.h"
typedef struct dx_assets_color_rgb_n8 dx_assets_color_rgb_n8;
typedef struct dx_assets_material_controller dx_assets_material_controller;

/// @brief A material asset.
DX_DECLARE_OBJECT_TYPE("dx.assets.material",
                       dx_assets_material,
                       Core_Object);

static inline dx_assets_material* DX_ASSETS_MATERIAL(void* p) {
    return (dx_assets_material*)p;
}

struct dx_assets_material {
  Core_Object _parent;
  /// @brief A pointer to the ADL name of this material.
  Core_String* name;
  /// @brief The ambient color of this material.
  dx_asset_reference* ambient_color;
  /// @brief Pointer to the ambient texture of this material or the null pointer.
  /// @default The null pointer.
  dx_asset_reference* ambient_texture_reference;
  /// @brief A pointer to the viewer controller or a null pointer.
  dx_assets_material_controller* controller;
};

static inline dx_assets_material_dispatch* DX_ASSETS_MATERIAL_DISPATCH(void* p) {
  return (dx_assets_material_dispatch*)p;
}

struct dx_assets_material_dispatch {
  Core_Object_Dispatch _parent;
};

/// @detail The material is constructed with default values.
/// @param name A pointer to the ADL name of this material.
/// @error #Core_Error_AllocationFailed an allocation failed
/// @constructor{dx_asset_material}
Core_Result dx_assets_material_construct(dx_assets_material* SELF, Core_String* name);

/// @create-operator{dx_assets_material}
/// @detail The object is created with default values.
/// @param name A pointer to the ADL name of this material.
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_assets_material_create(dx_assets_material** RETURN, Core_String* name);

/// @brief Set the ambient color.
/// @param ambient_color A pointer to color.
/// @error #Core_Error_ArgumentInvalid @a ambient_color is a null pointer,
/// @method{dx_assets_material}
Core_Result dx_assets_material_set_ambient_color(dx_assets_material* SELF, dx_assets_color_rgb_n8* ambient_color);

/// @brief Set the ambient texture.
/// @param ambient_texture_reference A pointer to the ambient_texture_reference or the null pointer.
/// @method{dx_assets_material}
Core_Result dx_assets_material_set_ambient_texture(dx_assets_material* SELF, dx_asset_reference* ambient_texture_reference);

#endif // DX_ASSETS_MATERIAL_H_INCLUDED
