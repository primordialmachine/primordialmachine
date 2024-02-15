// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_MESSAGEQUEUE_H_INCLUDED)
#define CORE_MESSAGEQUEUE_H_INCLUDED

#include "Core/Message.h"
#include "dx/core/inline_pointer_deque.h"

/* http://localhost/core#core-messagequeue */
Core_declareObjectType("Core.MessageQueue",
                       Core_MessageQueue,
                       Core_Object);

static inline Core_MessageQueue* CORE_MESSAGEQUEUE(void* p) {
  return (Core_MessageQueue*)p;
}

struct Core_MessageQueue {
  Core_Object _parent;
  Core_InlineArrayDequeP deque;
};

static inline Core_MessageQueue_Dispatch* CORE_MESSAGEQUEUE_DISPATCH(void* p) {
  return (Core_MessageQueue_Dispatch*)p;
}

struct Core_MessageQueue_Dispatch {
  Core_Object_Dispatch _parent;
};

/* http://localhost/core#core-messagequeue-construct */
Core_Result Core_MessageQueue_construct(Core_MessageQueue* SELF);

/* http://localhost/core#core-messagequeue-create */
Core_Result Core_MessageQueue_create(Core_MessageQueue** RETURN);

/* http://localhost/core#core-messagequeue-push */
Core_Result Core_MessageQueue_push(Core_MessageQueue* SELF, Core_Message* message);

/* http://localhost/core#core-messagequeue-pop */
Core_Result Core_MessageQueue_pop(Core_Message** RETURN, Core_MessageQueue* SELF);

#endif // CORE_MESSAGEQUEUE_H_INCLUDED
