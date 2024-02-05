#if !defined(DX_VAL_MATERIAL_H_INCLUDED)
#define DX_VAL_MATERIAL_H_INCLUDED

#include "dx/val/context.h"
#include "dx/assets.h"

/// @brief The backend-representation of a material.
/// A dx.val.material usually represents a dx.asset.material.
Core_declareObjectType("dx.val.material",
                       dx_val_material,
                       Core_Object);

static inline dx_val_material* DX_VAL_MATERIAL(void* p) {
  return (dx_val_material*)p;
}

struct dx_val_material {
  Core_Object _parent;
  dx_val_context* context;
  /// @brief A pointer to the underlaying material asset.
  dx_assets_material* material_asset;
  /// @brief The ambient color as specified by dx_assets_material.ambient_color.
  Core_InlineRgbaR32 ambient_color;
  /// @brief Pointer to the ambient texture or the null pointer as specified by dx_assets_material.ambient_texture.
  dx_val_texture* ambient_texture;
};

static inline dx_val_material_Dispatch* DX_VAL_MATERIAL_DISPATCH(void* p) {
  return (dx_val_material_Dispatch*)p;
}

struct dx_val_material_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_val_material_construct(dx_val_material* SELF, dx_val_context* context, dx_assets_material* material_asset);

Core_Result dx_val_material_create(dx_val_material** RETURN, dx_val_context* context, dx_assets_material* material_asset);

#endif // DX_VAL_MATERIAL_H_INCLUDED
