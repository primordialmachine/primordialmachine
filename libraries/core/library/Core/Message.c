// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Message.h"

Core_defineObjectType("Core.Message",
                      Core_Message,
                      Core_Object);

static void Core_Message_destruct(Core_Message* SELF)
{/*Intentionally empty.*/}

static void Core_Message_constructDispatch(Core_Message_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Message_construct(Core_Message* SELF, Core_Natural64 timeStamp) {
  DX_CONSTRUCT_PREFIX(Core_Message);
  SELF->timeStamp = timeStamp;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
