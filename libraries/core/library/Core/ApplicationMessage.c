// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/ApplicationMessage.h"

#include "Core/Time.h"

Core_defineObjectType("Core.ApplicationMessage",
                      Core_ApplicationMessage,
                      Core_Message);

static void Core_ApplicationMessage_destruct(Core_ApplicationMessage* SELF)
{/*Intentionally empty.*/}

static void Core_ApplicationMessage_constructDispatch(Core_ApplicationMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_ApplicationMessage_construct(Core_ApplicationMessage* SELF, Core_ApplicationMessageKind kind) {
  DX_CONSTRUCT_PREFIX(Core_ApplicationMessage);
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

Core_Result Core_ApplicationMessage_create(Core_ApplicationMessage** RETURN, Core_ApplicationMessageKind kind) {
  DX_CREATE_PREFIX(Core_ApplicationMessage);
  if (Core_ApplicationMessage_construct(SELF, kind)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
