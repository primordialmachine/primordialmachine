/// @file dx/core/visuals.h
/// @brief Core functionality with a focus on visuals.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_VISUALS_H_INCLUDED)
#define DX_CORE_VISUALS_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/visuals/cull_mode.h"
#include "dx/core/visuals/depth_test_function.h"
#include "dx/core/visuals/pixel_format.h"
#include "dx/core/visuals/texture_address_mode.h"
#include "dx/core/visuals/vertex_format.h"
#include "dx/core/visuals/winding_mode.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Visuals
/// @brief Enumeration of texture minification filters.
DX_DECLARE_ENUMERATION_TYPE("dx.texture_minification_filter",
                            dx_texture_minification_filter);

enum dx_texture_minification_filter {

  /// GL_TEXTURE_MIN_FILTER with GL_NEAREST in OpenGL
  dx_texture_minification_filter_nearest,

  /// GL_TEXTURE_MIN_FILTER with GL_LINEAR in OpenGL
  dx_texture_minification_filter_linear,

};

/// @ingroup Core_Visuals
/// @brief Enumeration of texture magnification filters.
DX_DECLARE_ENUMERATION_TYPE("dx.texture_magnification_filter",
                            dx_texture_magnification_filter);

enum dx_texture_magnification_filter {

  /// GL_TEXTURE_MAG_FILTER with GL_NEAREST in OpenGL
  dx_texture_magnification_filter_nearest,

  /// GL_TEXTURE_MAG_FILTER with GL_LINEAR in OpenGL
  dx_texture_magnification_filter_linear,

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_VISUALS_H_INCLUDED
