#if !defined(CORE_VISUALS_TEXTURE_H_INCLUDED)
#define CORE_VISUALS_TEXTURE_H_INCLUDED

#include "dx/val/context.h"
#include "dx/assets.h"

/// @brief The base of all textures.
Core_declareObjectType("Core.Visuals.Texture",
                       Core_Visuals_Texture,
                       Core_Object);
  
static inline Core_Visuals_Texture* CORE_VISUALS_TEXTURE(void* p) {
  return (Core_Visuals_Texture*)p;
}

struct Core_Visuals_Texture {
  Core_Object _parent;
  Core_Visuals_Context* context;
};

static inline Core_Visuals_Texture_Dispatch* CORE_VISUALS_TEXTURE_DISPATCH(void* p) {
  return (Core_Visuals_Texture_Dispatch*)p;
}

struct Core_Visuals_Texture_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*setData)(Core_Visuals_Texture*, Core_Assets_Texture*);
  Core_Result(*setAddressModeU)(Core_Visuals_Texture*, Core_TextureAddressMode);
  Core_Result(*getAddressModeU)(Core_TextureAddressMode*, Core_Visuals_Texture*);
  Core_Result(*setAddressModeV)(Core_Visuals_Texture*, Core_TextureAddressMode);
  Core_Result(*getAddressModeV)(Core_TextureAddressMode*, Core_Visuals_Texture*);
  Core_Result(*setBorderColor)(Core_Visuals_Texture*, Core_InlineRgbaR32 const*);
  Core_Result(*getBorderColor)(Core_InlineRgbaR32*, Core_Visuals_Texture*);
  Core_Result(*setMinificationFilter)(Core_Visuals_Texture*, Core_TextureFilter);
  Core_Result(*getMinificationFilter)(Core_TextureFilter*, Core_Visuals_Texture*);
  Core_Result(*setMagnificationFilter)(Core_Visuals_Texture*, Core_TextureFilter);
  Core_Result(*getMagnificationFilter)(Core_TextureFilter*, Core_Visuals_Texture*);
};

Core_Result Core_Visuals_Texture_construct(Core_Visuals_Texture* SELF, Core_Visuals_Context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the data of this texture.
/// @param texture A pointer to the texture asset.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_set_data(Core_Visuals_Texture* SELF, Core_Assets_Texture* texture) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, setData, SELF, texture);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture address mode for the u coordinates.
/// @param texture_address_mode_u The texture address mode.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_set_texture_address_mode_u(Core_Visuals_Texture* SELF, Core_TextureAddressMode texture_address_mode_u) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, setAddressModeU, SELF, texture_address_mode_u);
}

/// @brief Get the texture address mode for the u coordinates.
/// @param RETURN A pointer to a <code>Core_TextureAddressMode</code> variable.
/// @success <code>*RETURN</code> was assigned texture address mode for the u coordinates.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_get_texture_address_mode_u(Core_TextureAddressMode* RETURN, Core_Visuals_Texture* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, getAddressModeU, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture address mode for the v coordinates.
/// @param SELF A pointer to this texture.
/// @param texture_address_mode_v The texture address mode.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_set_texture_address_mode_v(Core_Visuals_Texture* SELF, Core_TextureAddressMode texture_address_mode_v) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, setAddressModeV, SELF, texture_address_mode_v);
}

/// @brief Get the texture address mode for the v coordinates.
/// @param RETURN A pointer to a <code>Core_TextureAddressMode</code> variable.
/// @success <code>*RETURN</code> was assigned texture address mode for the v coordinates.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_get_texture_address_mode_v(Core_TextureAddressMode* RETURN, Core_Visuals_Texture* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, getAddressModeV, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture border color.
/// @param SELF A pointer to this texture.
/// @param texture_border_color A pointer to the texture border color.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_set_texture_border_color(Core_Visuals_Texture* SELF, Core_InlineRgbaR32 const* texture_border_color) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, setBorderColor, SELF, texture_border_color);
}

/// @brief Get the texture border color.
/// @param RETURN A pointer to a <code>Core_InlineRgbaR32</code> variable.
/// @success <code>*RETURN</code> was assigned the texture border color.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_get_texture_border_color(Core_InlineRgbaR32* RETURN, Core_Visuals_Texture* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, getBorderColor, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture minification filter.
/// @param texture_minification_filter The texture minification filter.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_set_texture_minification_filter(Core_Visuals_Texture* SELF, Core_TextureFilter texture_minification_filter) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, setMinificationFilter, SELF, texture_minification_filter);
}

/// @brief Get the texture minification filter.
/// @param RETURN A pointer to a <code>Core_TextureFilter</code> variable.
/// @success <code>*RETURN</code> was assigned the texture minification filter.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_get_texture_minification_filter(Core_TextureFilter* RETURN, Core_Visuals_Texture* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, getMinificationFilter, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture magnification filter.
/// @param texture_magnification_filter The texture magnification filter.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_set_texture_magnification_filter(Core_Visuals_Texture* SELF, Core_TextureFilter texture_magnification_filter) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, setMagnificationFilter, SELF, texture_magnification_filter);
}

/// @brief Get the texture magnification filter.
/// @param RETURN A pointer to a <code>Core_TextureFilter</code> variable.
/// @success <code>*RETURN</code> was assigned the texture magnification filter.
/// @method{Core_Visuals_Texture}
static inline Core_Result Core_Visuals_Texture_get_texture_magnification_filter(Core_TextureFilter* RETURN, Core_Visuals_Texture* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Texture, getMagnificationFilter, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_VISUALS_TEXTURE_H_INCLUDED
