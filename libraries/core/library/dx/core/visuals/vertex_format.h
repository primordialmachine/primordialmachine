/// @file dx/core/visuals/vertex_format.h
/// @brief Enumeration of vertex formats.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_VISUALS_VERTEX_FORMAT_H_INCLUDED)
#define DX_CORE_VISUALS_VERTEX_FORMAT_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Visuals
/// @brief Enumeration of vertex formats.
DX_DECLARE_ENUMERATION_TYPE("dx.vertex_format",
                            dx_vertex_format);

enum dx_vertex_format {

  /// @brief A vertex of the format position.
  dx_vertex_format_position_xyz = 1,

  /// @brief A vertex of the format color.
  dx_vertex_format_ambient_rgba = 2,

  /// @brief A vertex of the format texture.
  dx_vertex_format_ambient_uv = 4,

  /// @brief A vertex of the format position first, color second.
  dx_vertex_format_position_xyz_ambient_rgba = (dx_vertex_format_position_xyz | dx_vertex_format_ambient_rgba),

  /// @brief A vertex of the format position first, ambient texture second.
  dx_vertex_format_position_xyz_ambient_uv = (dx_vertex_format_position_xyz | dx_vertex_format_ambient_uv),

  /// @brief A vertex of the format position first, color second, ambient texture third.
  dx_vertex_format_position_xyz_ambient_rgba_ambient_uv = (dx_vertex_format_position_xyz | dx_vertex_format_ambient_rgba | dx_vertex_format_ambient_uv),

};

#endif // DX_CORE_VISUALS_VERTEX_FORMAT_H_INCLUDED
