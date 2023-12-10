#include "Core/Tests/nextPowerOfTwo.h"

#define EXPECT_SUCCESS(SUFFIX, INPUT, OUTPUT) \
  { \
    Core_setError(Core_Error_NoError); \
    v = INPUT; \
    if (Core_nextPowerOfTwo##SUFFIX(&u, v)) { \
      return Core_Failure; \
    } \
    if (u != OUTPUT) { \
      return Core_Failure; \
    } \
  }

#define EXPECT_FAILURE(SUFFIX, INPUT, ERROR) \
  Core_setError(Core_Error_NoError); \
  v = INPUT; \
  if (Core_nextPowerOfTwo##SUFFIX(&u, v)) { \
    if (ERROR != Core_getError()) { \
      return Core_Failure; \
    } \
    Core_setError(Core_Error_NoError); \
  } else { \
    Core_setError(Core_Error_ArgumentInvalid); \
    return Core_Failure; \
  }

static Core_Result Core_Tests_nextPowerOfTwoTestsN64() {
  Core_Natural64 u, v;

  EXPECT_SUCCESS(GteN64, 0, 1);
  EXPECT_SUCCESS(GtN64, 0, 1);
  
  EXPECT_SUCCESS(GteN64, 1, 1);
  EXPECT_SUCCESS(GtN64, 1, 2);

  EXPECT_SUCCESS(GteN64, 2, 2);
  EXPECT_SUCCESS(GtN64, 2, 4);
    
  EXPECT_SUCCESS(GteN64, 3, 4);
  EXPECT_SUCCESS(GtN64, 3, 4);

  EXPECT_SUCCESS(GteN64, 4, 4);
  EXPECT_SUCCESS(GtN64, 4, 8);

  EXPECT_SUCCESS(GteN64, 5, 8);
  EXPECT_SUCCESS(GtN64, 5, 8);

  EXPECT_SUCCESS(GteN64, 6, 8);
  EXPECT_SUCCESS(GtN64, 6, 8);

  EXPECT_SUCCESS(GteN64, 7, 8);
  EXPECT_SUCCESS(GtN64, 7, 8);

  EXPECT_SUCCESS(GteN64, 8, 8);
  EXPECT_SUCCESS(GtN64, 8, 16);

  // ...

  EXPECT_FAILURE(GteN64, Core_Natural64_Greatest - 1, Core_Error_Overflow);
  EXPECT_FAILURE(GtN64, Core_Natural64_Greatest - 1, Core_Error_Overflow);

  EXPECT_FAILURE(GteN64, Core_Natural64_Greatest - 0, Core_Error_Overflow);
  EXPECT_FAILURE(GtN64, Core_Natural64_Greatest - 0, Core_Error_Overflow);

  return Core_Success;
}

static Core_Result Core_Tests_nextPowerOfTwoTestsN32() {
  Core_Natural32 u, v;

  EXPECT_SUCCESS(GteN32, 0, 1);
  EXPECT_SUCCESS(GtN32, 0, 1);

  EXPECT_SUCCESS(GteN32, 1, 1);
  EXPECT_SUCCESS(GtN32, 1, 2);

  EXPECT_SUCCESS(GteN32, 2, 2);
  EXPECT_SUCCESS(GtN32, 2, 4);

  EXPECT_SUCCESS(GteN32, 3, 4);
  EXPECT_SUCCESS(GtN32, 3, 4);

  EXPECT_SUCCESS(GteN32, 4, 4);
  EXPECT_SUCCESS(GtN32, 4, 8);

  EXPECT_SUCCESS(GteN32, 5, 8);
  EXPECT_SUCCESS(GtN32, 5, 8);

  EXPECT_SUCCESS(GteN32, 6, 8);
  EXPECT_SUCCESS(GtN32, 6, 8);

  EXPECT_SUCCESS(GteN32, 7, 8);
  EXPECT_SUCCESS(GtN32, 7, 8);

  EXPECT_SUCCESS(GteN32, 8, 8);
  EXPECT_SUCCESS(GtN32, 8, 16);

  // ...

  EXPECT_FAILURE(GteN32, Core_Natural32_Greatest - 1, Core_Error_Overflow);
  EXPECT_FAILURE(GtN32, Core_Natural32_Greatest - 1, Core_Error_Overflow);

  EXPECT_FAILURE(GteN32, Core_Natural32_Greatest - 0, Core_Error_Overflow);
  EXPECT_FAILURE(GtN32, Core_Natural32_Greatest - 0, Core_Error_Overflow);

  return Core_Success;
}

static Core_Result Core_Tests_nextPowerOfTwoTestsN16() {
  Core_Natural16 u, v;

  EXPECT_SUCCESS(GteN16, 0, 1);
  EXPECT_SUCCESS(GtN16, 0, 1);

  EXPECT_SUCCESS(GteN16, 1, 1);
  EXPECT_SUCCESS(GtN16, 1, 2);

  EXPECT_SUCCESS(GteN16, 2, 2);
  EXPECT_SUCCESS(GtN16, 2, 4);

  EXPECT_SUCCESS(GteN16, 3, 4);
  EXPECT_SUCCESS(GtN16, 3, 4);

  EXPECT_SUCCESS(GteN16, 4, 4);
  EXPECT_SUCCESS(GtN16, 4, 8);

  EXPECT_SUCCESS(GteN16, 5, 8);
  EXPECT_SUCCESS(GtN16, 5, 8);

  EXPECT_SUCCESS(GteN16, 6, 8);
  EXPECT_SUCCESS(GtN16, 6, 8);

  EXPECT_SUCCESS(GteN16, 7, 8);
  EXPECT_SUCCESS(GtN16, 7, 8);

  EXPECT_SUCCESS(GteN16, 8, 8);
  EXPECT_SUCCESS(GtN16, 8, 16);

  // ...

  EXPECT_FAILURE(GteN16, Core_Natural16_Greatest - 1, Core_Error_Overflow);
  EXPECT_FAILURE(GtN16, Core_Natural16_Greatest - 1, Core_Error_Overflow);

  EXPECT_FAILURE(GteN16, Core_Natural16_Greatest - 0, Core_Error_Overflow);
  EXPECT_FAILURE(GtN16, Core_Natural16_Greatest - 0, Core_Error_Overflow);

  return Core_Success;
}

static Core_Result Core_Tests_nextPowerOfTwoTestsN8() {
  Core_Natural8 u, v;

  EXPECT_SUCCESS(GteN8, 0, 1);
  EXPECT_SUCCESS(GtN8, 0, 1);

  EXPECT_SUCCESS(GteN8, 1, 1);
  EXPECT_SUCCESS(GtN8, 1, 2);

  EXPECT_SUCCESS(GteN8, 2, 2);
  EXPECT_SUCCESS(GtN8, 2, 4);

  EXPECT_SUCCESS(GteN8, 3, 4);
  EXPECT_SUCCESS(GtN8, 3, 4);

  EXPECT_SUCCESS(GteN8, 4, 4);
  EXPECT_SUCCESS(GtN8, 4, 8);

  EXPECT_SUCCESS(GteN8, 5, 8);
  EXPECT_SUCCESS(GtN8, 5, 8);

  EXPECT_SUCCESS(GteN8, 6, 8);
  EXPECT_SUCCESS(GtN8, 6, 8);

  EXPECT_SUCCESS(GteN8, 7, 8);
  EXPECT_SUCCESS(GtN8, 7, 8);

  EXPECT_SUCCESS(GteN8, 8, 8);
  EXPECT_SUCCESS(GtN8, 8, 16);

  // ...

  EXPECT_FAILURE(GteN8, Core_Natural8_Greatest - 1, Core_Error_Overflow);
  EXPECT_FAILURE(GtN8, Core_Natural8_Greatest - 1, Core_Error_Overflow);
  
  EXPECT_FAILURE(GteN8, Core_Natural8_Greatest - 0, Core_Error_Overflow);
  EXPECT_FAILURE(GtN8, Core_Natural8_Greatest - 0, Core_Error_Overflow);

  return Core_Success;
}

Core_Result Core_Tests_nextPowerOfTwoTests() {
  if (Core_Tests_nextPowerOfTwoTestsN64()) {
    return Core_Failure;
  }
  if (Core_Tests_nextPowerOfTwoTestsN32()) {
    return Core_Failure;
  }
  if (Core_Tests_nextPowerOfTwoTestsN16()) {
    return Core_Failure;
  }
  if (Core_Tests_nextPowerOfTwoTestsN8()) {
    return Core_Failure;
  }
  return Core_Success;
}
