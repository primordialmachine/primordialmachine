#include "dx/val/system.h"

DX_DEFINE_OBJECT_TYPE("dx.val.system",
                      dx_val_system,
                      dx_system);

static void dx_val_system_destruct(dx_val_system* SELF) {
  DX_UNREFERENCE(SELF->mouse_state);
  SELF->mouse_state = NULL;
  DX_UNREFERENCE(SELF->keyboard_state);
  SELF->keyboard_state = NULL;
}

static void dx_val_system_constructDispatch(dx_val_system_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_system_construct(dx_val_system* SELF, dx_msg_queue* msg_queue) {
  DX_CONSTRUCT_PREFIX(dx_val_system);
  SELF->msg_queue = msg_queue;
  if (dx_system_construct(DX_SYSTEM(SELF))) {
    return Core_Failure;
  }
  if (dx_keyboard_state_create(&SELF->keyboard_state)) {
    return Core_Failure;
  }
  if (dx_mouse_state_create(&SELF->mouse_state)) {
    DX_UNREFERENCE(SELF->keyboard_state);
    SELF->keyboard_state = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_system_emit_keyboard_key_pressed_msg(dx_val_system* SELF, dx_keyboard_key key, uint8_t modifiers) {
  Core_Message* msg = NULL;
  if (dx_keyboard_key_msg_create((dx_keyboard_key_msg**)&msg, DX_KEYBOARD_KEY_ACTION_PRESSED, key, modifiers)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_keyboard_key_released_msg(dx_val_system* SELF, dx_keyboard_key key, uint8_t modifiers) {
  Core_Message* msg = NULL;
  if (dx_keyboard_key_msg_create((dx_keyboard_key_msg**)&msg, DX_KEYBOARD_KEY_ACTION_RELEASED, key, modifiers)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_mouse_button_pressed_msg(dx_val_system* SELF, dx_mouse_button button, Core_Real32 x, Core_Real32 y, uint8_t modifiers) {
  Core_Message* msg = NULL;
  if (dx_mouse_button_msg_create((dx_mouse_button_msg**)&msg, DX_MOUSE_BUTTON_ACTION_PRESSED, button, modifiers, x, y)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_mouse_button_released_msg(dx_val_system* SELF, dx_mouse_button button, Core_Real32 x, Core_Real32 y, uint8_t modifiers) {
  Core_Message* msg = NULL;
  if (dx_mouse_button_msg_create((dx_mouse_button_msg**)&msg, DX_MOUSE_BUTTON_ACTION_RELEASED, button, modifiers, x, y)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_mouse_pointer_moved_msg(dx_val_system* SELF, Core_Real32 x, Core_Real32 y, uint8_t modifiers) {
  Core_Message* msg = NULL;
  if (dx_mouse_pointer_msg_create((dx_mouse_pointer_msg**)&msg, DX_MOUSE_POINTER_ACTION_MOVED, modifiers, x, y)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_canvas_size_changed_msg(dx_val_system* SELF, Core_Real32 width, Core_Real32 height) {
  Core_Message* msg = NULL;
  if (dx_canvas_size_changed_msg_create((dx_canvas_size_changed_msg**)&msg, width, height)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_canvas_activated_msg(dx_val_system* SELF) {
  Core_Message* msg = NULL;
  if (dx_canvas_msg_create((dx_canvas_msg**)&msg, dx_canvas_msg_kind_activated)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_canvas_deactivated_msg(dx_val_system* SELF) {
  Core_Message* msg = NULL;
  if (dx_canvas_msg_create((dx_canvas_msg**)&msg, dx_canvas_msg_kind_deactivated)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}
