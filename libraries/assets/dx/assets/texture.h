#if !defined(DX_ASSETS_TEXTURE_H_INCLUDED)
#define DX_ASSETS_TEXTURE_H_INCLUDED

#include "dx/assets/image.h"
#include "dx/assets/reference.h"

/// @brief A texture asset.
DX_DECLARE_OBJECT_TYPE("dx.assets.texture",
                       dx_assets_texture,
                       dx_object);

static inline dx_assets_texture* DX_ASSETS_TEXTURE(void* p) {
  return (dx_assets_texture*)p;
}

struct dx_assets_texture {
  dx_object _parent;
  /// @brief A pointer to the ADL name of this texture.
  dx_string* name;
  /// @brief The reference to the image of this texture.
  dx_asset_reference* image_reference;
};

static inline dx_assets_texture_dispatch* DX_ASSETS_TEXTURE_DISPATCH(void* p) {
  return (dx_assets_texture_dispatch*)p;
}

struct dx_assets_texture_dispatch {
  dx_object _parent;
};

/// @param name A pointer to the ADL name of the texture.
/// @param image_reference The image reference of this texture.
/// @constructor{dx_assets_texture}
dx_result dx_assets_texture_construct(dx_assets_texture* SELF, dx_string* name, dx_asset_reference* image_reference);

dx_result dx_assets_texture_create(dx_assets_texture** RETURN, dx_string* name, dx_asset_reference* image_reference);

#endif // DX_ASSETS_TEXTURE_H_INCLUDED
