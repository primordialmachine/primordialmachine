/// @file dx/core/visuals/pixel_format.h
/// @brief Enumeration of pixel formats.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_VISUALS_PIXEL_FORMAT_H_INCLUDED)
#define DX_CORE_VISUALS_PIXEL_FORMAT_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Visuals
/// @brief Enumeration of pixel formats.
/// @remarks The enumeration elements are listed in alphabetical order.
DX_DECLARE_ENUMERATION_TYPE("dx.pixel_format",
                            dx_pixel_format);

enum dx_pixel_format {

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the alpha component.
  /// The second value denotes the intensity of the blue component,
  /// the third value denotes the intensity of the green component, and
  /// the fourth value denotes the intensity of the red component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  dx_pixel_format_an8_bn8_gn8_rn8,

  /// @brief Two dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the alpha component.
  /// The second value denotes the intensity of the luminance component.
  /// A component value of 0 indicates the least intensity.
  /// A component value of 255 indicates the greatest intensity.
  dx_pixel_format_an8_ln8,

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the alpha component.
  /// The second value denotes the intensity of the blue component,
  /// the third value denotes the intensity of the green component, and
  /// the fourth value denotes the intensity of the red component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  dx_pixel_format_an8_rn8_gn8_bn8,

  /// @brief Three dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the blue component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the red component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  dx_pixel_format_bn8_gn8_rn8,

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the blue component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the red component.
  /// The fourth value denotes the intensity of the alpha component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  dx_pixel_format_bn8_gn8_rn8_an8,

  /// @brief One dx_n8 value.
  /// The value denotes the intensity of the luminance component.
  /// A component value of 0 indicates the least intensity.
  /// A component value of 255 indicates the greatest intensity. 
  dx_pixel_format_ln8,

  /// @brief Two dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the luminance component.
  /// The second value denotes the intensity of the alpha component.
  /// A component value of 0 indicates the least intensity.
  /// A component value of 255 indicates the greatest intensity. 
  dx_pixel_format_ln8_an8,

  /// @brief Three dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the red component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the blue component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  dx_pixel_format_rn8_gn8_bn8,

  /// @brief Four dx_n8 values layed out consecutively in memory.
  /// The first value denotes the intensity of the red component,
  /// the second value denotes the intensity of the green component, and
  /// the third value denotes the intensity of the blue component.
  /// The fourth value denotes the intensity of the alpha component.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  dx_pixel_format_rn8_gn8_bn8_an8,

};

/// @ingroup Core_Visuals
/// @brief Get the number of Bytes per pixel of this pixel format.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param self This pixel format.
/// @success <code>*RETURN</code> was assigned the Bytes per pixel of this pixel format.
/// @method-call
dx_result dx_pixel_format_get_number_of_bytes_per_pixel(dx_size* RETURN, dx_pixel_format SELF);

#endif // DX_CORE_VISUALS_PIXEL_FORMAT_H_INCLUDED
