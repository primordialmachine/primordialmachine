// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_KEYBOARDKEYMESSAGE_H_INCLUDED)
#define CORE_INPUT_KEYBOARDKEYMESSAGE_H_INCLUDED

#include "Core/Input/InputMessage.h"
#include "Core/Input/KeyboardKey.h"
#include "Core/Input/KeyboardKeyAction.h"

/* http://localhost/core#core-keyboardkeymessage */
Core_declareObjectType("Core.KeyboardKeyMessage",
                       Core_KeyboardKeyMessage,
                       Core_InputMessage);

static inline Core_KeyboardKeyMessage* CORE_KEYBOARDKEYMESSAGE(void* p) {
  return (Core_KeyboardKeyMessage*)p;
}

struct Core_KeyboardKeyMessage {
  Core_InputMessage _parent;
  Core_KeyboardKeyAction action;
  Core_KeyboardKey key;
};

static inline Core_KeyboardKeyMessage_Dispatch* CORE_KEYBOARDKEYMESSAGE_DISPATCH(void* p) {
  return (Core_KeyboardKeyMessage_Dispatch*)p;
}

struct Core_KeyboardKeyMessage_Dispatch {
  Core_InputMessage_Dispatch _parent;
};

/* http://localhost/core#core-keyboardkeymessage-construct */
Core_Result Core_KeyboardKeyMessage_construct(Core_KeyboardKeyMessage* SELF, Core_KeyboardKeyAction action, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

/* http://localhost/core#core-keyboardkeymessage-getaction */
Core_Result Core_KeyboardKeyMessage_getAction(Core_KeyboardKeyAction* RETURN, Core_KeyboardKeyMessage* SELF);

/* http://localhost/core#core-keyboardkeymessage-getkey */
Core_Result Core_KeyboardKeyMessage_getKey(Core_KeyboardKey* RETURN, Core_KeyboardKeyMessage* SELF);

/* http://localhost/core#core-keyboardkeymessage-create */
Core_Result Core_KeyboardKeyMessage_create(Core_KeyboardKeyMessage** RETURN, Core_KeyboardKeyAction action, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

#endif // CORE_INPUT_KEYBOARDKEYMESSAGE_H_INCLUDED
