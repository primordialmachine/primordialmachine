#if !defined(DX_ASSETS_MATERIAL_H_INCLUDED)
#define DX_ASSETS_MATERIAL_H_INCLUDED

#include "Core/Assets/Ref.h"
typedef struct Core_Assets_ColorRgbN8 Core_Assets_ColorRgbN8;
typedef struct dx_assets_material_controller dx_assets_material_controller;

/// @brief A material asset.
Core_declareObjectType("dx.assets.material",
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
  Core_Assets_Ref* ambient_color;
  /// @brief Pointer to the ambient texture of this material or the null pointer.
  /// @default The null pointer.
  Core_Assets_Ref* ambient_texture_reference;
  /// @brief A pointer to the viewer controller or a null pointer.
  dx_assets_material_controller* controller;
};

static inline dx_assets_material_Dispatch* DX_ASSETS_MATERIAL_DISPATCH(void* p) {
  return (dx_assets_material_Dispatch*)p;
}

struct dx_assets_material_Dispatch {
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
Core_Result dx_assets_material_set_ambient_color(dx_assets_material* SELF, Core_Assets_ColorRgbN8* ambient_color);

/// @brief Set the ambient texture.
/// @param ambient_texture_reference A pointer to the ambient_texture_reference or the null pointer.
/// @method{dx_assets_material}
Core_Result dx_assets_material_set_ambient_texture(dx_assets_material* SELF, Core_Assets_Ref* ambient_texture_reference);

#endif // DX_ASSETS_MATERIAL_H_INCLUDED
