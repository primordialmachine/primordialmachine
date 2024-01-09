// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Input/MouseButtonMessage.h"

#include "Core/Time.h"

Core_defineObjectType("Core.MouseButtonMessage",
                      Core_MouseButtonMessage,
                      Core_InputMessage);

static void Core_MouseButtonMessage_destruct(Core_MouseButtonMessage* SELF)
{/*Intentionally empty.*/}

static void Core_MouseButtonMessage_constructDispatch(Core_MouseButtonMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_MouseButtonMessage_construct(Core_MouseButtonMessage* SELF, Core_MouseButtonAction action, Core_MouseButton button, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y) {
  DX_CONSTRUCT_PREFIX(Core_MouseButtonMessage);
  if (Core_InputMessage_construct(CORE_INPUTMESSAGE(SELF), Core_InputMessageKind_MouseButton, modifierKeys)) {
    return Core_Failure;
  }
  SELF->action = action;
  SELF->button = button;
  SELF->x = x;
  SELF->y = y;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_MouseButtonMessage_getAction(Core_MouseButtonAction* RETURN, Core_MouseButtonMessage* SELF) {
  *RETURN = SELF->action;
  return Core_Success;
}

Core_Result Core_MouseButtonMessage_getButton(Core_MouseButton* RETURN, Core_MouseButtonMessage* SELF) {
  *RETURN = SELF->button;
  return Core_Success;
}

Core_Result Core_MouseButtonMessage_getX(Core_Real32* RETURN, Core_MouseButtonMessage* SELF) {
  *RETURN = SELF->x;
  return Core_Success;
}

Core_Result Core_MouseButtonMessage_getY(Core_Real32* RETURN, Core_MouseButtonMessage* SELF) {
  *RETURN = SELF->y;
  return Core_Success;
}

Core_Result Core_MouseButtonMessage_create(Core_MouseButtonMessage** RETURN, Core_MouseButtonAction action, Core_MouseButton button, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y) {
  DX_CREATE_PREFIX(Core_MouseButtonMessage);
  if (Core_MouseButtonMessage_construct(SELF, action, button, modifierKeys, x, y)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
