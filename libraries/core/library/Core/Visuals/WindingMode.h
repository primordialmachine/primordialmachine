// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_WINDINGMODE_H_INCLUDED)
#define CORE_VISUALS_WINDINGMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-windingmode */
DX_DECLARE_ENUMERATION_TYPE("Core.WindingMode",
                            Core_WindingMode);

enum Core_WindingMode {

  Core_WindingMode_Clockwise,

  Core_WindingMode_CounterClockwise,

};

#endif // CORE_VISUALS_WINDINGMODE_H_INCLUDED
