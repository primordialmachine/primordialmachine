// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_TEXTUREFILTER_H_INCLUDED)
#define CORE_VISUALS_TEXTUREFILTER_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/vals#core-texturefilter */
Core_declareEnumerationType("Core.TextureFilter",
                            Core_TextureFilter);

enum Core_TextureFilter {

  Core_TextureFilter_Nearest,

  Core_TextureFilter_Linear,

};

#endif // CORE_VISUALS_TEXTUREFILTER_H_INCLUDED
