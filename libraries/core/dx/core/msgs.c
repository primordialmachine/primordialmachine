#include "dx/core/msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/memory.h"
#include "dx/core/inline_pointer_deque.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_TRACE (0)

#if defined(DX_MSG_TRACE) && 1 == DX_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("dx.msg",
                      dx_msg,
                      dx_object);

uint32_t dx_msg_get_flags(dx_msg const* msg) {
  return msg->flags;
}

static void dx_msg_destruct(dx_msg* msg)
{/*Intentionally empty.*/}

static void dx_msg_dispatch_construct(dx_msg_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_msg_construct(dx_msg* SELF) {
  TRACE("enter: dx_msg_construct\n");
  dx_rti_type* TYPE = dx_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->flags = DX_MSG_TYPE_UNDETERMINED;
  DX_OBJECT(SELF)->type = TYPE;
  TRACE("leave: dx_msg_construct\n");
  return DX_SUCCESS;
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_EMIT_MSG_TRACE (0)

#if defined(DX_EMIT_MSG_TRACE) && 1 == DX_EMIT_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("dx.emit_msg",
                      dx_emit_msg,
                      dx_msg);

static void dx_emit_msg_destruct(dx_emit_msg* SELF) {
  TRACE("enter: dx_emit_msg_destruct\n");
  dx_memory_deallocate(SELF->p);
  SELF->p = NULL;
  TRACE("leave: dx_emit_msg_destruct\n");
}

static void dx_emit_msg_dispatch_construct(dx_emit_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_emit_msg_construct(dx_emit_msg* SELF, char const* p, dx_size n) {
  TRACE("enter: dx_emit_msg_construct\n");
  dx_rti_type* TYPE = dx_emit_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_msg_construct(DX_MSG(SELF))) {
    TRACE("leave: dx_emit_msg_construct\n");
    return DX_FAILURE;
  }
  SELF->p = NULL;
  if (dx_memory_allocate(&SELF->p, n)) {
    TRACE("leave: dx_emit_msg_construct\n");
    return DX_FAILURE;
  }
  dx_memory_copy(SELF->p, p, n);
  SELF->n = n;
  DX_MSG(SELF)->flags = DX_MSG_TYPE_EMIT;
  DX_OBJECT(SELF)->type = TYPE;
  TRACE("leave: dx_emit_msg_construct\n");
  return DX_SUCCESS;
}

dx_result dx_emit_msg_create(dx_emit_msg** RETURN, char const* p, dx_size n) {
  DX_CREATE_PREFIX(dx_emit_msg)
  if (dx_emit_msg_construct(SELF, p, n)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

int dx_emit_msg_get(dx_emit_msg* self, char const** p, dx_size* n) {
  *p = self->p;
  *n = self->n;
  return 0;
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_QUIT_MSG_TRACE (0)

#if defined(DX_QUIT_MSG_TRACE) && 1 == DX_QUIT_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("dx.quit_msg",
                      dx_quit_msg,
                      dx_msg);

static void dx_quit_msg_destruct(dx_quit_msg* SELF)
{/*Intentionally empty.*/}

static void dx_quit_msg_dispatch_construct(dx_quit_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_quit_msg_construct(dx_quit_msg* SELF) {
  TRACE("enter: dx_quit_msg_construct\n");
  dx_rti_type* TYPE = dx_quit_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_msg_construct(DX_MSG(SELF))) {
    TRACE("leave: dx_quit_msg_construct\n");
    return DX_FAILURE;
  }
  DX_MSG(SELF)->flags = DX_MSG_TYPE_QUIT;
  DX_OBJECT(SELF)->type = TYPE;
  TRACE("leave: dx_quit_msg_construct\n");
  return DX_SUCCESS;
}

dx_result dx_quit_msg_create(dx_quit_msg** RETURN) {
  DX_CREATE_PREFIX(dx_quit_msg)
  if (dx_quit_msg_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_QUEUE_TRACE (0)

#if defined(DX_MSG_QUEUE_TRACE) && 1 == DX_MSG_QUEUE_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

static void added(void** p) {
  DX_REFERENCE(*p);
}

static void removed(void** p) {
  DX_UNREFERENCE(*p);
}

typedef struct dx_msg_queue {
  dx_inline_pointer_deque deque;
} dx_msg_queue;

dx_result dx_msg_queue_push(dx_msg_queue* SELF, dx_msg* msg) {
  TRACE("enter: dx_msg_queue_push\n");
  if (!SELF || !msg) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    TRACE("leave: dx_msg_queue_push\n");
    return DX_FAILURE;
  }
  if (dx_inline_pointer_deque_push_back(&SELF->deque, msg)) {
    TRACE("leave: dx_msg_queue_push\n");
    return DX_FAILURE;
  }
  TRACE("leave: dx_msg_queue_push (success)\n");
  return DX_SUCCESS;
}

dx_result dx_msg_queue_pop(dx_msg** RETURN, dx_msg_queue* SELF) {
  TRACE("enter: dx_msg_queue_pop\n");
  dx_msg* msg = NULL;
  if (dx_inline_pointer_deque_pop_front(&msg, &SELF->deque, true)) {
    if (dx_get_error() != DX_ERROR_IS_EMPTY) {
      TRACE("leave: dx_msg_queue_pop (failure)\n");
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
      *RETURN = NULL;
      TRACE("leave: dx_msg_queue_pop (success)\n");
      return DX_SUCCESS;
    }
  }
  *RETURN = msg;
  TRACE("leave: dx_msg_queue_pop (success)\n");
  return DX_SUCCESS;
}

dx_result dx_msg_queue_create(dx_msg_queue** RETURN) {
  TRACE("enter: dx_msg_queue_create\n");
  dx_msg_queue* msg_queue = NULL;
  if (dx_memory_allocate(&msg_queue, sizeof(dx_msg_queue))) {
    dx_log("allocation failed\n", sizeof("allocation failed\n"));
    TRACE("leave: dx_msg_queue_create\n");
    return DX_FAILURE;
  }
  DX_INLINE_POINTER_DEQUE_CONFIGURATION configuration = {
    .added_callback = &added,
    .removed_callback = &removed,
  };
  if (dx_inline_pointer_deque_initialize(&msg_queue->deque, 0, &configuration)) {
    TRACE("leave: dx_msg_queue_create (failure)\n");
    return DX_FAILURE;
  }
  TRACE("leave: dx_msg_queue_create (success)\n");
  *RETURN = msg_queue;
  return DX_SUCCESS;
}

void dx_msg_queue_destroy(dx_msg_queue* msg_queue) {
  TRACE("enter: dx_msg_queue_destroy\n");
  dx_inline_pointer_deque_uninitialize(&msg_queue->deque);
  dx_memory_deallocate(msg_queue);
  msg_queue = NULL;
  TRACE("leave: dx_msg_queue_destroy\n");
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
