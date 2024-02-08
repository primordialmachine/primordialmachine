#if !defined(CORE_MESSAGEQUEUE_H_INCLUDED)
#define CORE_MESSAGEQUEUE_H_INCLUDED

#include "Core/Message.h"
#include "dx/core/inline_pointer_deque.h"

// A message queue.
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

/// @constructor{Core_MessageQueue}
Core_Result Core_MessageQueue_construct(Core_MessageQueue* SELF);

/// @create-operator{Core_MessageQueue}
Core_Result Core_MessageQueue_create(Core_MessageQueue** RETURN);


/// @brief Push a message on the message queue.
/// @param message A pointer to the message.
/// @success The message was added to the queue and the queue acquired a reference to the message.
/// method{Core_MessageQueue}
Core_Result Core_MessageQueue_push(Core_MessageQueue* SELF, Core_Message* message);

/// @brief Pop a message from the message queue.
/// @param RETURN A pointer to a <code>dx_msg*</code> variable.
/// @param SELF A pointer to this message queue.
/// @success
/// If a message was available for removal then <code>*RETURN</code> was assigned a pointer to that message and the caller acquired a reference to that message.
/// Otherwise the pointer is a null poiinter.
/// @method{Core_MessageQueue}
Core_Result Core_MessageQueue_pop(Core_Message** RETURN, Core_MessageQueue* SELF);

#endif // CORE_MESSAGEQUEUE_H_INCLUDED
