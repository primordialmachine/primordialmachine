// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED)
#define CORE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-textureaddressmode */
Core_declareEnumerationType("Core.TextureAddressMode",
                            Core_TextureAddressMode);

enum Core_TextureAddressMode {

  Core_TextureAddressMode_ClampToEdge,

  Core_TextureAddressMode_ClampToBorder,

  Core_TextureAddressMode_Repeat,

  Core_TextureAddressMode_MirroredRepeat,

};

#endif // CORE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED
