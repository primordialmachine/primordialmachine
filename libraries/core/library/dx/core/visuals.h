/// @file dx/core/visuals.h
/// @brief Core functionality with a focus on visuals.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_VISUALS_H_INCLUDED)
#define DX_CORE_VISUALS_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Core/Visuals/CanvasDpiChangedMessage.h"
#include "Core/Visuals/CanvasSizeChangedMessage.h"
#include "Core/Visuals/CanvasStateChangedMessage.h"
#include "Core/Visuals/CullMode.h"
#include "Core/Visuals/DepthCompareFunction.h"
#include "core/Visuals/PixelFormat.h"
#include "Core/Visuals/TextureAddressMode.h"
#include "Core/Visuals/VertexFormat.h"
#include "Core/Visuals/WindingMode.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Visuals
/// @brief Enumeration of texture minification filters.
Core_declareEnumerationType("Core.TextureMinificationFilter",
                            Core_TextureMinificationFilter);

enum Core_TextureMinificationFilter {

  /// GL_TEXTURE_MIN_FILTER with GL_NEAREST in OpenGL.
  Core_TextureMinificationFilter_Nearest,

  /// GL_TEXTURE_MIN_FILTER with GL_LINEAR in OpenGL
  Core_TextureMinificationFilter_Linear,

};

/// @ingroup Core_Visuals
/// @brief Enumeration of texture magnification filters.
Core_declareEnumerationType("Core.TextureMagnificationFilter",
                            Core_TextureMagnificationFilter);

enum Core_TextureMagnificationFilter {

  /// GL_TEXTURE_MAG_FILTER with GL_NEAREST in OpenGL.
  Core_TextureMagnificationFilter_Nearest,

  /// GL_TEXTURE_MAG_FILTER with GL_LINEAR in OpenGL.
  Core_TextureMagnificationFilter_Linear,

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_VISUALS_H_INCLUDED
