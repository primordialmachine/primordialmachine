// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_DEPTHCOMPAREMODE_H_INCLUDED)
#define CORE_VISUALS_DEPTHCOMPAREMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-depthcomparemode */
Core_declareEnumerationType("Core.DepthCompareMode",
                            Core_DepthCompareMode);

enum Core_DepthCompareMode {

  Core_DepthCompareMode_Always,

  Core_DepthCompareMode_Never,

  Core_DepthCompareMode_Equal,

  Core_DepthCompareMode_NotEqual,

  Core_DepthCompareMode_LessThan,

  Core_DepthCompareMode_LessThanOrEqualTo,

  Core_DepthCompareMode_GreaterThan,

  Core_DepthCompareMode_GreaterThanOrEqualTo,

};

#endif // CORE_VISUALS_DEPTHCOMPAREMODE_H_INCLUDED
