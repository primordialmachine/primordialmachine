// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Visuals/CanvasMessage.h"

#include "Core/Time.h"

Core_defineObjectType("Core.CanvasMessage",
                      Core_CanvasMessage,
                      Core_Message);

static void Core_CanvasMessage_destruct(Core_CanvasMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasMessage_constructDispatch(Core_CanvasMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_CanvasMessage_construct(Core_CanvasMessage* SELF, Core_CanvasMessageKind kind) {
  DX_CONSTRUCT_PREFIX(Core_CanvasMessage);
  Core_Natural64 timeStamp;
  if (Core_getNow(&timeStamp)) {
    return Core_Failure;
  }
  if (Core_Message_construct(CORE_MESSAGE(SELF), timeStamp)) {
    return Core_Failure;
  }
  SELF->kind = kind;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_CanvasMessage_getKind(Core_CanvasMessageKind* RETURN, Core_CanvasMessage* SELF) {
  *RETURN = SELF->kind;
  return Core_Success;
}
