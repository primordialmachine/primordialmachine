#if !defined(DX_ASSETS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED)
#define DX_ASSETS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_assets_image_operation
/// @brief An image operation.
/// Fills an image with a color.
DX_DECLARE_OBJECT_TYPE("dx.assets.image_operations.color_fill",
                       dx_assets_image_operations_color_fill,
                       dx_assets_image_operation);

static inline dx_assets_image_operations_color_fill* DX_ASSETS_IMAGE_OPERATIONS_COLOR_FILL(void* p) {
  return (dx_assets_image_operations_color_fill*)p;
}

struct dx_assets_image_operations_color_fill {
  dx_assets_image_operation _parent;
  /// @brief
  /// The reference to the color.
  /// Resolves to a dx_assets_color_rgb_n8.
  dx_asset_reference* color;
};

static inline dx_assets_image_operations_color_fill_dispatch* DX_ASSETS_IMAGE_OPERATIONS_COLOR_FILL_DISPATCH(void* p) {
  return (dx_assets_image_operations_color_fill_dispatch*)p;
}

struct dx_assets_image_operations_color_fill_dispatch {
  dx_assets_image_operation_dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{dx_assets_image_operations_color_fill}
dx_result dx_assets_image_operations_color_fill_construct(dx_assets_image_operations_color_fill* SELF);

/// @create-operator{dx_assets_image_operations_color_fill}
dx_result dx_assets_image_operations_color_fill_create(dx_assets_image_operations_color_fill** RETURN);

/// @brief Set the color.
/// @param color A pointer to the color object.
/// @method{dx_assets_image_operations_color_fill}
dx_result dx_assets_image_operations_color_fill_set_color(dx_assets_image_operations_color_fill* SELF, dx_assets_color_rgb_n8* color);

/// @brief Get the color.
/// @param RETURN A pointer to the DX_RGB_U8 color.
/// @success <code>*RETURN</code> was assigened the values.
/// @method{dx_assets_image_operations_color_fill}
dx_result dx_assets_image_operations_color_fill_get_color(DX_RGB_N8* RETURN, dx_assets_image_operations_color_fill* SELF);

#endif // DX_ASSETS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED
