#include "Core/EmitMessage.h"

#include "Core/Memory.h"
#include "Core/Time.h"

Core_defineObjectType("Core.EmitMessage",
                      Core_EmitMessage,
                      Core_Message);

static void Core_EmitMessage_destruct(Core_EmitMessage* SELF) {
  CORE_UNREFERENCE(SELF->message);
  SELF->message = NULL;
}

static void Core_EmitMessage_constructDispatch(Core_EmitMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_EmitMessage_construct(Core_EmitMessage* SELF, Core_String* message) {
  DX_CONSTRUCT_PREFIX(Core_EmitMessage);
  Core_Natural64 timeStamp;
  if (Core_getNow(&timeStamp)) {
    return Core_Failure;
  }
  if (Core_Message_construct(CORE_MESSAGE(SELF), timeStamp)) {
    return Core_Failure;
  }
  SELF->message = message;
  CORE_REFERENCE(SELF->message);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_EmitMessage_create(Core_EmitMessage** RETURN, Core_String* message) {
  DX_CREATE_PREFIX(Core_EmitMessage);
  if (Core_EmitMessage_construct(SELF, message)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_EmitMessage_get(Core_String** RETURN, Core_EmitMessage* SELF) {
  CORE_REFERENCE(SELF->message);
  *RETURN = SELF->message;
  return Core_Success;
}
