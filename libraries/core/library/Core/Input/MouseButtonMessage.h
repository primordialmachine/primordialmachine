// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_MOUSEBUTTONMESSAGE_H_INCLUDED)
#define CORE_INPUT_MOUSEBUTTONMESSAGE_H_INCLUDED

#include "Core/Input/InputMessage.h"
#include "Core/Input/MouseButton.h"
#include "Core/Input/MouseButtonAction.h"

/* http://localhost/core#core-mousebuttonmessage */
Core_declareObjectType("Core.MouseButtonMessage",
                       Core_MouseButtonMessage,
                       Core_InputMessage);

static inline Core_MouseButtonMessage* CORE_MOUSEBUTTONMESSAGE(void* p) {
  return (Core_MouseButtonMessage*)p;
}

struct Core_MouseButtonMessage {
  Core_InputMessage _parent;
  Core_MouseButtonAction action;
  Core_MouseButton button;
  Core_Real32 x, y;
};

static inline Core_MouseButtonMessage* CORE_MOUSEBUTTONMESSAGE_DISPATCH(void* p) {
  return (Core_MouseButtonMessage*)p;
}


struct Core_MouseButtonMessage_Dispatch {
  Core_InputMessage_Dispatch _parent;
};

/* http://localhost/core#core-mousebuttonmessage-construct */
Core_Result Core_MouseButtonMessage_construct(Core_MouseButtonMessage* SELF, Core_MouseButtonAction action, Core_MouseButton button, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y);

/* http://localhost/core#core-mousebuttonmessage-getaction */
Core_Result Core_MouseButtonMessage_getAction(Core_MouseButtonAction* RETURN, Core_MouseButtonMessage* SELF);

/* http://localhost/core#core-mousebuttonmessage-getbutton */
Core_Result Core_MouseButtonMessage_getButton(Core_MouseButton* RETURN, Core_MouseButtonMessage* SELF);

/* http://localhost/core#core-mousebuttonmessage-getx */
Core_Result Core_MouseButtonMessage_getX(Core_Real32* RETURN, Core_MouseButtonMessage* SELF);

/* http://localhost/core#core-mousebuttonmessage-gety */
Core_Result Core_MouseButtonMessage_getY(Core_Real32* RETURN, Core_MouseButtonMessage* SELF);

/* http://localhost/core#core-mousebuttonmessage-create */
Core_Result Core_MouseButtonMessage_create(Core_MouseButtonMessage** RETURN, Core_MouseButtonAction action, Core_MouseButton button, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y);

#endif // CORE_INPUT_MOUSEBUTTONMESSAGE_H_INCLUDED
