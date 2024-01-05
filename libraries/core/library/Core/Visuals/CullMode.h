// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CULLMODE_H_INCLUDED)
#define CORE_VISUALS_CULLMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-cullmode */
DX_DECLARE_ENUMERATION_TYPE("Core.CullMode",
                            Core_CullMode);

enum Core_CullMode {

  Core_CullMode_None,

  Core_CullMode_Front,

  Core_CullMode_Back,

  Core_CullMode_FrontAndBack,

  Core_CullMode_BackAndFront,

};

#endif // CORE_VISUALS_CULLMODE_H_INCLUDED
