// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_MOUSEPOINTERACTION_H_INCLUDED)
#define CORE_INPUT_MOUSEPOINTERACTION_H_INCLUDED

#include "dx/core/TypeSystem.h"
typedef struct Core_String Core_String;

/* http://localhost/core#core-mousepointeraction */
Core_declareEnumerationType("Core.MousePointerAction",
                            Core_MousePointerAction);

enum Core_MousePointerAction {

  Core_MousePointerAction_Entered = 1,

  Core_MousePointerAction_Exited,

  Core_MousePointerAction_Moved,

};

#endif // CORE_INPUT_MOUSEPOINTERACTION_H_INCLUDED
