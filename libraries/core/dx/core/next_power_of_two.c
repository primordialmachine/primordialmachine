#include "dx/core/next_power_of_two.h"

#include "dx/core/count_leading_zeroes.h"

dx_result dx_next_power_of_two_gt_n8(dx_n8* RETURN, dx_n8 x) {
  static dx_n8 const greatest_power_of_two = ((dx_n8)1) << 7;
  if (x < 1) {
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  dx_size i = dx_count_leading_zeroes_n8(x);
  dx_n8 t = ((dx_n8)1) << (8 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  *RETURN = t;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gte_n8(dx_n8* RETURN, dx_n8 x) {
  static dx_n8 const greatest_power_of_two = ((dx_n8)1) << 7;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n8 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gt_n16(dx_n16* RETURN, dx_n16 x) {
  static dx_n16 const greatest_power_of_two = ((dx_n16)1) << 15;
  if (x < 1) {
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  dx_size i = dx_count_leading_zeroes_n16(x);
  dx_n16 t = ((dx_n16)1) << (16 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  *RETURN = t;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gte_n16(dx_n16* RETURN, dx_n16 x) {
  static dx_n16 const greatest_power_of_two = ((dx_n16)1) << 15;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n16 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gt_n32(dx_n32* RETURN, dx_n32 x) {
  static dx_n32 const greatest_power_of_two = ((dx_n32)1) << 31;
  if (x < 1) {
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  dx_size i = dx_count_leading_zeroes_n32(x);
  dx_n32 t = ((dx_n32)1) << (32 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  *RETURN = t;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gte_n32(dx_n32* RETURN, dx_n32 x) {
  static dx_n32 const greatest_power_of_two = ((dx_n32)1) << 31;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n32 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gt_n64(dx_n64* RETURN, dx_n64 x) {
  static dx_n64 const greatest_power_of_two = ((dx_n64)1) << 63;
  if (x < 1) {
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  dx_size i = dx_count_leading_zeroes_n64(x);
  dx_n64 t = ((dx_n64)1) << (64 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  *RETURN = t;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gte_n64(dx_n64* RETURN, dx_n64 x) {
  static dx_n64 const greatest_power_of_two = ((dx_n64)1) << 63;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return DX_SUCCESS;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return DX_FAILURE;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n64 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_gt_sz(dx_size* RETURN, dx_size x) {
#if defined(_M_X64)
  return dx_next_power_of_two_gt_n64(RETURN, x);
#elif defined(_M_IX86)
  return dx_next_power_of_two_gt_n64(RETURN, x);
#else
  #error("environment not (yet) supported")
#endif
}

dx_result dx_next_power_of_two_gte_sz(dx_size* RETURN, dx_size x) {
#if defined(_M_X64)
  return dx_next_power_of_two_gte_n64(RETURN, x);
#elif defined(_M_IX86)
  return dx_next_power_of_two_gte_n64(RETURN, x);
#else
  #error("environment not (yet) supported")
#endif
}

#if defined(DX_NEXT_POWER_OF_TWO_WITH_TESTS) && 1 == DX_NEXT_POWER_OF_TWO_WITH_TESTS

#define EXPECT_SUCCESS(SUFFIX, INPUT, OUTPUT) \
  { \
    dx_set_error(DX_NO_ERROR); \
    v = INPUT; \
    if (dx_next_power_of_two_##SUFFIX(&u, v)) { \
      return DX_FAILURE; \
    } \
    if (u != OUTPUT) { \
      return DX_FAILURE; \
    } \
  }

#define EXPECT_FAILURE(SUFFIX, INPUT, ERROR) \
  dx_set_error(DX_NO_ERROR); \
  v = INPUT; \
  if (dx_next_power_of_two_##SUFFIX(&u, v)) { \
    if (ERROR != dx_get_error()) { \
      return DX_FAILURE; \
    } \
    dx_set_error(DX_NO_ERROR); \
  } else { \
    dx_set_error(DX_ERROR_INVALID_ARGUMENT); \
    return DX_FAILURE; \
  }

static dx_result dx_next_power_of_two_test_n64() {
  dx_n64 u, v;

  EXPECT_SUCCESS(gt_n64, 0, 1);
  EXPECT_SUCCESS(gte_n64, 0, 1);
  
  EXPECT_SUCCESS(gt_n64, 1, 2);
  EXPECT_SUCCESS(gte_n64, 1, 1);

  EXPECT_SUCCESS(gte_n64, 2, 2);
  EXPECT_SUCCESS(gt_n64, 2, 4);
    
  EXPECT_SUCCESS(gte_n64, 3, 4);
  EXPECT_SUCCESS(gt_n64, 3, 4);

  EXPECT_SUCCESS(gte_n64, 4, 4);
  EXPECT_SUCCESS(gt_n64, 4, 8);

  EXPECT_SUCCESS(gte_n64, 5, 8);
  EXPECT_SUCCESS(gt_n64, 5, 8);

  EXPECT_SUCCESS(gte_n64, 6, 8);
  EXPECT_SUCCESS(gt_n64, 6, 8);

  EXPECT_SUCCESS(gte_n64, 7, 8);
  EXPECT_SUCCESS(gt_n64, 7, 8);

  EXPECT_SUCCESS(gte_n64, 8, 8);
  EXPECT_SUCCESS(gt_n64, 8, 16);

  // ...

  EXPECT_FAILURE(gte_n64, DX_N64_GREATEST - 1, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n64, DX_N64_GREATEST - 1, DX_ERROR_OVERFLOW);

  EXPECT_FAILURE(gte_n64, DX_N64_GREATEST - 0, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n64, DX_N64_GREATEST - 0, DX_ERROR_OVERFLOW);

  return DX_SUCCESS;
}

static dx_result dx_next_power_of_two_test_n32() {
  dx_n32 u, v;

  EXPECT_SUCCESS(gte_n32, 0, 1);
  EXPECT_SUCCESS(gt_n32, 0, 1);

  EXPECT_SUCCESS(gte_n32, 1, 1);
  EXPECT_SUCCESS(gt_n32, 1, 2);

  EXPECT_SUCCESS(gte_n32, 2, 2);
  EXPECT_SUCCESS(gt_n32, 2, 4);

  EXPECT_SUCCESS(gte_n32, 3, 4);
  EXPECT_SUCCESS(gt_n32, 3, 4);

  EXPECT_SUCCESS(gte_n32, 4, 4);
  EXPECT_SUCCESS(gt_n32, 4, 8);

  EXPECT_SUCCESS(gte_n32, 5, 8);
  EXPECT_SUCCESS(gt_n32, 5, 8);

  EXPECT_SUCCESS(gte_n32, 6, 8);
  EXPECT_SUCCESS(gt_n32, 6, 8);

  EXPECT_SUCCESS(gte_n32, 7, 8);
  EXPECT_SUCCESS(gt_n32, 7, 8);

  EXPECT_SUCCESS(gte_n32, 8, 8);
  EXPECT_SUCCESS(gt_n32, 8, 16);

  // ...

  EXPECT_FAILURE(gte_n32, DX_N32_GREATEST - 1, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n32, DX_N32_GREATEST - 1, DX_ERROR_OVERFLOW);

  EXPECT_FAILURE(gte_n32, DX_N32_GREATEST - 0, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n32, DX_N32_GREATEST - 0, DX_ERROR_OVERFLOW);

  return DX_SUCCESS;
}

static dx_result dx_next_power_of_two_test_n16() {
  dx_n16 u, v;

  EXPECT_SUCCESS(gte_n16, 0, 1);
  EXPECT_SUCCESS(gt_n16, 0, 1);

  EXPECT_SUCCESS(gte_n16, 1, 1);
  EXPECT_SUCCESS(gt_n16, 1, 2);

  EXPECT_SUCCESS(gte_n16, 2, 2);
  EXPECT_SUCCESS(gt_n16, 2, 4);

  EXPECT_SUCCESS(gte_n16, 3, 4);
  EXPECT_SUCCESS(gt_n16, 3, 4);

  EXPECT_SUCCESS(gte_n16, 4, 4);
  EXPECT_SUCCESS(gt_n16, 4, 8);

  EXPECT_SUCCESS(gte_n16, 5, 8);
  EXPECT_SUCCESS(gt_n16, 5, 8);

  EXPECT_SUCCESS(gte_n16, 6, 8);
  EXPECT_SUCCESS(gt_n16, 6, 8);

  EXPECT_SUCCESS(gte_n16, 7, 8);
  EXPECT_SUCCESS(gt_n16, 7, 8);

  EXPECT_SUCCESS(gte_n16, 8, 8);
  EXPECT_SUCCESS(gt_n16, 8, 16);

  // ...

  EXPECT_FAILURE(gte_n16, DX_N16_GREATEST - 1, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n16, DX_N16_GREATEST - 1, DX_ERROR_OVERFLOW);

  EXPECT_FAILURE(gte_n16, DX_N16_GREATEST - 0, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n16, DX_N16_GREATEST - 0, DX_ERROR_OVERFLOW);

  return DX_SUCCESS;
}

static dx_result dx_next_power_of_two_test_n8() {
  dx_n8 u, v;

  EXPECT_SUCCESS(gte_n8, 0, 1);
  EXPECT_SUCCESS(gt_n8, 0, 1);

  EXPECT_SUCCESS(gte_n8, 1, 1);
  EXPECT_SUCCESS(gt_n8, 1, 2);

  EXPECT_SUCCESS(gte_n8, 2, 2);
  EXPECT_SUCCESS(gt_n8, 2, 4);

  EXPECT_SUCCESS(gte_n8, 3, 4);
  EXPECT_SUCCESS(gt_n8, 3, 4);

  EXPECT_SUCCESS(gte_n8, 4, 4);
  EXPECT_SUCCESS(gt_n8, 4, 8);

  EXPECT_SUCCESS(gte_n8, 5, 8);
  EXPECT_SUCCESS(gt_n8, 5, 8);

  EXPECT_SUCCESS(gte_n8, 6, 8);
  EXPECT_SUCCESS(gt_n8, 6, 8);

  EXPECT_SUCCESS(gte_n8, 7, 8);
  EXPECT_SUCCESS(gt_n8, 7, 8);

  EXPECT_SUCCESS(gte_n8, 8, 8);
  EXPECT_SUCCESS(gt_n8, 8, 16);

  // ...

  EXPECT_FAILURE(gte_n8, DX_N8_GREATEST - 1, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n8, DX_N8_GREATEST - 1, DX_ERROR_OVERFLOW);
  
  EXPECT_FAILURE(gte_n8, DX_N8_GREATEST - 0, DX_ERROR_OVERFLOW);
  EXPECT_FAILURE(gt_n8, DX_N8_GREATEST - 0, DX_ERROR_OVERFLOW);

  return DX_SUCCESS;
}

dx_result dx_next_power_of_two_tests() {
  if (dx_next_power_of_two_test_n64()) {
    return DX_FAILURE;
  }
  if (dx_next_power_of_two_test_n32()) {
    return DX_FAILURE;
  }
  if (dx_next_power_of_two_test_n16()) {
    return DX_FAILURE;
  }
  if (dx_next_power_of_two_test_n8()) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

#endif
