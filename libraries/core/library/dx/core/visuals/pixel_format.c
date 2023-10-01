/// @file dx/core/visuals/pixel_format.c
/// @brief Enumeration of pixel formats.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#include "dx/core/visuals/pixel_format.h"

DX_DEFINE_ENUMERATION_TYPE("dx.pixel_format",
                           dx_pixel_format);

dx_result dx_pixel_format_get_number_of_bytes_per_pixel(dx_size* RETURN, dx_pixel_format SELF) {
  switch (SELF) {
    case dx_pixel_format_an8_ln8: {
      *RETURN = 2;
      return DX_SUCCESS;
    } break;
    case dx_pixel_format_bn8_gn8_rn8: {
      *RETURN = 3;
      return DX_SUCCESS;
    } break;
    case dx_pixel_format_bn8_gn8_rn8_an8: {
      *RETURN = 4;
      return DX_SUCCESS;
    } break;
    case dx_pixel_format_ln8: {
      *RETURN = 1;
      return DX_SUCCESS;
    } break;
    case dx_pixel_format_ln8_an8: {
      *RETURN = 2;
      return DX_SUCCESS;
    } break;
    case dx_pixel_format_rn8_gn8_bn8: {
      *RETURN = 3;
      return DX_SUCCESS;
    } break;
    case dx_pixel_format_rn8_gn8_bn8_an8: {
      *RETURN = 4;
      return DX_SUCCESS;
    } break;
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    } break;
  };
}
