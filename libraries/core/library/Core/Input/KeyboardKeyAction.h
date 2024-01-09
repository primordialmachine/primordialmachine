// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_KEYBOARDKEYACTION_H_INCLUDED)
#define CORE_INPUT_KEYBOARDKEYACTION_H_INCLUDED

#include "dx/core/TypeSystem.h"
typedef struct Core_String Core_String;

/* http://localhost/core#core-keyboardkeyaction */
Core_declareEnumerationType("Core.KeyboardKeyAction",
                            Core_KeyboardKeyAction);

enum Core_KeyboardKeyAction {

  Core_KeyboardKeyAction_Pressed = 1,

  Core_KeyboardKeyAction_Released,

};

#endif // CORE_INPUT_KEYBOARDKEYACTION_H_INCLUDED
