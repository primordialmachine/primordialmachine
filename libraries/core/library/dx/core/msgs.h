#if !defined(CORE_MESSAGES_H_INCLUDED)
#define CORE_MESSAGES_H_INCLUDED

#include "Core/Message.h"
#include "Core/ApplicationMessage.h"
#include "Core/EmitMessage.h"

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
