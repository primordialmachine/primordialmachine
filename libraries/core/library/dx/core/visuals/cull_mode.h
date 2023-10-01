/// @file dx/core/visuals/cull_mode.h
/// @brief Enumeration of cull modes.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_VISUALS_CULL_MODE_H_INCLUDED)
#define DX_CORE_VISUALS_CULL_MODE_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Visuals
/// @brief Enumeration of cull modes.
DX_DECLARE_ENUMERATION_TYPE("dx.cull_mode",
                            dx_cull_mode);

enum dx_cull_mode {
  
  /// @brief Neither front-facing nor back-facing geometry is culled.
  dx_cull_mode_none,
  
  /// @brief Front-facing geometry is culled.
  dx_cull_mode_front,

  /// @brief Back-facing geometry is culled.
  dx_cull_mode_back,

  /// @brief Both front-facing and back-facing geometry is culled.
  dx_cull_mode_front_and_back,

};

#endif // DX_CORE_VISUALS_CULL_MODE_H_INCLUDED
