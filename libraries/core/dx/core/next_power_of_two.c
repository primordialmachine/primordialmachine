#include "dx/core/next_power_of_two.h"

#include "dx/core/count_leading_zeroes.h"

dx_n8 dx_next_power_of_two_gt_n8(dx_n8 x) {
  static dx_n8 const greatest_power_of_two = ((dx_n8)1) << 7;
  if (x < 1) {
    return 1;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  dx_size i = dx_count_leading_zeroes_n8(x);
  dx_n8 t = ((dx_n8)1) << (8 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n8 dx_next_power_of_two_gte_n8(dx_n8 x) {
  static dx_n8 const greatest_power_of_two = ((dx_n8)1) << 7;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    return 1;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n8 y = 2;
  while (y < x) {
    y <<= 1;
  }
  return y;
}

dx_n16 dx_next_power_of_two_gt_n16(dx_n16 x) {
  static dx_n16 const greatest_power_of_two = ((dx_n16)1) << 15;
  if (x < 1) {
    return 1;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  dx_size i = dx_count_leading_zeroes_n16(x);
  dx_n16 t = ((dx_n16)1) << (16 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n16 dx_next_power_of_two_gte_n16(dx_n16 x) {
  static dx_n16 const greatest_power_of_two = ((dx_n16)1) << 15;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    return 1;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n16 y = 2;
  while (y < x) {
    y <<= 1;
  }
  return y;
}

dx_n32 dx_next_power_of_two_gt_n32(dx_n32 x) {
  static dx_n32 const greatest_power_of_two = ((dx_n32)1) << 31;
  if (x < 1) {
    return 1;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  dx_size i = dx_count_leading_zeroes_n32(x);
  dx_n32 t = ((dx_n32)1) << (32 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n32 dx_next_power_of_two_gte_n32(dx_n32 x) {
  static dx_n32 const greatest_power_of_two = ((dx_n32)1) << 31;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    return 1;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n32 y = 2;
  while (y < x) {
    y <<= 1;
  }
  return y;
}

dx_n64 dx_next_power_of_two_gt_n64(dx_n64 x) {
  static dx_n64 const greatest_power_of_two = ((dx_n64)1) << 63;
  if (x < 1) {
    return 1;
  }
  if (x >= greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  dx_size i = dx_count_leading_zeroes_n64(x);
  dx_n64 t = ((dx_n64)1) << (64 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n64 dx_next_power_of_two_gte_n64(dx_n64 x) {
  static dx_n64 const greatest_power_of_two = ((dx_n64)1) << 63;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    return 1;
  }
  if (x > greatest_power_of_two) {
    dx_set_error(DX_ERROR_OVERFLOW);
    return 0;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  dx_n64 y = 2;
  while (y < x) {
    y <<= 1;
  }
  return y;
}

dx_size dx_next_power_of_two_gt_sz(dx_size x) {
#if defined(_M_X64)
  return dx_next_power_of_two_gt_n64(x);
#elif defined(_M_IX86)
  return dx_next_power_of_two_gt_n64(x);
#else
  #error("environment not supported")
#endif
}

dx_size dx_next_power_of_two_gte_sz(dx_size x) {
#if defined(_M_X64)
  return dx_next_power_of_two_gte_n64(x);
#elif defined(_M_IX86)
  return dx_next_power_of_two_gte_n64(x);
#else
#error("environment not supported")
#endif
}

#if defined(DX_NEXT_POWER_OF_TWO_WITH_TESTS) && 1 == DX_NEXT_POWER_OF_TWO_WITH_TESTS

static int dx_next_power_of_two_gt_test_n64() {
  dx_n64 u, v;

  dx_set_error(DX_NO_ERROR);
  v = 0;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 1) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 1;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 2) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 2;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 3;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 4;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 5;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 6;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 7;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 8;
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 16) {
    return 1;
  }

  // ...

  dx_set_error(DX_NO_ERROR);
  v = DX_N64_GREATEST - 1; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  dx_set_error(DX_NO_ERROR);
  v = DX_N64_GREATEST; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n64(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  return 0;
}

static int dx_next_power_of_two_gt_test_n32() {
  dx_n32 u, v;

  dx_set_error(DX_NO_ERROR);
  v = 0;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 1) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 1;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 2) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 2;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 3;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 4;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 5;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 6;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 7;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 8;
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 16) {
    return 1;
  }

  // ...

  dx_set_error(DX_NO_ERROR);
  v = DX_N32_GREATEST - 1; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  dx_set_error(DX_NO_ERROR);
  v = DX_N32_GREATEST; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n32(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  return 0;
}

static int dx_next_power_of_two_gt_test_n16() {
  dx_n16 u, v;

  dx_set_error(DX_NO_ERROR);
  v = 0;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 1) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 1;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 2) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 2;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 3;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 4;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 5;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 6;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 7;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 8;
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 16) {
    return 1;
  }

  // ...

  dx_set_error(DX_NO_ERROR);
  v = DX_N16_GREATEST - 1; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  dx_set_error(DX_NO_ERROR);
  v = DX_N16_GREATEST; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n16(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  return 0;
}

static int dx_next_power_of_two_gt_test_n8() {
  dx_n8 u, v;

  dx_set_error(DX_NO_ERROR);
  v = 0;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 1) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 1;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 2) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 2;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 3;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 4) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 4;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 5;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 6;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 7;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 8) {
    return 1;
  }

  dx_set_error(DX_NO_ERROR);
  v = 8;
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 16) {
    return 1;
  }

  // ...

  dx_set_error(DX_NO_ERROR);
  v = DX_N8_GREATEST - 1; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  dx_set_error(DX_NO_ERROR);
  v = DX_N8_GREATEST; // this is certainly no power of two
  u = dx_next_power_of_two_gt_n8(v);
  if (u != 0 && dx_get_error() != DX_ERROR_OVERFLOW) {
    return 1;
  }
  dx_set_error(DX_NO_ERROR);

  return 0;
}

int dx_next_power_of_two_tests() {
  if (dx_next_power_of_two_gt_test_n64()) {
    return 1;
  }
  if (dx_next_power_of_two_gt_test_n32()) {
    return 1;
  }
  if (dx_next_power_of_two_gt_test_n16()) {
    return 1;
  }
  if (dx_next_power_of_two_gt_test_n8()) {
    return 1;
  }
  return 0;
}

#endif
