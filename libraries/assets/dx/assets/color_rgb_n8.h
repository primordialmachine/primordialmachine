#if !defined(DX_ASSET_COLOR_RGB_N8_H_INCLUDED)
#define DX_ASSET_COLOR_RGB_N8_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.assets.color_rgb_n8",
                       dx_assets_color_rgb_n8,
                       Core_Object);

static inline dx_assets_color_rgb_n8* DX_ASSETS_COLOR_RGB_N8(void* p) {
  return (dx_assets_color_rgb_n8*)p;
}

struct dx_assets_color_rgb_n8 {
  Core_Object _parent;
  DX_RGB_N8 value;
};

static inline dx_assets_color_rgb_n8_dispatch* DX_ASSETS_COLOR_RGB_N8_DISPATCH(void* p) {
  return (dx_assets_color_rgb_n8_dispatch*)p;
}

struct dx_assets_color_rgb_n8_dispatch {
  Core_Object_Dispatch _parent;
};

/// @detail The color is initialized with the specified values.
/// @param value A pointer to a <code>DX_RGB_N8</code> object from which the values are read.
/// @constructor{dx_assets_color_rgb_n8} 
Core_Result dx_assets_color_rgb_n8_construct(dx_assets_color_rgb_n8* SELF, DX_RGB_N8 const* value);

Core_Result dx_assets_color_rgb_n8_create(dx_assets_color_rgb_n8** RESULT, DX_RGB_N8 const* value);

/// @brief Get the values.
/// @param RETURN A pointer to a <code>DX_RGB_N8</code> object.
/// @success <code>*RETURN</code> was assigned the component values.
/// @constructor{dx_assets_color_rgb_n8}
Core_Result dx_assets_color_rgb_n8_get_value(DX_RGB_N8* RETURN, dx_assets_color_rgb_n8* SELF);

#endif // DX_ASSET_COLOR_RGB_N8_H_INCLUDED
