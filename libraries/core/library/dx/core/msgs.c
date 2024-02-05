#include "dx/core/msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Core/Time.h"
#include "Core/Memory.h"
#include "dx/core/inline_pointer_deque.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_QUEUE_TRACE (0)

#if defined(DX_MSG_QUEUE_TRACE) && 1 == DX_MSG_QUEUE_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

static void added(void** p) {
  CORE_REFERENCE(*p);
}

static void removed(void** p) {
  CORE_UNREFERENCE(*p);
}

typedef struct dx_msg_queue {
  Core_InlineArrayDequeP deque;
} dx_msg_queue;

Core_Result dx_msg_queue_push(dx_msg_queue* SELF, Core_Message* msg) {
  TRACE("enter: dx_msg_queue_push\n");
  if (!SELF || !msg) {
    Core_setError(Core_Error_ArgumentInvalid);
    TRACE("leave: dx_msg_queue_push\n");
    return Core_Failure;
  }
  if (Core_InlineArrayDequeP_push_back(&SELF->deque, msg)) {
    TRACE("leave: dx_msg_queue_push\n");
    return Core_Failure;
  }
  TRACE("leave: dx_msg_queue_push (success)\n");
  return Core_Success;
}

Core_Result dx_msg_queue_pop(Core_Message** RETURN, dx_msg_queue* SELF) {
  TRACE("enter: dx_msg_queue_pop\n");
  Core_Message* msg = NULL;
  if (Core_InlineArrayDequeP_pop_front(&msg, &SELF->deque, true)) {
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
  Core_InlineArrayDequeP_Configuration configuration = {
    .added_callback = &added,
    .removed_callback = &removed,
  };
  if (Core_InlineArrayDequeP_initialize(&msg_queue->deque, 0, &configuration)) {
    TRACE("leave: dx_msg_queue_create (failure)\n");
    return Core_Failure;
  }
  TRACE("leave: dx_msg_queue_create (success)\n");
  *RETURN = msg_queue;
  return Core_Success;
}

void dx_msg_queue_destroy(dx_msg_queue* msg_queue) {
  TRACE("enter: dx_msg_queue_destroy\n");
  Core_InlineArrayDequeP_uninitialize(&msg_queue->deque);
  Core_Memory_deallocate(msg_queue);
  msg_queue = NULL;
  TRACE("leave: dx_msg_queue_destroy\n");
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
