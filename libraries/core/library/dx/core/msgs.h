#if !defined(CORE_MESSAGES_H_INCLUDED)
#define CORE_MESSAGES_H_INCLUDED

#include "dx/core/Object.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Core/Message.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "emit" message.
DX_DECLARE_OBJECT_TYPE("Core.EmitMessage",
                       Core_EmitMessage,
                       Core_Message);

static inline Core_EmitMessage* CORE_EMITMESSAGE(void* p) {
  return (Core_EmitMessage*)p;
}

struct Core_EmitMessage {
  Core_Message _parent;
  char* p;
  Core_Size n;
};

static inline Core_EmitMessage_dispatch* CORE_EMITMESSAGE_DISPATCH(void* p) {
  return (Core_EmitMessage_dispatch*)p;
}

struct Core_EmitMessage_dispatch {
  Core_Message_dispatch _parent;
};

/// @constructor{Core_EmitMessage}
Core_Result Core_EmitMessage_construct(Core_EmitMessage* SELF, char const* p, Core_Size n);

/// @brief Create an "emit" message.
/// @param RETURN A pointer to a <code>Core_EmitMessage*</code> variable.
/// @param p A pointer to an utf-8 string of @a n Bytes.
/// @param n The length of the utf-8 string in Bytes pointed to by @a p.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the "emit" message object.
/// The caller acquired a reference to the returned object.
/// @create-operator{Core_EmitMessage}
Core_Result Core_EmitMessage_create(Core_EmitMessage** RETURN, char const* p, Core_Size n);

Core_Result Core_EmitMessage_get(Core_EmitMessage* SELF, char const** p, Core_Size* n);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("Core.ApplicationMessageKind",
                            Core_ApplicationMessageKind);

enum Core_ApplicationMessageKind {
  Core_ApplicationMessageKind_QuitRequested = 1,
};

// The opaque type of an "quit" message.
DX_DECLARE_OBJECT_TYPE("Core.ApplicationMessage",
                       Core_ApplicationMessage,
                       Core_Message);

static inline Core_ApplicationMessage* CORE_APPLICATIONMESSAGE(void* p) {
  return (Core_ApplicationMessage*)p;
}

struct Core_ApplicationMessage {
  Core_Message _parent;
  Core_ApplicationMessageKind kind;
};

static inline Core_ApplicationMessage_dispatch* CORE_APPLICATIONMESSAGE_DISPATCH(void* p) {
  return (Core_ApplicationMessage_dispatch*)p;
}

struct Core_ApplicationMessage_dispatch {
  Core_Message_dispatch _parent;
};

/// @constructor{Core_ApplicationMessage}
Core_Result Core_ApplicationMessage_construct(Core_ApplicationMessage* SELF, Core_ApplicationMessageKind kind);

/// @create-operator{Core_ApplicationMessage}
Core_Result Core_ApplicationMessage_create(Core_ApplicationMessage** RETURN, Core_ApplicationMessageKind kind);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of a message queue.
typedef struct dx_msg_queue dx_msg_queue;

/// @brief Push a message on the message queue.
/// @param message A pointer to the message.
/// @success The message was added to the queue and the queue acquired a reference to the message.
/// method{dx_msg_queue}
Core_Result dx_msg_queue_push(dx_msg_queue* SELF, Core_Message* message);

/// @brief Pop a message from the message queue.
/// @param RETURN A pointer to a <code>dx_msg*</code> variable.
/// @param SELF A pointer to this message queue.
/// @success
/// If a message was available for removal then <code>*RETURN</code> was assigned a pointer to that message and the caller acquired a reference to that message.
/// Otherwise the pointer is a null poiinter.
/// @method{dx_msg_queue}
Core_Result dx_msg_queue_pop(Core_Message** RETURN, dx_msg_queue* SELF);

// Destroy a message queue.
// The message queue relinquishes all references to messages contained in the queue.
void dx_msg_queue_destroy(dx_msg_queue* msg_queue);

/// @create-operator{dx_msg_queue}
Core_Result dx_msg_queue_create(dx_msg_queue** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_MESSAGES_H_INCLUDED
