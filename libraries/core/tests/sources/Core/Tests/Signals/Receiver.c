#include "Core/Tests/Signals/Receiver.h"

Core_defineObjectType("Core.Tests.Signals.Receiver",
                      Core_Tests_Signals_Receiver,
                      Core_Object);

static inline Core_Result onReceiveMessage(Core_Tests_Signals_Receiver* SELF, Core_String* message);

static inline Core_Result getLastMessage(Core_String** RETRUN, Core_Tests_Signals_Receiver* SELF);

static void Core_Tests_Signals_Receiver_destruct(Core_Tests_Signals_Receiver* SELF)
{/*Intentionally empty.*/}

static void Core_Tests_Signals_Receiver_constructDispatch(Core_Tests_Signals_Receiver_Dispatch* SELF) {
  SELF->onReceiveMessage = &onReceiveMessage;
  SELF->getLastMessage = &getLastMessage;
}

Core_Result Core_Tests_Signals_Receiver_construct(Core_Tests_Signals_Receiver* SELF) {
  Core_BeginConstructor(Core_Tests_Signals_Receiver);
  SELF->message = NULL;
  Core_EndConstructor(Core_Tests_Signals_Receiver);
}

Core_Result Core_Tests_Signals_Receiver_create(Core_Tests_Signals_Receiver** RETURN) {
  DX_CREATE_PREFIX(Core_Tests_Signals_Receiver);
  if (Core_Tests_Signals_Receiver_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static inline Core_Result onReceiveMessage(Core_Tests_Signals_Receiver* SELF, Core_String* message) {
  if (message) {
    CORE_REFERENCE(message);
  }
  if (SELF->message) {
    CORE_UNREFERENCE(SELF->message);
  }
  SELF->message = message;
  return Core_Success;
}

static inline Core_Result getLastMessage(Core_String** RETURN, Core_Tests_Signals_Receiver* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Tests_Signals_Receiver, getLastMessage, RETURN, SELF);
}
