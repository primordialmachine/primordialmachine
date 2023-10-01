#if !defined(DX_CORE_MSGS_H_INCLUDED)
#define DX_CORE_MSGS_H_INCLUDED

#include "dx/core/object.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_TYPE_UNDETERMINED (0)

#define DX_MSG_TYPE_EMIT (1)

#define DX_MSG_TYPE_QUIT (2)

/// @brief Messages related to the canvas.
#define DX_MSG_TYPE_CANVAS (3)

#define DX_MSG_TYPE_INPUT (4)

// The opaque type of a message.
DX_DECLARE_OBJECT_TYPE("dx.msg",
                       dx_msg,
                       dx_object);

static inline dx_msg* DX_MSG(void* p) {
  return (dx_msg*)p;
}

struct dx_msg {
  dx_object _parent;
  uint32_t flags;
};

static inline dx_msg_dispatch* DX_MSG_DISPATCH(void* p) {
  return (dx_msg_dispatch*)p;
}

struct dx_msg_dispatch {
  dx_object_dispatch _parent;
};

uint32_t dx_msg_get_flags(dx_msg const* msg);

dx_result dx_msg_construct(dx_msg* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "emit" message.
DX_DECLARE_OBJECT_TYPE("dx.emit_msg",
                       dx_emit_msg,
                       dx_msg);

static inline dx_emit_msg* DX_EMIT_MSG(void* p) {
  return (dx_emit_msg*)p;
}

struct dx_emit_msg {
  dx_msg _parent;
  char* p;
  dx_size n;
};

static inline dx_emit_msg_dispatch* DX_EMIT_MSG_DISPATCH(void* p) {
  return (dx_emit_msg_dispatch*)p;
}

struct dx_emit_msg_dispatch {
  dx_msg_dispatch _parent;
};

dx_result dx_emit_msg_construct(dx_emit_msg* SELF, char const* p, dx_size n);

/// @brief Create an "emit" message.
/// @param RETURN A pointer to a <code>dx_emit_msg*</code> variable.
/// @param p A pointer to an utf-8 string of @a n Bytes.
/// @param n The length of the utf-8 string in Bytes pointed to by @a p.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the "emit" message object.
/// The caller acquired a reference to the returned object.
/// @procedure-call
dx_result dx_emit_msg_create(dx_emit_msg** RETURN, char const* p, dx_size n);

int dx_emit_msg_get(dx_emit_msg* emit_msg, char const** p, dx_size* n);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "quit" message.
DX_DECLARE_OBJECT_TYPE("dx.quit_msg",
                       dx_quit_msg,
                       dx_msg);

static inline dx_quit_msg* DX_QUIT_MSG(void* p) {
  return (dx_quit_msg*)p;
}

struct dx_quit_msg {
  dx_msg _parent;
};

static inline dx_quit_msg_dispatch* DX_QUIT_MSG_DISPATCH(void* p) {
  return (dx_quit_msg_dispatch*)p;
}

struct dx_quit_msg_dispatch {
  dx_msg_dispatch _parent;
};

dx_result dx_quit_msg_construct(dx_quit_msg* SELF);

/// @brief Create a "quit" message.
/// @param RETURN A pointer to a <code>dx_quit_msg*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the "quit" message object.
/// The caller acquired a reference to the returned object.
/// @procedure-call
dx_result dx_quit_msg_create(dx_quit_msg** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of a message queue.
typedef struct dx_msg_queue dx_msg_queue;

// Push a message on the message queue.
// @post
// On success:
// The message was added to the queue and the queue acquired a reference to the message.
// @return #DX_SUCCESS on success. #DX_FAILURE on failure.
dx_result dx_msg_queue_push(dx_msg_queue* SELF, dx_msg* msg);

// @brief Pop a message from the message queue.
// @param RETURN A pointer to a <code>dx_msg*</code> variable.
// @param SELF A pointer to this message queue.
// @success
// If a message was available for removal then <code>*RETURN</code> was assigned a pointer to that message and the caller acquired a reference to that message.
// Otherwise the pointer is a null poiinter.
// @return #DX_SUCCESS on success. #DX_FAILURE on failure.
dx_result dx_msg_queue_pop(dx_msg** RETURN, dx_msg_queue* SELF);

// Create a message queue.
// @return pointer to the message queue on success. null pointer on failure.
dx_result dx_msg_queue_create(dx_msg_queue** RETURN);

// Destroy a message queue.
// The message queue relinquishes all references to messages contained in the queue.
void dx_msg_queue_destroy(dx_msg_queue* msg_queue);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_MSGS_H_INCLUDED