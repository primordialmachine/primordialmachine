#if !defined(DX_ASSET_MATERIAL_H_INCLUDED)
#define DX_ASSET_MATERIAL_H_INCLUDED

#include "dx/assets/texture.h"
#include "dx/assets/color_rgb_n8.h"
typedef struct dx_asset_material_controller dx_asset_material_controller;

/// @brief A material asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.material",
                       dx_asset_material,
                       dx_object);

static inline dx_asset_material* DX_ASSET_MATERIAL(void* p) {
    return (dx_asset_material*)p;
}

struct dx_asset_material {
  dx_object _parent;
  /// @brief A pointer to the ADL name of this material.
  dx_string* name;
  /// @brief The ambient color of this material.
  dx_asset_reference* ambient_color;
  /// @brief Pointer to the ambient texture of this material or the null pointer.
  /// @default The null pointer.
  dx_asset_reference* ambient_texture_reference;
  /// @brief A pointer to the viewer controller or a null pointer.
  dx_asset_material_controller* controller;
};

static inline dx_asset_material_dispatch* DX_ASSET_MATERIAL_DISPATCH(void* p) {
  return (dx_asset_material_dispatch*)p;
}

struct dx_asset_material_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this dx_asset_material object with default values.
/// @param name A pointer to the ADL name of this material.
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @constructor{dx_asset_material}
dx_result dx_asset_material_construct(dx_asset_material* SELF, dx_string* name);

/// @create-operator{dx_asset_material}
/// @detail The object is created with default values.
/// @param name A pointer to the ADL name of this material.
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_asset_material_create(dx_asset_material** RETURN, dx_string* name);

/// @brief Set the ambient color.
/// @param ambient_color A pointer to color.
/// @error #DX_ERROR_INVALID_ARGUMENT @a value is a null pointer
/// @method{dx_asset_material}
dx_result dx_asset_material_set_ambient_color(dx_asset_material* SELF, dx_assets_color_rgb_n8* ambient_color);

/// @brief Set the ambient texture.
/// @param ambient_texture_reference A pointer to the ambient_texture_reference or the null pointer.
/// @method{dx_asset_material}
dx_result dx_asset_material_set_ambient_texture(dx_asset_material* SELF, dx_asset_reference* ambient_texture_reference);

#endif // DX_ASSET_MATERIAL_H_INCLUDED
