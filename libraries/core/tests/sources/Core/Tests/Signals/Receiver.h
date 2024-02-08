#if !defined(CORE_TESTS_SIGNALS_RECEIVER_H_INCLUDED)
#define CORE_TESTS_SIGNALS_RECEIVER_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("Core.Tests.Signals.Receiver",
                       Core_Tests_Signals_Receiver,
                       Core_Object);

static inline Core_Tests_Signals_Receiver* CORE_TESTS_SIGNALS_RECEIVER(void* p) {
  return (Core_Tests_Signals_Receiver*)p;
}

struct Core_Tests_Signals_Receiver {
  Core_Object _parent;
  Core_String* message;
};

static inline Core_Tests_Signals_Receiver_Dispatch* CORE_TESTS_SIGNALS_RECEIVER_DISPATCH(void* p) {
  return (Core_Tests_Signals_Receiver_Dispatch*)p;
}

struct Core_Tests_Signals_Receiver_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*onReceiveMessage)(Core_Tests_Signals_Receiver* SELF, Core_String* message);
  Core_Result(*getLastMessage)(Core_String** RETURN, Core_Tests_Signals_Receiver* SELF);
};

Core_Result Core_Tests_Signals_Receiver_construct(Core_Tests_Signals_Receiver* SELF);

Core_Result Core_Tests_Signals_Receiver_create(Core_Tests_Signals_Receiver** RETURN);

static inline Core_Result Core_Tests_Signals_Receiver_onReceiveMessage(Core_Tests_Signals_Receiver* SELF, Core_String* message) {
  DX_OBJECT_VIRTUALCALL(Core_Tests_Signals_Receiver, onReceiveMessage, SELF, message);
}

static inline Core_Result Core_Tests_Signals_Receiver_getLastMessage(Core_String** RETURN, Core_Tests_Signals_Receiver* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Tests_Signals_Receiver, getLastMessage, RETURN, SELF);
}

#endif // CORE_TESTS_SIGNALS_RECEIVER_H_INCLUDED
