#if !defined(DX_ASSET_TEXTURE_H_INCLUDED)
#define DX_ASSET_TEXTURE_H_INCLUDED

#include "dx/assets/image.h"
#include "dx/assets/reference.h"

/// @brief A texture asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.texture",
                       dx_asset_texture,
                       dx_object)

static inline dx_asset_texture* DX_ASSET_TEXTURE(void* p) {
  return (dx_asset_texture*)p;
}

struct dx_asset_texture {
  dx_object _parent;
  /// @brief A pointer to the ADL name of this texture.
  dx_string* name;
  /// @brief The reference to the image of this texture.
  dx_asset_reference* image_reference;
};

struct dx_asset_texture_dispatch {
  dx_object _parent;
};

/// @brief Construct this dx_asset_texture object.
/// @param SELF A pointer to this dx_asset_material object.
/// @param name A pointer to the ADL name of the texture.
/// @param image_reference The image reference of this texture.
/// @default-return
/// @default-failure
dx_result dx_asset_texture_construct(dx_asset_texture* SELF, dx_string* name, dx_asset_reference* image_reference);

dx_result dx_asset_texture_create(dx_asset_texture** RETURN, dx_string* name, dx_asset_reference* image_reference);

#endif // DX_ASSET_TEXTURE_H_INCLUDED
