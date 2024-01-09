// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_VERTEXFORMAT_H_INCLUDED)
#define CORE_VISUALS_VERTEXFORMAT_H_INCLUDED

#include "dx/core/typesystem.h"

/* http://localhost/val#core-vertexformat */
Core_declareEnumerationType("Core.VertexFormat",
                            Core_VertexFormat);

enum Core_VertexFormat {

  Core_VertexFormat_position_xyz = 1,

  Core_VertexFormat_ambient_rgba = 2,

  Core_VertexFormat_ambient_uv = 4,

  Core_VertexFormat_position_xyz_ambient_rgba = (Core_VertexFormat_position_xyz | Core_VertexFormat_ambient_rgba),

  Core_VertexFormat_position_xyz_ambient_uv = (Core_VertexFormat_position_xyz | Core_VertexFormat_ambient_uv),

  Core_VertexFormat_position_xyz_ambient_rgba_ambient_uv = (Core_VertexFormat_position_xyz | Core_VertexFormat_ambient_rgba | Core_VertexFormat_ambient_uv),

};

#endif // CORE_VISUALS_VERTEXFORMAT_H_INCLUDED
