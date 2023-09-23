#if !defined(DX_VAL_TEXTURE_H_INCLUDED)
#define DX_VAL_TEXTURE_H_INCLUDED

#include "dx/val/context.h"
#include "dx/assets.h"

/// @brief The base of all textures.
DX_DECLARE_OBJECT_TYPE("dx.val.texture",
                       dx_val_texture,
                       dx_val_object);
  
static inline dx_val_texture* DX_VAL_TEXTURE(void* p) {
  return (dx_val_texture*)p;
}

struct dx_val_texture {
  dx_object _parent;
  dx_val_context* context;
};

static inline dx_val_texture_dispatch* DX_VAL_TEXTURE_DISPATCH(void* p) {
  return (dx_val_texture_dispatch*)p;
}

struct dx_val_texture_dispatch {
  dx_object_dispatch _parent;
  dx_result(*set_data)(dx_val_texture*, dx_asset_texture*);
  dx_result(*set_texture_address_mode_u)(dx_val_texture*, DX_TEXTURE_ADDRESS_MODE);
  dx_result(*get_texture_address_mode_u)(DX_TEXTURE_ADDRESS_MODE*, dx_val_texture*);
  dx_result(*set_texture_address_mode_v)(dx_val_texture*, DX_TEXTURE_ADDRESS_MODE);
  dx_result(*get_texture_address_mode_v)(DX_TEXTURE_ADDRESS_MODE*, dx_val_texture*);
  dx_result(*set_texture_border_color)(dx_val_texture*, DX_VEC4 const*);
  dx_result(*get_texture_border_color)(DX_VEC4*, dx_val_texture*);
  dx_result(*set_texture_minification_filter)(dx_val_texture*, DX_TEXTURE_MINIFICATION_FILTER);
  dx_result(*get_texture_minification_filter)(DX_TEXTURE_MINIFICATION_FILTER*, dx_val_texture*);
  dx_result(*set_texture_magnification_filter)(dx_val_texture*, DX_TEXTURE_MAGNIFICATION_FILTER);
  dx_result(*get_texture_magnification_filter)(DX_TEXTURE_MAGNIFICATION_FILTER*, dx_val_texture*);
};

dx_result dx_val_texture_construct(dx_val_texture* SELF, dx_val_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the data of this texture.
/// @param SELF A pointer to this texture.
/// @param texture A pointer to the texture asset.
/// @default-failure
/// @default-return
static inline dx_result dx_val_texture_set_data(dx_val_texture* SELF, dx_asset_texture* texture) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, set_data, SELF, texture);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture address mode for the u coordinates.
/// @param SELF A pointer to this texture.
/// @param texture_address_mode_u The texture address mode.
/// @default-failure
/// @default-return
static inline dx_result dx_val_texture_set_texture_address_mode_u(dx_val_texture* SELF, DX_TEXTURE_ADDRESS_MODE texture_address_mode_u) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, set_texture_address_mode_u, SELF, texture_address_mode_u);
}

/// @brief Get the texture address mode for the u coordinates.
/// @param RETURN A pointer to a <code>DX_TEXTURE_ADDRESS_MODE</code> variable.
/// @param SELF A pointer to this texture.
/// @success <code>*RETURN</code> was assigned texture address mode for the u coordinates.
/// @method-call
static inline dx_result dx_val_texture_get_texture_address_mode_u(DX_TEXTURE_ADDRESS_MODE* RETURN, dx_val_texture* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, get_texture_address_mode_u, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture address mode for the v coordinates.
/// @param SELF A pointer to this texture.
/// @param texture_address_mode_v The texture address mode.
/// @method-call
static inline dx_result dx_val_texture_set_texture_address_mode_v(dx_val_texture* SELF, DX_TEXTURE_ADDRESS_MODE texture_address_mode_v) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, set_texture_address_mode_v, SELF, texture_address_mode_v);
}

/// @brief Get the texture address mode for the v coordinates.
/// @param RETURN A pointer to a <code>DX_TEXTURE_ADDRESS_MODE</code> variable.
/// @param SELF A pointer to this texture.
/// @success <code>*RETURN</code> was assigned texture address mode for the v coordinates.
/// @method-call
static inline dx_result dx_val_texture_get_texture_address_mode_v(DX_TEXTURE_ADDRESS_MODE* RETURN, dx_val_texture* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, get_texture_address_mode_v, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture border color.
/// @param SELF A pointer to this texture.
/// @param texture_border_color A pointer to the texture border color.
/// @method-call
static inline dx_result dx_val_texture_set_texture_border_color(dx_val_texture* SELF, DX_VEC4 const* texture_border_color) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, set_texture_border_color, SELF, texture_border_color);
}

/// @brief Get the texture border color.
/// @param RETURN A pointer to a <code>DX_VEC4</code> variable.
/// @param SELF A pointer to this texture.
/// @success <code>*RETURN</code> was assigned the texture border color.
/// @method-call
static inline dx_result dx_val_texture_get_texture_border_color(DX_VEC4* RETURN, dx_val_texture* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, get_texture_border_color, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture minification filter.
/// @param SELF A pointer to this texture.
/// @param texture_minification_filter The texture minification filter.
/// @default-failure
/// @default-return
static inline dx_result dx_val_texture_set_texture_minification_filter(dx_val_texture* SELF, DX_TEXTURE_MINIFICATION_FILTER texture_minification_filter) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, set_texture_minification_filter, SELF, texture_minification_filter);
}

/// @brief Get the texture minification filter.
/// @param RETURN A pointer to a <code>DX_TEXTURE_MINIFICATION_FILTER</code> variable.
/// @param SELF A pointer to this texture.
/// @success <code>*RETURN</code> was assigned the texture minification filter.
/// @default-failure
/// @default-return
static inline dx_result dx_val_texture_get_texture_minification_filter(DX_TEXTURE_MINIFICATION_FILTER* RETURN, dx_val_texture* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, get_texture_minification_filter, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the texture magnification filter.
/// @param SELF A pointer to this texture.
/// @param texture_magnification_filter The texture magnification filter.
/// @default-failure
/// @default-return
static inline dx_result dx_val_texture_set_texture_magnification_filter(dx_val_texture* SELF, DX_TEXTURE_MAGNIFICATION_FILTER texture_magnification_filter) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, set_texture_magnification_filter, SELF, texture_magnification_filter);
}

/// @brief Get the texture magnification filter.
/// @param RETURN A pointer to a <code>DX_TEXTURE_MAGNIFICATION_FILTER</code> variable.
/// @param SELF A pointer to this texture.
/// @success <code>*RETURN</code> was assigned the texture magnification filter.
/// @default-failure
/// @default-return
static inline dx_result dx_val_texture_get_texture_magnification_filter(DX_TEXTURE_MAGNIFICATION_FILTER* RETURN, dx_val_texture* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_texture, get_texture_magnification_filter, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_VAL_TEXTURE_H_INCLUDED
