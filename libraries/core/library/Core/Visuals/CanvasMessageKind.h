// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASMESSAGEKIND_H_INCLUDED)
#define CORE_VISUALS_CANVASMESSAGEKIND_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-canvasmessagekind */
Core_declareEnumerationType("Core.CanvasMessageKind",
                            Core_CanvasMessageKind);

enum Core_CanvasMessageKind {

  Core_CanvasMessageKind_StateChanged = 1,
  
  Core_CanvasMessageKind_SizeChanged,

  Core_CanvasMessageKind_DpiChanged,

};

#endif // CORE_VISUALS_CANVASMESSAGEKIND_H_INCLUDED
