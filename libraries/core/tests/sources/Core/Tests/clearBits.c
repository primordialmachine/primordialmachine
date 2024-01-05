#include "Core/Tests/clearBits.h"

static Core_Result testClearBitsN8() {
  Core_Natural8 v, u;

  v = 0xff;

  //
  for (Core_Size i = 0; i < 8; ++i) {
    if (Core_clearBitsN8(&u, v, i, 0)) {
      return Core_Failure;
    }
    if (u != v) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  //
  if (!Core_clearBitsN8(&u, v, 8, 0)) {
    return Core_Failure;
  }
  if (Core_getError() != Core_Error_ArgumentInvalid) {
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);

  //
  for (Core_Size i = 0; i < 8; ++i) {
    v = 0xff;
    if (Core_clearBitsN8(&u, v, i, 1)) {
      return Core_Failure;
    }
    Core_Natural8 a = UINT8_C(1) << (8 - i - 1);
    if (u != v - a) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }

  return Core_Success;
}

static Core_Result testClearBitsN16() {
  Core_Natural16 v, u;

  v = 0xffff;

  //
  for (Core_Size i = 0; i < 16; ++i) {
    if (Core_clearBitsN16(&u, v, i, 0)) {
      return Core_Failure;
    }
    if (u != v) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  //
  if (!Core_clearBitsN16(&u, v, 16, 0)) {
    return Core_Failure;
  }
  if (Core_getError() != Core_Error_ArgumentInvalid) {
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);

  //
  for (Core_Size i = 0; i < 16; ++i) {
    v = 0xffff;
    if (Core_clearBitsN16(&u, v, i, 1)) {
      return Core_Failure;
    }
    Core_Natural16 a = UINT16_C(1) << (16 - i - 1);
    if (u != v - a) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }

  return Core_Success;
}

static Core_Result testClearBitsN32() {
  Core_Natural32 v, u;

  v = 0xffffffff;

  //
  for (Core_Size i = 0; i < 32; ++i) {
    if (Core_clearBitsN32(&u, v, i, 0)) {
      return Core_Failure;
    }
    if (u != v) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  //
  if (!Core_clearBitsN32(&u, v, 32, 0)) {
    return Core_Failure;
  }
  if (Core_getError() != Core_Error_ArgumentInvalid) {
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);

  //
  for (Core_Size i = 0; i < 32; ++i) {
    v = 0xffffffff;
    if (Core_clearBitsN32(&u, v, i, 1)) {
      return Core_Failure;
    }
    Core_Natural32 a = UINT32_C(1) << (32 - i - 1);
    if (u != v - a) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }

  return Core_Success;
}

static Core_Result testClearBitsN64() {
  Core_Natural64 v, u;

  v = 0xffffffffffffffff;

  //
  for (Core_Size i = 0; i < 64; ++i) {
    if (Core_clearBitsN64(&u, v, i, 0)) {
      return Core_Failure;
    }
    if (u != v) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  //
  if (!Core_clearBitsN64(&u, v, 64, 0)) {
    return Core_Failure;
  }
  if (Core_getError() != Core_Error_ArgumentInvalid) {
    return Core_Failure;
  }
  Core_setError(Core_Error_NoError);

  //
  for (Core_Size i = 0; i < 64; ++i) {
    v = 0xffffffffffffffff;
    if (Core_clearBitsN64(&u, v, i, 1)) {
      return Core_Failure;
    }
    Core_Natural64 a = UINT64_C(1) << (64 - i - 1);
    if (u != v - a) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }

  return Core_Success;
}

Core_Result Core_Tests_clearBitsTests() {
  if (testClearBitsN8()) {
    return Core_Failure;
  }
  if (testClearBitsN16()) {
    return Core_Failure;
  }
  if (testClearBitsN32()) {
    return Core_Failure;
  }
  if (testClearBitsN64()) {
    return Core_Failure;
  }
  return Core_Success;
}
