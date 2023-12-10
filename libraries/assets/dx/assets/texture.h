#if !defined(DX_ASSETS_TEXTURE_H_INCLUDED)
#define DX_ASSETS_TEXTURE_H_INCLUDED

#include "dx/assets/image.h"
#include "dx/assets/reference.h"

/// @brief A texture asset.
DX_DECLARE_OBJECT_TYPE("dx.assets.texture",
                       dx_assets_texture,
                       Core_Object);

static inline dx_assets_texture* DX_ASSETS_TEXTURE(void* p) {
  return (dx_assets_texture*)p;
}

struct dx_assets_texture {
  Core_Object _parent;
  /// @brief A pointer to the ADL name of this texture.
  Core_String* name;
  /// @brief The reference to the image of this texture.
  dx_asset_reference* image_reference;
};

static inline dx_assets_texture_dispatch* DX_ASSETS_TEXTURE_DISPATCH(void* p) {
  return (dx_assets_texture_dispatch*)p;
}

struct dx_assets_texture_dispatch {
  Core_Object_Dispatch _parent;
};

/// @param name A pointer to the ADL name of the texture.
/// @param image_reference The image reference of this texture.
/// @constructor{dx_assets_texture}
Core_Result dx_assets_texture_construct(dx_assets_texture* SELF, Core_String* name, dx_asset_reference* image_reference);

Core_Result dx_assets_texture_create(dx_assets_texture** RETURN, Core_String* name, dx_asset_reference* image_reference);

#endif // DX_ASSETS_TEXTURE_H_INCLUDED
