// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Input/KeyboardKeyMessage.h"

#include "Core/Time.h"

Core_defineObjectType("Core.KeyboardKeyMessage",
                      Core_KeyboardKeyMessage,
                      Core_InputMessage);

static void Core_KeyboardKeyMessage_destruct(Core_KeyboardKeyMessage* SELF)
{/*Intentionally empty.*/}

static void Core_KeyboardKeyMessage_constructDispatch(Core_KeyboardKeyMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_KeyboardKeyMessage_construct(Core_KeyboardKeyMessage* SELF, Core_KeyboardKeyAction action, Core_KeyboardKey key, Core_ModifierKeys modifierKeys) {
  DX_CONSTRUCT_PREFIX(Core_KeyboardKeyMessage);
  if (Core_InputMessage_construct(CORE_INPUTMESSAGE(SELF), Core_InputMessageKind_KeyboardKey, modifierKeys)) {
    return Core_Failure;
  }
  SELF->action = action;
  SELF->key = key;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_KeyboardKeyMessage_getAction(Core_KeyboardKeyAction* RETURN, Core_KeyboardKeyMessage* SELF) {
  *RETURN = SELF->action;
  return Core_Success;
}

Core_Result Core_KeyboardKeyMessage_getKey(Core_KeyboardKey* RETURN, Core_KeyboardKeyMessage* SELF) {
  *RETURN = SELF->key;
  return Core_Success;
}

Core_Result Core_KeyboardKeyMessage_create(Core_KeyboardKeyMessage** RETURN, Core_KeyboardKeyAction action, Core_KeyboardKey key, Core_ModifierKeys modifierKeys) {
  DX_CREATE_PREFIX(Core_KeyboardKeyMessage);
  if (Core_KeyboardKeyMessage_construct(SELF, action, key, modifierKeys)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
