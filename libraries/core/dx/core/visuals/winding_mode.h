/// @file dx/core/visuals/winding_mode.h
/// @brief Enumeration of winding modes.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_VISUALS_WINDING_MODE_H_INCLUDED)
#define DX_CORE_VISUALS_WINDING_MODE_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Visuals
/// @brief Enumeration of winding modes.
DX_DECLARE_ENUMERATION_TYPE("dx.winding_mode",
                            dx_winding_mode);

enum dx_winding_mode {

  /// @brief Clockwise winding mode.
  dx_winding_mode_clockwise,

  /// @brief Counter clockwise winding mode.
  dx_winding_mode_counter_clockwise,

};

#endif // DX_CORE_VISUALS_WINDING_MODE_H_INCLUDED
