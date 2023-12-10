/// @file Core/Visuals/TextureAddressMode.h
/// @brief Enumeration of texture address modes.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(CORE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED)
#define CORE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-textureaddressmode */
DX_DECLARE_ENUMERATION_TYPE("Core.TextureAddressMode",
                            Core_TextureAddressMode);

enum Core_TextureAddressMode {

  Core_TextureAddressMode_ClampToEdge,

  Core_TextureAddressMode_ClampToBorder,

  Core_TextureAddressMode_Repeat,

  Core_TextureAddressMode_MirroredRepeat,

};

#endif // CORE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED
