#include "dx/val/system.h"

Core_defineObjectType("Core.Visuals.System",
                      Core_Visuals_System,
                      Core_System);

static void Core_Visuals_System_destruct(Core_Visuals_System* SELF) {
  CORE_UNREFERENCE(SELF->mouseState);
  SELF->mouseState = NULL;
  CORE_UNREFERENCE(SELF->keyboardState);
  SELF->keyboardState = NULL;
  CORE_UNREFERENCE(SELF->messageQueue);
  SELF->messageQueue = NULL;
}

static void Core_Visuals_System_constructDispatch(Core_Visuals_System_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Visuals_System_construct(Core_Visuals_System* SELF, Core_MessageQueue* messageQueue) {
  Core_BeginConstructor(Core_Visuals_System);
  SELF->messageQueue = messageQueue;
  CORE_REFERENCE(SELF->messageQueue);
  if (Core_System_construct(CORE_SYSTEM(SELF))) {
    CORE_UNREFERENCE(SELF->messageQueue);
    SELF->messageQueue = NULL;
    return Core_Failure;
  }
  if (Core_KeyboardState_create(&SELF->keyboardState)) {
    CORE_UNREFERENCE(SELF->messageQueue);
    SELF->messageQueue = NULL;
    return Core_Failure;
  }
  if (Core_MouseState_create(&SELF->mouseState)) {
    CORE_UNREFERENCE(SELF->keyboardState);
    SELF->keyboardState = NULL;
    CORE_UNREFERENCE(SELF->messageQueue);
    SELF->messageQueue = NULL;
    return Core_Failure;
  }
  Core_EndConstructor(Core_Visuals_System);
}

Core_Result Core_Visuals_System_emitKeyboardKeyPressedMessage(Core_Visuals_System* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_KeyboardKeyMessage_create((Core_KeyboardKeyMessage**)&message, Core_KeyboardKeyAction_Pressed, key, modifierKeys)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitKeyboardKeyReleasedMessage(Core_Visuals_System* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_KeyboardKeyMessage_create((Core_KeyboardKeyMessage**)&message, Core_KeyboardKeyAction_Released, key, modifierKeys)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitMouseButtonPressedMessage(Core_Visuals_System* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_MouseButtonMessage_create((Core_MouseButtonMessage**)&message, Core_MouseButtonAction_Pressed, button, modifierKeys, x, y)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitMouseButtonReleasedMessage(Core_Visuals_System* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_MouseButtonMessage_create((Core_MouseButtonMessage**)&message, Core_MouseButtonAction_Released, button, modifierKeys, x, y)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitMousePointerMovedMessage(Core_Visuals_System* SELF, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys) {
  Core_Message* message = NULL;
  if (Core_MousePointerMessage_create((Core_MousePointerMessage**)&message, Core_MousePointerAction_Moved, x, y, modifierKeys)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitCanvasSizeChangedMessage(Core_Visuals_System* SELF, Core_Real32 horizontalSize, Core_Real32 verticalSize) {
  Core_Message* message = NULL;
  if (Core_CanvasSizeChangedMessage_create((Core_CanvasSizeChangedMessage**)&message, horizontalSize, verticalSize)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitCanvasActivatedMessage(Core_Visuals_System* SELF) {
  Core_Message* message = NULL;
  if (Core_CanvasStateChangedMessage_create((Core_CanvasStateChangedMessage**)&message, Core_True)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}

Core_Result Core_Visuals_System_emitCanvasDeactivatedMessage(Core_Visuals_System* SELF) {
  Core_Message* message = NULL;
  if (Core_CanvasStateChangedMessage_create((Core_CanvasStateChangedMessage**)&message, Core_False)) {
    return Core_Failure;
  }
  if (Core_MessageQueue_push(SELF->messageQueue, message)) {
    CORE_UNREFERENCE(message);
    message = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
  return Core_Success;
}
