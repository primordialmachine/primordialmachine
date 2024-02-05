#if !defined(DX_ASSETS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED)
#define DX_ASSETS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_assets_image_operation
/// @brief An image operation.
/// Fills an image with the a checkerboard pattern.
Core_declareObjectType("dx.assets.image_operations.checkerboard_pattern_fill",
                       dx_assets_image_operations_checkerboard_pattern_fill,
                       dx_assets_image_operation);

static inline dx_assets_image_operations_checkerboard_pattern_fill* DX_ASSETS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(void* p) {
  return (dx_assets_image_operations_checkerboard_pattern_fill*)p;
}

struct dx_assets_image_operations_checkerboard_pattern_fill {
  dx_assets_image_operation _parent;
  
  Core_Size number_of_checkers_horizontal;
  Core_Size number_of_checkers_vertical;

  Core_Size checker_size_horizontal;
  Core_Size checker_size_vertical;

  /// @brief
  /// The reference to the first checker color.
  /// Resolves to a dx_assets_color_rgb_n8.
  dx_asset_reference* first_checker_color;

  /// @brief
  /// The reference to the second checker color.
  /// Resolves to a dx_assets_color_rgb_n8.
  dx_asset_reference* second_checker_color;
};

static inline dx_assets_image_operations_checkerboard_pattern_fill_Dispatch* DX_ASSETS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_DISPATCH(void* p) {
  return (dx_assets_image_operations_checkerboard_pattern_fill_Dispatch*)p;
}

struct dx_assets_image_operations_checkerboard_pattern_fill_Dispatch {
  dx_assets_image_operation_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_construct(dx_assets_image_operations_checkerboard_pattern_fill* SELF);

/// @create-operator{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_create(dx_assets_image_operations_checkerboard_pattern_fill** RETURN);



/// @brief Set the number of checkers along the horizontal axis.
/// @param number_of_checkers_horizontal The number of checkers along the horizontal axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_number_of_checkers_horizontal(dx_assets_image_operations_checkerboard_pattern_fill* SELF, Core_Size number_of_checkers_horizontal);

/// @brief Get the number of checkers along the horizontal axis.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the number of checkers along the horizontal axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_number_of_checkers_horizontal(Core_Size* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the number of checkers along the vertical axis.
/// @param number_of_checkers_vertical The number of checkers along the vertical axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_number_of_checkers_vertical(dx_assets_image_operations_checkerboard_pattern_fill* SELF, Core_Size number_of_checkers_vertical);

/// @brief Get the number of checkers along the vertical axis.
/// @param RETURN A pointer to a <code>Core_Size</code>.
/// @success <code>*RETURN</code> was assigned the number of checkers along the vertical axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_number_of_checkers_vertical(Core_Size* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the size of a checker along the horizontal axis.
/// @param checker_size_horizontal The size of a checker along the horizontal axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_checker_size_horizontal(dx_assets_image_operations_checkerboard_pattern_fill* SELF, Core_Size checker_size_horizontal);

/// @brief Get the size of a checker along the horizontal axis.
/// @param RETURN A pointer to the Core_Size variable.
/// @success <code>*RETURN</code> was assigned the size of a checker along the horizontal axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_checker_size_horizontal(Core_Size* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the size of a checker along the vertical axis.
/// @param checker_size_vertical The size of a checker along the vertical axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_checker_size_vertical(dx_assets_image_operations_checkerboard_pattern_fill* SELF, Core_Size checker_size_vertical);

/// @brief Get the size of a checker along the vertical axis.
/// @param RETURN A pointer to the Core_Size variable.
/// @success <code>*RETURN</code> was assigned the size of a checker along the vertical axis.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_checker_size_vertical(Core_Size* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the first checker color.
/// @param first_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_first_checker_color(dx_assets_image_operations_checkerboard_pattern_fill* SELF, dx_assets_color_rgb_n8* first_checker_color);

/// @brief Get the first checker color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_first_checker_color(Core_InlineRgbN8* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the second checker color.
/// @param second_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_second_checker_color(dx_assets_image_operations_checkerboard_pattern_fill* SELF, dx_assets_color_rgb_n8* second_checker_color);

/// @brief Get the first checker color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @method{dx_assets_image_operations_checkerboard_pattern_fill}
Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_second_checker_color(Core_InlineRgbN8* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF);

#endif // DX_ASSETS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED
