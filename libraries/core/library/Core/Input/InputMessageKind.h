// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_INPUTMESSAGEKIND_H_INCLUDED)
#define CORE_INPUT_INPUTMESSAGEKIND_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/core#core-inputmessagekind */
Core_declareEnumerationType("Core.InputMessageKind",
                            Core_InputMessageKind);

enum Core_InputMessageKind {

  Core_InputMessageKind_KeyboardKey = 1,
  
  Core_InputMessageKind_MouseButton,
  
  Core_InputMessageKind_MousePointer,

};

#endif // CORE_INPUT_INPUTMESSAGEKIND_H_INCLUDED
