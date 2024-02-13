// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_ASSETS_TEXTURE_H_INCLUDED)
#define CORE_ASSETS_TEXTURE_H_INCLUDED

#include "Core/Assets/Image.h"
#include "Core/Assets/Ref.h"

/* http://localhost/assets#core-assets-texture */
Core_declareObjectType("Core.Assets.Texture",
                       Core_Assets_Texture,
                       Core_Assets_Def);

static inline Core_Assets_Texture* CORE_ASSETS_TEXTURE(void* p) {
  return (Core_Assets_Texture*)p;
}

struct Core_Assets_Texture {
  Core_Assets_Def _parent;
  /// @brief A pointer to the ADL name of this texture.
  Core_String* name;
  /// @brief The reference to the image of this texture.
  Core_Assets_Ref* image_reference;
};

static inline Core_Assets_Texture_Dispatch* CORE_ASSETS_TEXTURE_DISPATCH(void* p) {
  return (Core_Assets_Texture_Dispatch*)p;
}

struct Core_Assets_Texture_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @param name A pointer to the ADL name of the texture.
/// @param image_reference The image reference of this texture.
/// @constructor{Core_Assets_Texture}
Core_Result Core_Assets_Texture_construct(Core_Assets_Texture* SELF, Core_String* name, Core_Assets_Ref* image_reference);

Core_Result Core_Assets_Texture_create(Core_Assets_Texture** RETURN, Core_String* name, Core_Assets_Ref* image_reference);

#endif // CORE_ASSETS_TEXTURE_H_INCLUDED
