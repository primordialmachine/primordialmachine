#if !defined(DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Fills an image with the a checkerboard pattern.
DX_DECLARE_OBJECT_TYPE("dx.asset.image_operations.checkerboard_pattern_fill",
                       dx_asset_image_operations_checkerboard_pattern_fill,
                       dx_asset_image_operation)

static inline dx_asset_image_operations_checkerboard_pattern_fill* DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(void* p) {
  return (dx_asset_image_operations_checkerboard_pattern_fill*)p;
}

struct dx_asset_image_operations_checkerboard_pattern_fill {
  dx_asset_image_operation _parent;
  
  dx_size number_of_checkers_horizontal;
  dx_size number_of_checkers_vertical;

  dx_size checker_size_horizontal;
  dx_size checker_size_vertical;

  dx_asset_color_rgb_n8* first_checker_color;
  dx_asset_color_rgb_n8* second_checker_color;
};

struct dx_asset_image_operations_checkerboard_pattern_fill_dispatch {
  dx_asset_image_operation_dispatch _parent;
};

/// @brief Construct this dx_asset_image_operations_checkerboard_pattern_fill object with default values.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_construct(dx_asset_image_operations_checkerboard_pattern_fill* self);

dx_asset_image_operations_checkerboard_pattern_fill* dx_asset_image_operations_checkerboard_pattern_fill_create();



/// @brief Set the number of checkers along the horizontal axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_horizontal(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_size number_of_checkers_horizontal);

/// @brief Get the number of checkers along the horizontal axis.
/// @param RETURN A pointer to a <code>dx_size</code> variable receiving the number of checkers along the vertical axis.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_number_of_checkers_horizontal(dx_size* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF);



/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_vertical(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_size number_of_checkers_vertical);

/// @brief Get the number of checkers along the vertical axis.
/// @param RETURN A pointer to a <code>dx_size</code> variable receiving the number of checkers along the vertical axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_number_of_checkers_vertical(dx_size* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the size of a checker along the horizontal axis.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param checker_size_horizontal The size of a checker along the horizontal axis.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_horizontal(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_size checker_size_horizontal);

/// @brief Get the size of a checker along the horizontal axis.
/// @param RETURN A pointer to the dx_size variable receiving the size of a checker along the horizontal axis.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_checker_size_horizontal(dx_size* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the size of a checker along the vertical axis.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param checker_size_vertical The size of a checker along the vertical axis.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_vertical(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_size checker_size_vertical);

/// @brief Get the size of a checker along the vertical axis.
/// @param RETURN A pointer to the dx_size variable receiving the size of a checker along the vertical axis.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_checker_size_vertical(dx_size* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF);



/// @brief Set the first checker color.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param first_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_asset_color_rgb_n8* first_checker_color);

/// @brief Get the first checker color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_first_checker_color(DX_RGB_N8* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF);




/// @brief Set the second checker color.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param second_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_asset_color_rgb_n8* second_checker_color);

/// @brief Get the first checker color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param SELF A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-runtime-calling-convention
dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_second_checker_color(DX_RGB_N8* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF);

#endif // DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED
