#include "Core/Tests/Signals/Test1.h"

#include "Core/Tests/Signals/ObjectA.h"
#include "Core/Tests/Signals/ObjectB.h"
#include "Core/Tests/Signals/ObjectC.h"
#include "Core/Tests/Signals/Receiver.h"
#include <string.h>

/* add same signal twice to type A */
static Core_Result Core_Tests_Signals_test1() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (Core_Tests_Signals_ObjectA_getType(&type)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_add(type, "mySignal1", strlen("mySignal1"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (!Core_Signals_add(type, "mySignal1", strlen("mySignal1"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Error_Exists != Core_getError()) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);
  Core_uninitialize();
  return Core_Success;
}

/* add same signal first to type A then to type B */
static Core_Result Core_Tests_Signals_test2() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  Core_Type* type1 = NULL;
  Core_Type* type2 = NULL;
  if (Core_Tests_Signals_ObjectA_getType(&type1)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Tests_Signals_ObjectB_getType(&type2)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_add(type1, "mySignal1", strlen("mySignal1"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (!Core_Signals_add(type2, "mySignal1", strlen("mySignal1"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Error_Exists != Core_getError()) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);
  Core_uninitialize();
  return Core_Success;
}

/* add same signal first to type A then to type C */
static Core_Result Core_Tests_Signals_test3() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  Core_Type* type1 = NULL;
  Core_Type* type2 = NULL;
  if (Core_Tests_Signals_ObjectA_getType(&type1)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Tests_Signals_ObjectC_getType(&type2)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_add(type1, "mySignal1", strlen("mySignal1"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (!Core_Signals_add(type2, "mySignal1", strlen("mySignal1"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Error_Exists != Core_getError()) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);
  Core_uninitialize();
  return Core_Success;
}

/* instantiate object with signal */
static Core_Result Core_Tests_Signals_test4() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (Core_Tests_Signals_ObjectC_getType(&type)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_add(type, "mySignal", strlen("mySignal"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_Tests_Signals_ObjectC* sender = NULL;
  Core_Tests_Signals_Receiver* receiver = NULL;
  if (Core_Tests_Signals_ObjectC_create(&sender)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Tests_Signals_Receiver_create(&receiver)) {
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
  CORE_UNREFERENCE(receiver);
  receiver = NULL;
  CORE_UNREFERENCE(sender);
  sender = NULL;
  Core_uninitialize();
  return Core_Success;
}

/* instantiate object with signal */
/* connect to signal of instantiated object */
static Core_Result Core_Tests_Signals_test5() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (Core_Tests_Signals_ObjectC_getType(&type)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_add(type, "mySignal", strlen("mySignal"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_Tests_Signals_ObjectC* sender = NULL;
  Core_Tests_Signals_Receiver* receiver = NULL;
  if (Core_Tests_Signals_ObjectC_create(&sender)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Tests_Signals_Receiver_create(&receiver)) {
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
  void* id = NULL;
  if (Core_Signals_connect(&id, CORE_OBJECT(sender), "mySignal", strlen("mySignal"), CORE_OBJECT(receiver), (Core_Callback*)&Core_Tests_Signals_Receiver_onReceiveMessage)) {
    CORE_UNREFERENCE(receiver);
    receiver = NULL;
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
  CORE_UNREFERENCE(receiver);
  receiver = NULL;
  CORE_UNREFERENCE(sender);
  sender = NULL;
  Core_uninitialize();
  return Core_Success;
}

/* instantiate object with signal */
/* connect to signal of instantiated object */
/* invoke signal from instantiated object */
static Core_Result Core_Tests_Signals_test6() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (Core_Tests_Signals_ObjectC_getType(&type)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_add(type, "mySignal", strlen("mySignal"))) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_Tests_Signals_ObjectC* sender = NULL;
  Core_Tests_Signals_Receiver* receiver = NULL;
  if (Core_Tests_Signals_ObjectC_create(&sender)) {
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Tests_Signals_Receiver_create(&receiver)) {
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
  void* id = NULL;
  if (Core_Signals_connect(&id, CORE_OBJECT(sender), "mySignal", strlen("mySignal"), CORE_OBJECT(receiver), (Core_Callback*)&Core_Tests_Signals_Receiver_onReceiveMessage)) {
    CORE_UNREFERENCE(receiver);
    receiver = NULL;
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
#if 1
  Core_String* message = NULL;
  if (Core_String_create(&message, "1", strlen("1"))) {
    CORE_UNREFERENCE(receiver);
    receiver = NULL;
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
  if (Core_Signals_invoke(CORE_OBJECT(sender), "mySignal", strlen("mySignal"), CORE_OBJECT(message))) {
    CORE_UNREFERENCE(message);
    message = NULL;
    CORE_UNREFERENCE(receiver);
    receiver = NULL;
    CORE_UNREFERENCE(sender);
    sender = NULL;
    Core_uninitialize();
    return Core_Failure;
  }
  CORE_UNREFERENCE(message);
  message = NULL;
#endif
  CORE_UNREFERENCE(receiver);
  receiver = NULL;
  CORE_UNREFERENCE(sender);
  sender = NULL;
  Core_uninitialize();
  return Core_Success;
}


Core_Result Core_Tests_Signals_test() {
#if 1
  if (Core_Tests_Signals_test1()) {
    return Core_Failure;
  }
#endif
#if 1
  if (Core_Tests_Signals_test2()) {
    return Core_Failure;
  }
#endif
#if 1
  if (Core_Tests_Signals_test3()) {
    return Core_Failure;
  }
#endif
#if 1
  if (Core_Tests_Signals_test4()) {
    return Core_Failure;
  }
#endif
#if 1
  if (Core_Tests_Signals_test5()) {
    return Core_Failure;
  }
#endif
#if 1
  if (Core_Tests_Signals_test6()) {
    return Core_Failure;
  }
#endif
  return Core_Success;
}
