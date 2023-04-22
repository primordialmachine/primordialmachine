/// @file dx/core/visuals.h
/// @brief Core functionality with a focus on visuals.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_VISUALS_H_INCLUDED)
#define DX_CORE_VISUALS_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Visuals
/// @brief Formats of vertices.
typedef enum DX_VERTEX_FORMAT {

  /// @brief A vertex of the format position.
  DX_VERTEX_FORMAT_POSITION_XYZ = 1,

  /// @brief A vertex of the format color.
  DX_VERTEX_FORMAT_AMBIENT_RGBA = 2,

  /// @brief A vertex of the format texture.
  DX_VERTEX_FORMAT_AMBIENT_UV = 4,

  /// @brief A vertex of the format position first, color second.
  DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA = (DX_VERTEX_FORMAT_POSITION_XYZ | DX_VERTEX_FORMAT_AMBIENT_RGBA),

  /// @brief A vertex of the format position first, texture second.
  DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV = (DX_VERTEX_FORMAT_POSITION_XYZ | DX_VERTEX_FORMAT_AMBIENT_UV),

  /// @brief A vertex of the format position first, color second, texture third.
  DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA_AMBIENT_UV = (DX_VERTEX_FORMAT_POSITION_XYZ | DX_VERTEX_FORMAT_AMBIENT_RGBA | DX_VERTEX_FORMAT_AMBIENT_UV),

} DX_VERTEX_FORMAT;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Visuals
/// @brief Formats of pixels.
/// @remarks The enumeration elements are listed in alphabetical order.
typedef enum DX_PIXEL_FORMAT {

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the alpha component.
  /// The second value denotes the intensity of the blue component,
  /// the third value denotes the intensity of the green component, and
  /// the fourth value denotes the intensity of the red component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_AN8_BN8_GN8_RN8,

  /// @brief Two dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the alpha component.
  /// The second value denotes the intensity of the luminance component.
  /// A component value of 0 indicates the least intensity.
  /// A component value of 255 indicates the greatest intensity.
  DX_PIXEL_FORMAT_AN8_LN8,

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the alpha component.
  /// The second value denotes the intensity of the blue component,
  /// the third value denotes the intensity of the green component, and
  /// the fourth value denotes the intensity of the red component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_AN8_RN8_GN8_BN8,

  /// @brief Three dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the blue component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the red component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_BN8_GN8_RN8,

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the blue component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the red component.
  /// The fourth value denotes the intensity of the alpha component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_BN8_GN8_RN8_AN8,

  /// @brief One dx_n8 value.
  /// The value denotes the intensity of the luminance component.
  /// A component value of 0 indicates the least intensity.
  /// A component value of 255 indicates the greatest intensity. 
  DX_PIXEL_FORMAT_LN8,

  /// @brief Two dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the luminance component.
  /// The second value denotes the intensity of the alpha component.
  /// A component value of 0 indicates the least intensity.
  /// A component value of 255 indicates the greatest intensity. 
  DX_PIXEL_FORMAT_LN8_AN8,

  /// @brief Three dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the red component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the blue component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_RN8_GN8_BN8,

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the red component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the blue component.
  /// The fourth value denotes the intensity of the alpha component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_RN8_GN8_BN8_AN8,

} DX_PIXEL_FORMAT;

/// @ingroup Core_Visuals
/// @brief Get the number of Bytes per pixel of this pixel format.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param self This pixel format.
/// @success <code>*RETURN</code> was assigned the Bytes per pixel of this pixel format.
/// @default-return
/// @default-failure
static inline dx_result dx_pixel_format_get_number_of_bytes_per_pixel(dx_size* RETURN, DX_PIXEL_FORMAT self) {
  switch (self) {
    case DX_PIXEL_FORMAT_AN8_LN8: {
      *RETURN = 2;
      return DX_SUCCESS;
    } break;
    case DX_PIXEL_FORMAT_BN8_GN8_RN8: {
      *RETURN = 3;
      return DX_SUCCESS;
    } break;
    case DX_PIXEL_FORMAT_BN8_GN8_RN8_AN8: {
      *RETURN = 4;
      return DX_SUCCESS;
    } break;
    case DX_PIXEL_FORMAT_LN8: {
      *RETURN = 1;
      return DX_SUCCESS;
    } break;
    case DX_PIXEL_FORMAT_LN8_AN8: {
      *RETURN = 2;
      return DX_SUCCESS;
    } break;
    case DX_PIXEL_FORMAT_RN8_GN8_BN8: {
      *RETURN = 3;
      return DX_SUCCESS;
    } break;
    case DX_PIXEL_FORMAT_RN8_GN8_BN8_AN8: {
      *RETURN = 4;
      return DX_SUCCESS;
    } break;
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    } break;
  };
}

/// @ingroup Core_Visuals
/// @brief Enumeration of texture minification filters.
typedef enum DX_TEXTURE_MINIFICATION_FILTER {

  /// GL_TEXTURE_MIN_FILTER with GL_NEAREST in OpenGL
  DX_TEXTURE_MINIFICATION_FILTER_NEAREST,

  /// GL_TEXTURE_MIN_FILTER with GL_LINEAR in OpenGL
  DX_TEXTURE_MINIFICATION_FILTER_LINEAR,

} DX_TEXTURE_MINIFICATION_FILTER;

/// @ingroup Core_Visuals
/// @brief Enumeration of texture magnification filters.
typedef enum DX_TEXTURE_MAGNIFICATION_FILTER {

  /// GL_TEXTURE_MAG_FILTER with GL_NEAREST in OpenGL
  DX_TEXTURE_MAGNIFICATION_FILTER_NEAREST,

  /// GL_TEXTURE_MAG_FILTER with GL_LINEAR in OpenGL
  DX_TEXTURE_MAGNIFICATION_FILTER_LINEAR,

} DX_TEXTURE_MAGNIFICATION_FILTER;

/// @ingroup Core_Visuals
/// @brief Enumeration of texture addressing modes.
typedef enum DX_TEXTURE_ADDRESS_MODE {
  /// GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_CLAMP_TO_BORDER in OpenGL
  /// D3D12_TEXTURE_ADDRESS_MODE_BORDER in Direct3D 12
  DX_TEXTURE_ADDRESS_MODE_CLAMP_TO_BORDER,

  /// GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_REPEAT in OpenGL
  /// D3D12_TEXTURE_ADDRESS_MODE_WRAP in Direct3D 12
  DX_TEXTURE_ADDRESS_MODE_REPEAT,
  
  /// GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_MIRRORED_REPEAT in OpenGL
  /// D3D12_TEXTURE_ADDRESS_MODE_MIRROR in Direct3D 12
  DX_TEXTURE_ADDRESS_MODE_MIRRORED_REPEAT,

} DX_TEXTURE_ADDRESS_MODE;

/// @ingroup Core_Visuals
/// @brief Enumeration of depth test functions.
typedef enum DX_DEPTH_TEST_FUNCTION {

  /// @brief The incoming fragment always passes.
  DX_DEPTH_TEST_FUNCTION_ALWAYS,

  /// @brief The incoming fragment never passes.
  DX_DEPTH_TEST_FUNCTION_NEVER,

  /// @brief The incoming fragment passes if its depth value is equal to its corresponding depth value in the depth buffer.
  DX_DEPTH_TEST_FUNCTION_EQUAL,

  /// @brief The incoming fragement passes if its depth value is not equal to its corresponding depth value in the depth buffer.
  DX_DEPTH_TEST_FUNCTION_NOT_EQUAL,

  /// @brief The incoming fragment passes if its depth value is less than its corresponding depth value in the depth buffer.
  DX_DEPTH_TEST_FUNCTION_LESS_THAN,

  /// @brief The incoming fragment passes if its depth value is less than or equal to its corresponding depth value in the depth buffer.
  DX_DEPTH_TEST_FUNCTION_LESS_THAN_OR_EQUAL_TO,

  /// @brief The incoming fragment passes if its depth value is greater than its corresponding depth value in the depth buffer.
  DX_DEPTH_TEST_FUNCTION_GREATER_THAN,

  /// @brief The incoming fragment passes if its depth value is greater than or equal to its corresponding depth value in the depth buffer.
  DX_DEPTH_TEST_FUNCTION_GREATER_THAN_OR_EQUAL_TO,

} DX_DEPTH_TEST_FUNCTION;

/// @ingroup Core_Visuals
/// @brief Enumeration of culling modes.
typedef enum DX_CULL_MODE {
  
  /// @brief Neither front-facing nor back-facing geometry is culled.
  DX_CULL_MODE_NONE,
  
  /// @brief Front-facing geometry is culled.
  DX_CULL_MODE_FRONT,

  /// @brief Back-facing geometry is culled.
  DX_CULL_MODE_BACK,

  /// @brief Both front-facing and back-facing geometry is culled.
  DX_CULL_MODE_FRONT_AND_BACK,

} DX_CULL_MODE;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_VISUALS_H_INCLUDED
