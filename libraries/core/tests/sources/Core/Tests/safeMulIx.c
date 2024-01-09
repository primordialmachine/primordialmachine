#include "Core/Tests/safeMulIx.h"

#define DEFINE(A, B, U, V) \
{ \
  Core_Integer8 u, v; \
  if (Core_safeMulI8(&u, A, B, &v)) { \
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

static Core_Result Core_Tests_safeMulI8Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(Core_Integer8_Greatest, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, Core_Integer8_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Integer8_Greatest, 1, Core_Integer8_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Integer8_Greatest, Core_Integer8_Greatest, 0);

  // Core_Integer8_Least = -128
  // -128     x -128     = 16384
  // 10000000 x 10000000 = 0100000000000000
  // v = 01000000 u = 00000000
  // v = 64
  DEFINE(Core_Integer8_Least, Core_Integer8_Least, 0, 64);

  return Core_Success;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Integer16 u, v; \
  if (Core_safeMulI16(&u, A, B, &v)) { \
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

static Core_Result Core_Tests_safeMulI16Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(Core_Integer16_Greatest, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, Core_Integer16_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Integer16_Greatest, 1, Core_Integer16_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Integer16_Greatest, Core_Integer16_Greatest, 0);

  // Core_Integer16_Least = -32768
  // -32768 x -32768 = 1073741824
  // 1000000000000000   x 1000000000000000 = 01000000000000000000000000000000
  // v = 0100000000000000 u = 0000000000000000
  // v = 16384
  DEFINE(Core_Integer16_Least, Core_Integer16_Least, 0, 16384);

  return Core_Success;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Integer32 u, v; \
  if (Core_safeMulI32(&u, A, B, &v)) { \
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

static Core_Result Core_Tests_safeMulI32Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(Core_Integer32_Greatest, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, Core_Integer32_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Integer32_Greatest, 1, Core_Integer32_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Integer32_Greatest, Core_Integer32_Greatest, 0);

  // Core_Integer32_Least = -2147483648
  // -2147483648 x -2147483648 = 4611686018427387904
  // 10000000000000000000000000000000 x 10000000000000000000000000000000 = 0100000000000000000000000000000000000000000000000000000000000000
  // v = 01000000000000000000000000000000 u = 00000000000000000000000000000000
  // v = 1073741824
  DEFINE(Core_Integer32_Least, Core_Integer32_Least, 0, 1073741824);

  return Core_Success;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Integer64 u, v; \
  if (Core_safeMulI64(&u, A, B, &v)) { \
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

static Core_Result Core_Tests_safeMulI64Tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(Core_Integer64_Greatest, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, Core_Integer64_Greatest, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(Core_Integer64_Greatest, 1, Core_Integer64_Greatest, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, Core_Integer64_Greatest, Core_Integer64_Greatest, 0);

  // Core_Integer64_Least = -9223372036854775808
  // -9223372036854775808 x -9223372036854775808 = 85070591730234615865843651857942052864
  // 1000000000000000000000000000000000000000000000000000000000000000 x 1000000000000000000000000000000000000000000000000000000000000000 = 01000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
  // v = 0100000000000000000000000000000000000000000000000000000000000000 u = 0000000000000000000000000000000000000000000000000000000000000000
  // v = 4611686018427387904
  DEFINE(Core_Integer64_Least, Core_Integer64_Least, 0, 4611686018427387904);

  return Core_Success;
}

#undef DEFINE

Core_Result Core_Tests_safeMulIxTests() {
  if (Core_Tests_safeMulI8Tests()) {
    return Core_Failure;
  }
  if (Core_Tests_safeMulI16Tests()) {
    return Core_Failure;
  }
  if (Core_Tests_safeMulI32Tests()) {
    return Core_Failure;
  }
  if (Core_Tests_safeMulI64Tests()) {
    return Core_Failure;
  }
  return Core_Success;
}
