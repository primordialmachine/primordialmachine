// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Input/InputMessage.h"

#include "Core/Time.h"

Core_defineObjectType("Core.InputMessage",
                      Core_InputMessage,
                      Core_Message);

static void Core_InputMessage_destruct(Core_InputMessage* SELF)
{/*Intentionally empty.*/}

static void Core_InputMessage_constructDispatch(Core_InputMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_InputMessage_construct(Core_InputMessage* SELF, Core_InputMessageKind kind, Core_ModifierKeys modifierKeys) {
  DX_CONSTRUCT_PREFIX(Core_InputMessage);
  Core_Natural64 timeStamp;
  if (Core_getNow(&timeStamp)) {
    return Core_Failure;
  }
  if (Core_Message_construct(CORE_MESSAGE(SELF), timeStamp)) {
    return Core_Failure;
  }
  SELF->kind = kind;
  SELF->modifierKeys = modifierKeys;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_InputMessage_getKind(Core_InputMessageKind* RETURN, Core_InputMessage* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->kind;
  return Core_Success;
}

Core_Result Core_InputMessage_getModifierKeys(Core_ModifierKeys* RETURN, Core_InputMessage* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->modifierKeys;
  return Core_Success;
}
