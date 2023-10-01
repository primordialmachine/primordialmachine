/// @file dx/core/visuals/texture_address_mode.h
/// @brief Enumeration of texture address modes.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_VISUALS_TEXTURE_ADDRESS_MODE_H_INCLUDED)
#define DX_CORE_VISUALS_TEXTURE_ADDRESS_MODE_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Visuals
/// @brief Enumeration of texture address modes.
/// @details A texture address mode identifies a technique for resolving texture coordinates that are outside of the boundaries of a texture.
DX_DECLARE_ENUMERATION_TYPE("dx.texture_address_mode",
                            dx_texture_address_mode);

enum dx_texture_address_mode {

  /// GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_CLAMP_TO_BORDER in OpenGL,
  /// D3D12_TEXTURE_ADDRESS_MODE_BORDER in Direct3D 12
  dx_texture_address_mode_clamp_to_border,

  /// GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_REPEAT in OpenGL,
  /// D3D12_TEXTURE_ADDRESS_MODE_WRAP in Direct3D 12.
  dx_texture_address_mode_repeat,
  
  /// GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_MIRRORED_REPEAT in OpenGL,
  /// D3D12_TEXTURE_ADDRESS_MODE_MIRROR in Direct3D 12.
  dx_texture_address_mode_mirrored_repeat,

};

#endif // DX_CORE_VISUALS_TEXTURE_ADDRESS_MODE_H_INCLUDED
