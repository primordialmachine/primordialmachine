// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_DEPTHCOMPAREFUNCTION_H_INCLUDED)
#define CORE_VISUALS_DEPTHCOMPAREFUNCTION_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-depthcomparefunction */
DX_DECLARE_ENUMERATION_TYPE("Core.DepthCompareFunction",
                            Core_DepthCompareFunction);

enum Core_DepthCompareFunction {

  Core_DepthCompareFunction_Always,

  Core_DepthCompareFunction_Never,

  Core_DepthCompareFunction_Equal,

  Core_DepthCompareFunction_NotEqual,

  Core_DepthCompareFunction_LessThan,

  Core_DepthCompareFunction_LessThanOrEqualTo,

  Core_DepthCompareFunction_GreaterThan,

  Core_DepthCompareFunction_GreaterThanOrEqualTo,

};

#endif // CORE_VISUALS_DEPTHCOMPAREFUNCTION_H_INCLUDED