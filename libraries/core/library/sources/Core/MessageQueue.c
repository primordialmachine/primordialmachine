// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/MessageQueue.h"

#include "dx/core/inline_pointer_deque.h"

Core_defineObjectType("Core.MessageQueue",
                      Core_MessageQueue,
                      Core_Object);

static void Core_MessageQueue_destruct(Core_MessageQueue* SELF) {
  Core_InlineArrayDequeP_uninitialize(&SELF->deque);
}

static void Core_MessageQueue_constructDispatch(Core_MessageQueue_Dispatch* SELF)
{/*Intentionally empty.*/}

static void added(void** p) {
  CORE_REFERENCE(*p);
}

static void removed(void** p) {
  CORE_UNREFERENCE(*p);
}

Core_Result Core_MessageQueue_construct(Core_MessageQueue* SELF) {
  Core_BeginConstructor(Core_MessageQueue);
  static Core_InlineArrayDequeP_Configuration const configuration = {
    .added_callback = &added,
    .removed_callback = &removed,
  };
  if (Core_InlineArrayDequeP_initialize(&SELF->deque, 0, &configuration)) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_MessageQueue);
}

Core_Result Core_MessageQueue_create(Core_MessageQueue** RETURN) {
  DX_CREATE_PREFIX(Core_MessageQueue);
  if (Core_MessageQueue_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_MessageQueue_push(Core_MessageQueue* SELF, Core_Message* message) {
  if (!SELF || !message) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_InlineArrayDequeP_push_back(&SELF->deque, message)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_MessageQueue_pop(Core_Message** RETURN, Core_MessageQueue* SELF) {
  Core_Message* msg = NULL;
  if (Core_InlineArrayDequeP_pop_front(&msg, &SELF->deque, true)) {
    if (Core_Error_Empty != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
      *RETURN = NULL;
      return Core_Success;
    }
  }
  *RETURN = msg;
  return Core_Success;
}
