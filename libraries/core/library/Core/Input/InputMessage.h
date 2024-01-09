// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_INPUTMESSAGE_H_INCLUDED)
#define CORE_INPUT_INPUTMESSAGE_H_INCLUDED

#include "Core/Message.h"
#include "Core/Input/InputMessageKind.h"
#include "Core/Input/ModifierKeys.h"

/* http://localhost/core#core-inputmessage */
Core_declareObjectType("Core.InputMessage",
                       Core_InputMessage,
                       Core_Message);

static inline Core_InputMessage* CORE_INPUTMESSAGE(void* p) {
  return (Core_InputMessage*)p;
}

struct Core_InputMessage {
  Core_Message _parent;
  Core_InputMessageKind kind;
  Core_ModifierKeys modifierKeys;
};

static inline Core_InputMessage_Dispatch* CORE_INPUTMESSAGE_DISPATCH(void* p) {
  return (Core_InputMessage_Dispatch*)p;
}

struct Core_InputMessage_Dispatch {
  Core_Message_Dispatch _parent;
};

/* http://localhost/core#core-inputmessage-construct */
Core_Result Core_InputMessage_construct(Core_InputMessage* SELF, Core_InputMessageKind kind, Core_ModifierKeys modifierKeys);

/* http://localhost/core#core-inputmessage-getkind */
Core_Result Core_InputMessage_getKind(Core_InputMessageKind* RETURN, Core_InputMessage* SELF);

/* http://localhost/core#core-inputmessage-getmodifierkeys */
Core_Result Core_InputMessage_getModifierKeys(Core_ModifierKeys* RETURN, Core_InputMessage* SELF);

#endif // CORE_INPUT_INPUTMESSAGE_H_INCLUDED
