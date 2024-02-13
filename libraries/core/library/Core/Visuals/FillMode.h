// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_VISUALS_FILLMODE_H_INCLUDED)
#define CORE_VISUALS_FILLMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-fillmode */
Core_declareEnumerationType("Core.FillMode",
                            Core_FillMode);

enum Core_FillMode {

  Core_FillMode_Solid = 1,

  Core_FillMode_Lines = 2,

  Core_FillMode_Point = 3,

};

#endif // CORE_VISUALS_FILLMODE_H_INCLUDED
