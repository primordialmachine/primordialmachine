#if !defined(DX_ASSETS_TEXTURE_H_INCLUDED)
#define DX_ASSETS_TEXTURE_H_INCLUDED

#include "Core/Assets/Image.h"
#include "Core/Assets/Ref.h"

/// @brief A texture asset.
Core_declareObjectType("dx.assets.texture",
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
  Core_Assets_Ref* image_reference;
};

static inline dx_assets_texture_Dispatch* DX_ASSETS_TEXTURE_DISPATCH(void* p) {
  return (dx_assets_texture_Dispatch*)p;
}

struct dx_assets_texture_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @param name A pointer to the ADL name of the texture.
/// @param image_reference The image reference of this texture.
/// @constructor{dx_assets_texture}
Core_Result dx_assets_texture_construct(dx_assets_texture* SELF, Core_String* name, Core_Assets_Ref* image_reference);

Core_Result dx_assets_texture_create(dx_assets_texture** RETURN, Core_String* name, Core_Assets_Ref* image_reference);

#endif // DX_ASSETS_TEXTURE_H_INCLUDED
