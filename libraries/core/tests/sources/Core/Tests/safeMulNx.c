#include "Core/Tests/safeMulNx.h"

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural8 u, v; \
  if (Core_safeMulN8(&u, A, B, &v)) { \
    return Core_Failure; \
  } \
  if (u != U) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
  if (v != V) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
}

static Core_Result safeMulN8Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(Core_Natural8_Greatest, 0, 0, 0);
  // 0 * MAX = (0, 0)
  DEFINE(0, Core_Natural8_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Natural8_Greatest, 1, Core_Natural8_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Natural8_Greatest, Core_Natural8_Greatest, 0);
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural8 lower, upper;
    Core_Natural8 u, v;
    // The lower Core_Natural8 value.
    // All bits are one except for the LSB.
    lower = Core_Natural8_Greatest;
    Core_Natural8 mask;
    Core_makeBitMaskN8(&mask, 7, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural8 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = (lower, upper)
    if (Core_safeMulN8(&u, Core_Natural8_Greatest, 2, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural8 lower, upper;
    Core_Natural8 u, v;
    // The lower Core_Natural8 value.
    // All bits are one except for the LSB.
    lower = Core_Natural8_Greatest;
    Core_Natural8 mask;
    Core_makeBitMaskN8(&mask, 7, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural8 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // 2 * MAX = (lower, upper))
    if (Core_safeMulN8(&u, 2, Core_Natural8_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural8 upper, lower;
    Core_Natural8 u, v;
    // The lower Core_Natural8 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural8_Greatest;
    Core_Natural8 mask;
    Core_makeBitMaskN8(&mask, 6, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural16 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = (lower, upper)
    if (Core_safeMulN8(&u, Core_Natural8_Greatest, 4, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural8 upper, lower;
    Core_Natural8 u, v;
    // The lower Core_Natural8 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural8_Greatest;
    Core_Natural8 mask;
    Core_makeBitMaskN8(&mask, 6, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural16 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // 4 * MAX = (lower, upper)
    if (Core_safeMulN8(&u, 4, Core_Natural8_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  return Core_Success;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural16 u, v; \
  if (Core_safeMulN16(&u, A, B, &v)) { \
    return Core_Failure; \
  } \
  if (u != U) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
  if (v != V) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
}

static Core_Result safeMulN16Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(Core_Natural16_Greatest, 0, 0, 0);
  // 0 * MAX = (0, 0)
  DEFINE(0, Core_Natural16_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Natural16_Greatest, 1, Core_Natural16_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Natural16_Greatest, Core_Natural16_Greatest, 0);
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural16 lower, upper;
    Core_Natural16 u, v;
    // The lower Core_Natural16 value.
    // All bits are one except for the LSB.
    lower = Core_Natural16_Greatest;
    Core_Natural16 mask;
    Core_makeBitMaskN16(&mask, 15, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural16 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = (lower, upper)
    if (Core_safeMulN16(&u, Core_Natural16_Greatest, 2, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural16 lower, upper;
    Core_Natural16 u, v;
    // The lower Core_Natural16 value.
    // All bits are one except for the LSB.
    lower = Core_Natural16_Greatest;
    Core_Natural16 mask;
    Core_makeBitMaskN16(&mask, 15, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural16 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = (lower, upper)
    if (Core_safeMulN16(&u, 2, Core_Natural16_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural16 upper, lower;
    Core_Natural16 u, v;
    // The lower Core_Natural16 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural16_Greatest;
    Core_Natural16 mask;
    Core_makeBitMaskN16(&mask, 14, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural16 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = (lower, upper)
    if (Core_safeMulN16(&u, Core_Natural16_Greatest, 4, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural16 upper, lower;
    Core_Natural16 u, v;
    // The lower Core_Natural16 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural16_Greatest;
    Core_Natural16 mask;
    Core_makeBitMaskN16(&mask, 14, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural16 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // 4 * MAX = (lower, upper)
    if (Core_safeMulN16(&u, 4, Core_Natural16_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  return Core_Success;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural32 u, v; \
  if (Core_safeMulN32(&u, A, B, &v)) { \
    return Core_Failure; \
  } \
  if (u != U) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
  if (v != V) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
}

static Core_Result safeMulN32Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (MAX, 0)
  DEFINE(Core_Natural32_Greatest, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, Core_Natural32_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Natural32_Greatest, 1, Core_Natural32_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Natural32_Greatest, Core_Natural32_Greatest, 0);
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural32 lower, upper;
    Core_Natural32 u, v;
    // The lower Core_Natural32 value.
    // All bits are one except for the LSB.
    lower = Core_Natural32_Greatest;
    Core_Natural32 mask;
    Core_makeBitMaskN32(&mask, 31, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural32 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = (lower, upper)
    if (Core_safeMulN32(&u, Core_Natural32_Greatest, 2, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural32 lower, upper;
    Core_Natural32 u, v;
    // The lower Core_Natural32 value.
    // All bits are one except for the LSB.
    lower = Core_Natural32_Greatest;
    Core_Natural32 mask;
    Core_makeBitMaskN32(&mask, 31, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural32 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // 2 * MAX = (lower, upper)
    if (Core_safeMulN32(&u, 2, Core_Natural32_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural32 lower, upper;
    Core_Natural32 u, v;
    // The lower Core_Natural32 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural32_Greatest;
    Core_Natural32 mask;
    Core_makeBitMaskN32(&mask, 30, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural32 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = (lower, upper)
    if (Core_safeMulN32(&u, Core_Natural32_Greatest, 4, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural32 lower, upper;
    Core_Natural32 u, v;
    // The lower Core_Natural32 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural32_Greatest;
    Core_Natural32 mask;
    Core_makeBitMaskN32(&mask, 30, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural32 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // 4 * MA = (lower, upper)
    if (Core_safeMulN32(&u, 4, Core_Natural32_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  return Core_Success;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural64 u, v; \
  if (Core_safeMulN64(&u, A, B, &v)) { \
    return Core_Failure; \
  } \
  if (u != U) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
  if (v != V) { \
    Core_setError(Core_Error_TestFailed); \
    return Core_Failure; \
  } \
}

static Core_Result safeMulN64Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (MAX, 0)
  DEFINE(Core_Natural64_Greatest, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, Core_Natural64_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Natural64_Greatest, 1, Core_Natural64_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Natural64_Greatest, Core_Natural64_Greatest, 0);
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural64 upper, lower;
    Core_Natural64 u, v;
    // The lower Core_Natural64 value.
    // All bits are one except for the LSB.
    lower = Core_Natural64_Greatest;
    Core_Natural64 mask;
    Core_makeBitMaskN64(&mask, 63, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural64 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = (lower, upper)
    if (Core_safeMulN64(&u, Core_Natural64_Greatest, 2, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural64 upper, lower;
    Core_Natural64 u, v;
    // The lower Core_Natural64 value.
    // All bits are one except for the LSB.
    lower = Core_Natural64_Greatest;
    Core_Natural64 mask;
    Core_makeBitMaskN64(&mask, 63, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural64 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // 2 * MAX = (lower, upper)
    if (Core_safeMulN64(&u, 2, Core_Natural64_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural64 upper, lower;
    Core_Natural64 u, v;
    // The lower Core_Natural64 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural64_Greatest;
    Core_Natural64 mask;
    Core_makeBitMaskN64(&mask, 62, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural64 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = (lower, upper)
    if (Core_safeMulN64(&u, Core_Natural64_Greatest, 4, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Core_Natural64 upper, lower;
    Core_Natural64 u, v;
    // The lower Core_Natural64 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Core_Natural64_Greatest;
    Core_Natural64 mask;
    Core_makeBitMaskN64(&mask, 62, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Core_Natural64 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // 4 * MAX = (lower, upper)
    if (Core_safeMulN64(&u, 4, Core_Natural64_Greatest, &v)) {
      return Core_Failure;
    }
    if (u != lower) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    if (v != upper) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  return Core_Success;
}

#undef DEFINE

Core_Result Core_Tests_safeMulNxTests() {
  if (safeMulN8Tests()) {
    return Core_Failure;
  }
  if (safeMulN16Tests()) {
    return Core_Failure;
  }
  if (safeMulN32Tests()) {
    return Core_Failure;
  }
  if (safeMulN64Tests()) {
    return Core_Failure;
  }
  return Core_Success;
}
