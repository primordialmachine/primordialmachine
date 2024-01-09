#include "dx/val/system.h"

Core_defineObjectType("dx.val.system",
                      dx_val_system,
                      dx_system);

static void dx_val_system_destruct(dx_val_system* SELF) {
  CORE_UNREFERENCE(SELF->mouse_state);
  SELF->mouse_state = NULL;
  CORE_UNREFERENCE(SELF->keyboard_state);
  SELF->keyboard_state = NULL;
}

static void dx_val_system_constructDispatch(dx_val_system_Dispatch* SELF)
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
    CORE_UNREFERENCE(SELF->keyboard_state);
    SELF->keyboard_state = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_system_emit_keyboard_key_pressed_msg(dx_val_system* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_KeyboardKeyMessage_create((Core_KeyboardKeyMessage**)&message, Core_KeyboardKeyAction_Pressed, key, modifierKeys)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_keyboard_key_released_msg(dx_val_system* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_KeyboardKeyMessage_create((Core_KeyboardKeyMessage**)&message, Core_KeyboardKeyAction_Released, key, modifierKeys)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_mouse_button_pressed_msg(dx_val_system* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_MouseButtonMessage_create((Core_MouseButtonMessage**)&message, Core_MouseButtonAction_Pressed, button, modifierKeys, x, y)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_mouse_button_released_msg(dx_val_system* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_MouseButtonMessage_create((Core_MouseButtonMessage**)&message, Core_MouseButtonAction_Released, button, modifierKeys, x, y)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_mouse_pointer_moved_msg(dx_val_system* SELF, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_MousePointerMessage_create((Core_MousePointerMessage**)&message, Core_MousePointerAction_Moved, x, y, modifierKeys)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_canvas_size_changed_msg(dx_val_system* SELF, Core_Real32 horizontalSize, Core_Real32 verticalSize) {
  Core_Message* message = NULL;
  if (Core_CanvasSizeChangedMessage_create((Core_CanvasSizeChangedMessage**)&message, horizontalSize, verticalSize)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_canvas_activated_msg(dx_val_system* SELF) {
  Core_Message* message = NULL;
  if (Core_CanvasStateChangedMessage_create((Core_CanvasStateChangedMessage**)&message, Core_True)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result dx_val_system_emit_canvas_deactivated_msg(dx_val_system* SELF) {
  Core_Message* message = NULL;
  if (Core_CanvasStateChangedMessage_create((Core_CanvasStateChangedMessage**)&message, Core_False)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}
