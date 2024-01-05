#include <stdlib.h>

#include "Core/Tests/clearBits.h"
#include "Core/Tests/countLeadingZeroes.h"
#include "Core/Tests/fundamentalTypes.h"
#include "Core/Tests/nextPowerOfTwo.h"
#include "Core/Tests/safeAddNx.h"
#include "Core/Tests/safeMulNx.h"

#include "Core/Tests/Signals/Test1.h"

static Core_Result Core_Tests_applicationMsgsTests() {
  Core_ApplicationMessage* msg = NULL;
  //
  if (Core_ApplicationMessage_create(&msg, Core_ApplicationMessageKind_QuitRequested)) {
    return Core_Failure;
  }
  //
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  return Core_Success;
}

static Core_Result Core_Tests_mouseButtonMsgTests() {
  dx_mouse_button_msg* msg = NULL;
  //
  if (dx_mouse_button_msg_create(&msg, DX_MOUSE_BUTTON_ACTION_PRESSED, Core_MouseButton_Button0, 0, 0, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  if (dx_mouse_button_msg_create(&msg, DX_MOUSE_BUTTON_ACTION_RELEASED, Core_MouseButton_Button0, 0, 0, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  return Core_Success;
}

static Core_Result Core_Tests_mousePointerMsgTests() {
  dx_mouse_pointer_msg* msg = NULL;
  //
  if (dx_mouse_pointer_msg_create(&msg, DX_MOUSE_POINTER_ACTION_MOVED, 0, 0, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  if (dx_mouse_pointer_msg_create(&msg, DX_MOUSE_POINTER_ACTION_ENTERED, 0, 0, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  if (dx_mouse_pointer_msg_create(&msg, DX_MOUSE_POINTER_ACTION_EXITED, 0, 0, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  return Core_Success;
}

static Core_Result Core_Tests_keyboardKeyMsgTests() {
  dx_keyboard_key_msg* msg = NULL;
  //
  if (dx_keyboard_key_msg_create(&msg, Core_KeyboardKeyAction_Pressed, Core_KeyboardKey_A, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  if (dx_keyboard_key_msg_create(&msg, Core_KeyboardKeyAction_Released, Core_KeyboardKey_A, 0)) {
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  //
  return Core_Success;
}

#include <stdio.h>

static Core_Result run_tests() {
  if (Core_Tests_clearBitsTests()) {
    fprintf(stdout, "Core.Tests.clearBitsTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.clearBitsTests succeeded\n");
  
  if (Core_Tests_countLeadingZeroesTests()) {
    fprintf(stdout, "Core.Tests.countLeadingZeroesTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.countLeadingZeroesTests succeeded\n");
  
  if (Core_Tests_fundamentalTypesTests()) {
    fprintf(stdout, "Core.Tests.fundamentalTypesTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.fundamentalTypesTests succeeded\n");
  
  if (Core_Tests_applicationMsgsTests()) {
    fprintf(stdout, "Core.Tests.applicationMsgsTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.applicationMsgsTests succeeded\n");
  
  if (Core_Tests_mouseButtonMsgTests()) {
    fprintf(stdout, "Core.Tests.mouseButtonMsgTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.mouseButtonMsgTests succeeded\n");

  if (Core_Tests_mousePointerMsgTests()) {
    fprintf(stdout, "Core.Tests.mousePointerMsgTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.mousePointerMsgTests succeeded\n");
  
  if (Core_Tests_keyboardKeyMsgTests()) {
    fprintf(stdout, "Core.Tests.keyboardKeyMsgTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.keyboardKeyMsgTests succeeded\n");
  
  if (Core_Tests_nextPowerOfTwoTests()) {
    fprintf(stdout, "Core.Tests.nextPowerOfTwoTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.nextPowerOfTwoTests succeeded\n");

  if (Core_Tests_safeAddNxTests()) {
    fprintf(stdout, "Core.Tests.safeAddNxTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.safeAddNxTests succeded\n");

  if (Core_Tests_safeMulNxTests()) {
    fprintf(stdout, "Core.Tests.safeMulNxTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.safeMulNxTests succeded\n");

#if defined(DX_SAFE_MUL_IX_WITH_TESTS) && 1 == DX_SAFE_MUL_IX_WITH_TESTS
  if (dx_safe_mul_ix_tests()) {
    fprintf(stdout, "Core.Tests.safeMulIxTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.safeMulIxTests succeded\n");
#endif
#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
  if (dx_math_tests()) {
    fprintf(stdout, "Core.Tests.mathTests failed\n");
    return Core_Failure;
  }
  fprintf(stdout, "Core.Tests.mathTests succeded\n");
#endif
  return Core_Success;
}

static Core_Result level1Tests() {
  if (Core_initialize()) {
    return Core_Failure;
  }
  if (run_tests()) {
    Core_uninitialize();
    return Core_Failure;
  }
  Core_uninitialize();
  return Core_Success;
}

int main(int argc, char **argv) {
  if (level1Tests()) {
    return EXIT_FAILURE;
  }
  if (Core_Tests_Signals_test()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}