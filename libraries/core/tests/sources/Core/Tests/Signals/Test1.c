#include "Core/Tests/Signals/Test1.h"

#include "Core/Tests/Signals/ObjectA.h"
#include "Core/Tests/Signals/ObjectB.h"
#include "Core/Tests/Signals/ObjectC.h"
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

Core_Result Core_Tests_Signals_test() {
  if (Core_Tests_Signals_test1()) {
    return Core_Failure;
  }
  if (Core_Tests_Signals_test2()) {
    return Core_Failure;
  }
  if (Core_Tests_Signals_test3()) {
    return Core_Failure;
  }
  return Core_Success;
}
