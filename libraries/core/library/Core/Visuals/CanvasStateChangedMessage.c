// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Visuals/CanvasStateChangedMessage.h"

Core_defineObjectType("Core.CanvasStateChangedMessage",
                      Core_CanvasStateChangedMessage,
                      Core_CanvasMessage);

static void Core_CanvasStateChangedMessage_destruct(Core_CanvasStateChangedMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasStateChangedMessage_constructDispatch(Core_CanvasStateChangedMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_CanvasStateChangedMessage_construct(Core_CanvasStateChangedMessage* SELF, Core_Boolean active) {
  DX_CONSTRUCT_PREFIX(Core_CanvasStateChangedMessage);
  if (Core_CanvasMessage_construct(CORE_CANVASMESSAGE(SELF), Core_CanvasMessageKind_StateChanged)) {
    return Core_Failure;
  }
  SELF->active = active;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_CanvasStateChangedMessage_create(Core_CanvasStateChangedMessage** RETURN, Core_Boolean active) {
  DX_CREATE_PREFIX(Core_CanvasStateChangedMessage);
  if (Core_CanvasStateChangedMessage_construct(SELF, active)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
