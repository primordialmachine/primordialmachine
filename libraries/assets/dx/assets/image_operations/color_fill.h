#if !defined(DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Fills an image with a color.
DX_DECLARE_OBJECT_TYPE("dx.asset.image_operations.color_fill",
                       dx_asset_image_operations_color_fill,
                       dx_asset_image_operation);

static inline dx_asset_image_operations_color_fill* DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(void* p) {
  return (dx_asset_image_operations_color_fill*)p;
}

struct dx_asset_image_operations_color_fill {
  dx_asset_image_operation _parent;
  /// @todo This must be a color reference.
  DX_RGB_N8 color;
};

struct dx_asset_image_operations_color_fill_dispatch {
  dx_asset_image_operation_dispatch _parent;
};

/// @brief Construct this dx_asset_image_operations_color_fill object with default values.
/// @param SELF A pointer to this dx_asset_image_operations_color_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_color_fill_construct(dx_asset_image_operations_color_fill* SELF);

dx_asset_image_operations_color_fill* dx_asset_image_operations_color_fill_create();

/// @brief Set the color.
/// @param SELF A pointer to this dx_asset_image_operations_color_fill object.
/// @param color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_color_fill_set_color(dx_asset_image_operations_color_fill* SELF, dx_asset_color_rgb_n8* color);

/// @brief Get the color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param SELF A pointer to this dx_asset_image_operations_color_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_color_fill_get_color(DX_RGB_N8* RETURN, dx_asset_image_operations_color_fill* SELF);

#endif // DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED
