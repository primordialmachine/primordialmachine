// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASMESSAGEKIND_H_INCLUDED)
#define CORE_VISUALS_CANVASMESSAGEKIND_H_INCLUDED

#include "dx/core/TypeSystem.h"

/// @brief An enumeration of the different kinds of canvas messages.
DX_DECLARE_ENUMERATION_TYPE("Core.CanvasMessageKind",
                            Core_CanvasMessageKind);

enum Core_CanvasMessageKind {

  /// @brief Kind of a a canvas activated message.
  Core_CanvasMessageKind_Activated,

  /// @brief Kind of a canvas deactivated message.
  Core_CanvasMessageKind_Deactivated,

  /// @brief Kind of a canvas size changed message.
  Core_CanvasMessageKind_SizeChanged,

  /// @brief Kind of a canvas DPI changed message.
  Core_CanvasMessageKind_DpiChanged,

};


#endif // CORE_VISUALS_CANVASMESSAGEKIND_H_INCLUDED
