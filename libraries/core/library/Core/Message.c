// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#include "Core/Message.h"

DX_DEFINE_OBJECT_TYPE("Core.Message",
                      Core_Message,
                      Core_Object);

static void Core_Message_destruct(Core_Message* SELF)
{/*Intentionally empty.*/}

static void Core_Message_constructDispatch(Core_Message_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Message_construct(Core_Message* SELF, Core_Natural64 timeStamp) {
  DX_CONSTRUCT_PREFIX(Core_Message);
  SELF->timeStamp = timeStamp;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
