#include "dx/core/msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Core/Memory.h"
#include "dx/core/inline_pointer_deque.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_TRACE (0)

#if defined(DX_MSG_TRACE) && 1 == DX_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("Core.Message",
                      Core_Message,
                      Core_Object);

Core_Result Core_Message_getFlags(uint32_t* RETURN, Core_Message const* SELF) {
  *RETURN = SELF->flags;
  return Core_Success;
}

static void Core_Message_destruct(Core_Message* SELF)
{/*Intentionally empty.*/}

static void Core_Message_constructDispatch(Core_Message_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Message_construct(Core_Message* SELF) {
  TRACE("enter: dx_msg_construct\n");
  DX_CONSTRUCT_PREFIX(Core_Message);
  SELF->flags = DX_MSG_TYPE_UNDETERMINED;
  CORE_OBJECT(SELF)->type = TYPE;
  TRACE("leave: dx_msg_construct\n");
  return Core_Success;
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
                      Core_Message);

static void dx_emit_msg_destruct(dx_emit_msg* SELF) {
  TRACE("enter: dx_emit_msg_destruct\n");
  Core_Memory_deallocate(SELF->p);
  SELF->p = NULL;
  TRACE("leave: dx_emit_msg_destruct\n");
}

static void dx_emit_msg_constructDispatch(dx_emit_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_emit_msg_construct(dx_emit_msg* SELF, char const* p, Core_Size n) {
  TRACE("enter: dx_emit_msg_construct\n");
  DX_CONSTRUCT_PREFIX(dx_emit_msg);
  if (Core_Message_construct(CORE_MESSAGE(SELF))) {
    TRACE("leave: dx_emit_msg_construct\n");
    return Core_Failure;
  }
  SELF->p = NULL;
  if (Core_Memory_allocate(&SELF->p, n)) {
    TRACE("leave: dx_emit_msg_construct\n");
    return Core_Failure;
  }
  Core_Memory_copy(SELF->p, p, n);
  SELF->n = n;
  CORE_MESSAGE(SELF)->flags = DX_MSG_TYPE_EMIT;
  CORE_OBJECT(SELF)->type = TYPE;
  TRACE("leave: dx_emit_msg_construct\n");
  return Core_Success;
}

Core_Result dx_emit_msg_create(dx_emit_msg** RETURN, char const* p, Core_Size n) {
  DX_CREATE_PREFIX(dx_emit_msg);
  if (dx_emit_msg_construct(SELF, p, n)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_emit_msg_get(dx_emit_msg* SELF, char const** p, Core_Size* n) {
  *p = SELF->p;
  *n = SELF->n;
  return Core_Success;
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
                      Core_Message);

static void dx_quit_msg_destruct(dx_quit_msg* SELF)
{/*Intentionally empty.*/}

static void dx_quit_msg_constructDispatch(dx_quit_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_quit_msg_construct(dx_quit_msg* SELF) {
  TRACE("enter: dx_quit_msg_construct\n");
  DX_CONSTRUCT_PREFIX(dx_quit_msg);
  if (Core_Message_construct(CORE_MESSAGE(SELF))) {
    TRACE("leave: dx_quit_msg_construct\n");
    return Core_Failure;
  }
  CORE_MESSAGE(SELF)->flags = DX_MSG_TYPE_QUIT;
  CORE_OBJECT(SELF)->type = TYPE;
  TRACE("leave: dx_quit_msg_construct\n");
  return Core_Success;
}

Core_Result dx_quit_msg_create(dx_quit_msg** RETURN) {
  DX_CREATE_PREFIX(dx_quit_msg);
  if (dx_quit_msg_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
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

Core_Result dx_msg_queue_push(dx_msg_queue* SELF, Core_Message* msg) {
  TRACE("enter: dx_msg_queue_push\n");
  if (!SELF || !msg) {
    Core_setError(Core_Error_ArgumentInvalid);
    TRACE("leave: dx_msg_queue_push\n");
    return Core_Failure;
  }
  if (dx_inline_pointer_deque_push_back(&SELF->deque, msg)) {
    TRACE("leave: dx_msg_queue_push\n");
    return Core_Failure;
  }
  TRACE("leave: dx_msg_queue_push (success)\n");
  return Core_Success;
}

Core_Result dx_msg_queue_pop(Core_Message** RETURN, dx_msg_queue* SELF) {
  TRACE("enter: dx_msg_queue_pop\n");
  Core_Message* msg = NULL;
  if (dx_inline_pointer_deque_pop_front(&msg, &SELF->deque, true)) {
    if (Core_Error_Empty != Core_getError()) {
      TRACE("leave: dx_msg_queue_pop (failure)\n");
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
      *RETURN = NULL;
      TRACE("leave: dx_msg_queue_pop (success)\n");
      return Core_Success;
    }
  }
  *RETURN = msg;
  TRACE("leave: dx_msg_queue_pop (success)\n");
  return Core_Success;
}

Core_Result dx_msg_queue_create(dx_msg_queue** RETURN) {
  TRACE("enter: dx_msg_queue_create\n");
  dx_msg_queue* msg_queue = NULL;
  if (Core_Memory_allocate(&msg_queue, sizeof(dx_msg_queue))) {
    dx_log("allocation failed\n", sizeof("allocation failed\n"));
    TRACE("leave: dx_msg_queue_create\n");
    return Core_Failure;
  }
  DX_INLINE_POINTER_DEQUE_CONFIGURATION configuration = {
    .added_callback = &added,
    .removed_callback = &removed,
  };
  if (dx_inline_pointer_deque_initialize(&msg_queue->deque, 0, &configuration)) {
    TRACE("leave: dx_msg_queue_create (failure)\n");
    return Core_Failure;
  }
  TRACE("leave: dx_msg_queue_create (success)\n");
  *RETURN = msg_queue;
  return Core_Success;
}

void dx_msg_queue_destroy(dx_msg_queue* msg_queue) {
  TRACE("enter: dx_msg_queue_destroy\n");
  dx_inline_pointer_deque_uninitialize(&msg_queue->deque);
  Core_Memory_deallocate(msg_queue);
  msg_queue = NULL;
  TRACE("leave: dx_msg_queue_destroy\n");
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
