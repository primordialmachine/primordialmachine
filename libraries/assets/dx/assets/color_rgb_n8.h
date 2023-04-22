#if !defined(DX_ASSET_COLOR_RGB_N8_H_INCLUDED)
#define DX_ASSET_COLOR_RGB_N8_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.asset.color_rgb_n8",
                       dx_asset_color_rgb_n8,
                       dx_object);

static inline dx_asset_color_rgb_n8* DX_ASSET_COLOR_RGB_N8(void* p) {
  return (dx_asset_color_rgb_n8*)p;
}

struct dx_asset_color_rgb_n8 {
  dx_object _parent;
  DX_RGB_N8 value;
};

struct dx_asset_color_rgb_n8_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_asset_color_rgb_n8_construct(dx_asset_color_rgb_n8* SELF, DX_RGB_N8 const* value);

dx_result dx_asset_color_rgb_n8_create(dx_asset_color_rgb_n8** RESULT, DX_RGB_N8 const* value);

/// @brief Get a pointer to the underlaying value.
/// @param SELF A pointer to this color.
/// @return A pointer to the underlaying value on success. The null pointer on failure.
DX_RGB_N8 const* dx_asset_color_rgb_n8_get_value(dx_asset_color_rgb_n8* SELF);

#endif // DX_ASSET_COLOR_RGB_N8_H_INCLUDED
