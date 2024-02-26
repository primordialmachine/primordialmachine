// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_MOUSEPOINTERMESSAGE_H_INCLUDED)
#define CORE_INPUT_MOUSEPOINTERMESSAGE_H_INCLUDED

#include "Core/Input/InputMessage.h"
#include "Core/Input/MousePointerAction.h"

/* http://localhost/core#core-mousepointermessage */
Core_declareObjectType("Core.MousePointerMessage",
                       Core_MousePointerMessage,
                       Core_InputMessage);

static inline Core_MousePointerMessage* CORE_MOUSEPOINTERMESSAGE(void* p) {
  return (Core_MousePointerMessage*)p;
}

struct Core_MousePointerMessage {
  Core_InputMessage _parent;
  Core_MousePointerAction action;
  Core_Real32 x, y;
};

static inline Core_MousePointerMessage* CORE_MOUSEPOINTERMESSAGE_DISPATCH(void* p) {
  return (Core_MousePointerMessage*)p;
}

struct Core_MousePointerMessage_Dispatch {
  Core_InputMessage_Dispatch _parent;
};

/* http://localhost/core#core-mousepointermessage-construct */
Core_Result Core_MousePointerMessage_construct(Core_MousePointerMessage* SELF, Core_MousePointerAction action, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/* http://localhost/core#core-mousepointermessage-getaction */
Core_Result Core_MousePointerMessage_getAction(Core_MousePointerAction* RETURN, Core_MousePointerMessage* SELF);

/* http://localhost/core#core-mousepointermessage-getx */
Core_Result Core_MousePointerMessage_getX(Core_Real32* RETURN, Core_MousePointerMessage* SELF);

/* http://localhost/core#core-mousepointermessage-gety */
Core_Result Core_MousePointerMessage_getY(Core_Real32* RETURN, Core_MousePointerMessage* SELF);

/* http://localhost/core#core-mousepointermessage-create */
Core_Result Core_MousePointerMessage_create(Core_MousePointerMessage** RETURN, Core_MousePointerAction action, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

#endif // CORE_INPUT_MOUSEPOINTERMESSAGE_H_INCLUDED
