// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Input/MousePointerMessage.h"

#include "Core/Time.h"

Core_defineObjectType("Core.MousePointerMessage",
                      Core_MousePointerMessage,
                      Core_InputMessage);

static void Core_MousePointerMessage_destruct(Core_MousePointerMessage* SELF)
{/*Intentionally empty.*/}

static void Core_MousePointerMessage_constructDispatch(Core_MousePointerMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_MousePointerMessage_construct(Core_MousePointerMessage* SELF, Core_MousePointerAction action, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  DX_CONSTRUCT_PREFIX(Core_MousePointerMessage);
  if (Core_InputMessage_construct(CORE_INPUTMESSAGE(SELF), Core_InputMessageKind_MousePointer, modifierKeys)) {
    return Core_Failure;
  }
  SELF->action = action;
  SELF->x = x;
  SELF->y = y;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_MousePointerMessage_getAction(Core_MousePointerAction* RETURN, Core_MousePointerMessage* SELF) {
  *RETURN = SELF->action;
  return Core_Success;
}

Core_Result Core_MousePointerMessage_getX(Core_Real32* RETURN, Core_MousePointerMessage* SELF) {
  *RETURN = SELF->x;
  return Core_Success;
}

Core_Result Core_MousePointerMessage_getY(Core_Real32* RETURN, Core_MousePointerMessage* SELF) {
  *RETURN = SELF->y;
  return Core_Success;
}

Core_Result Core_MousePointerMessage_create(Core_MousePointerMessage** RETURN, Core_MousePointerAction action, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  DX_CREATE_PREFIX(Core_MousePointerMessage);
  if (Core_MousePointerMessage_construct(SELF, action, x, y, modifierKeys)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
