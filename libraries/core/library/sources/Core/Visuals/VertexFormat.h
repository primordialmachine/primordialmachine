// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_VERTEXFORMAT_H_INCLUDED)
#define CORE_VISUALS_VERTEXFORMAT_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-vertexformat */
Core_declareEnumerationType("Core.VertexFormat",
                            Core_VertexFormat);

enum Core_VertexFormat {
#pragma push_macro("Shift")
#define Shift (4)

  // ...xx1
  Core_VertexFormat_PositionXyz = 1,

  // ...x10
  Core_VertexFormat_AmbientRgba = 2,

  // ...100
  Core_VertexFormat_AmbientUv = 4,

  Core_VertexFormat_PositionXyzAmbientRgba = (Core_VertexFormat_PositionXyz << (Shift*1) | Core_VertexFormat_AmbientRgba << (Shift * 0)),

  Core_VertexFormat_PositionXyzAmbientUv = (Core_VertexFormat_PositionXyz << (Shift*1) | Core_VertexFormat_AmbientUv << (Shift*0)),

  Core_VertexFormat_PositionXyzAmbientRgbaAmbientUv = (Core_VertexFormat_PositionXyz << (Shift*2) | Core_VertexFormat_AmbientRgba << (Shift*1) | Core_VertexFormat_AmbientUv << (Shift*0)),
  
#undef Shift
#pragma pop_macro("Shift")
};

#endif // CORE_VISUALS_VERTEXFORMAT_H_INCLUDED
