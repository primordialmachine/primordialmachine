#include "Core/Tests/safeMulNx.h"

/// @{
/// @internal
/// @brief Get the @a n most significant bits a dx_nx value.
/// @param x The dx_nx value.
/// @param n The @a n bits to get. The behavior is undefined if @a n is greater than @a DX_NX_NUMBER_OF_BITS. 
/// @return
/// Let <code>n</code> be the number of bits to get.
/// If <code>n</code> is @a 0, then @a 0 is returned.
/// Let <code>sum_{i=0}^DX_NUMBER_OF_BITS 2^i * x[i]</code> be the binary representation of @a x.
/// Then the value <code>sum_{i=DX_NX_NUMBER_OF_BITS-n}^DX_NX_NUMBER_OF_BITS 2^i * x[i] / 2^(DX_NX_NUMBER_OF_BITS-n)</code> is returned..
/// 
/// By the above, let's take a look at some examples for <code>DX_N8_NUMBER_OF_BITS</code>.
/// - @code{n = 0}.
/// By the above, @a 0 is returned.
/// 
/// - @code{n = 1}
/// By the above, <code>sum_{i=7}^7 2^i x_i / 2^7 = 2^7 x_7 / 2^7 = 2^0 x_7</code> is returned.
/// 
/// - @code{n = 2}
/// By the above <code>sum_{i=6}^7 2^i x_i / 2^6 = (2^6 x_6 + 2^7 x_7) / 2^6 = 2^0 x_6 + 2^1 x_7</code> is returned.
/// 
/// - @code{n = 8}
/// By the above <code>sum_{i=0}^7 2^i x_i / 2^0 = \sum_{i=0}^7 2^i x_i = x</code> is returned.
/// 
static Core_Natural8 get_msbs_n8(Core_Natural8 x, Core_Natural8 n) {
  Core_Natural8 mask = 0;
  for (Core_Size i = 8; i > 8 - n; --i) {
    mask |= ((Core_Natural8)1) << (i - 1);
  }
  return (x & mask) >> (8 - n);
}

static Core_Natural16 get_msbs_n16(Core_Natural16 x, Core_Natural8 n) {
  Core_Natural16 mask = 0;
  for (Core_Size i = 16; i > 16 - n; --i) {
    mask |= ((Core_Natural16)1) << (i - 1);
  }
  return (x & mask) >> (16 - n);
}

static Core_Natural32 get_msbs_n32(Core_Natural32 x, Core_Natural8 n) {
  Core_Natural32 mask = 0;
  for (Core_Size i = 32; i > 32 - n; --i) {
    mask |= ((Core_Natural32)1) << (i - 1);
  }
  return (x & mask) >> (32 - n);
}

static Core_Natural64 get_msbs_n64(Core_Natural64 x, Core_Natural8 n) {
  Core_Natural64 mask = 0;
  for (Core_Size i = 64; i > 64 - n; --i) {
    mask |= ((Core_Natural64)1) << (i - 1);
  }
  return (x & mask) >> (64 - n);
}

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural8 u, v; \
  u = dx_mul_u8(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}


Core_Result dx_safe_mul_n8_tests() {
  Core_Natural8 u, v;

  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);

  // MAX * 0 = (MAX, 0)
  DEFINE(Core_Natural8_Greatest, 0, 0, 0);

  // 0 * MAX = (MAX, 0)
  u = dx_mul_u8(0, Core_Natural8_Greatest, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 1 = (MAX, 0)
  u = dx_mul_u8(Core_Natural8_Greatest, 1, &v);
  if (u != Core_Natural8_Greatest) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 1 * MAX = (MAX, 0)
  u = dx_mul_u8(1, Core_Natural8_Greatest, &v);
  if (u != Core_Natural8_Greatest) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  uint8_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint8_t)(Core_Natural8_Greatest << 1);
  b = get_msbs_n8(Core_Natural8_Greatest, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u8(Core_Natural8_Greatest, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u8(2, Core_Natural8_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint8_t)(Core_Natural8_Greatest << 2);
  b = get_msbs_n8(Core_Natural8_Greatest, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u8(Core_Natural8_Greatest, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u8(4, Core_Natural8_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural16 u, v; \
  u = dx_mul_u16(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

int dx_safe_mul_n16_tests() {
  Core_Natural16 u, v;

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

  uint16_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint16_t)(Core_Natural16_Greatest << 1);
  b = get_msbs_n16(Core_Natural16_Greatest, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u16(Core_Natural16_Greatest, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u16(2, Core_Natural16_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint16_t)(Core_Natural16_Greatest << 2);
  b = get_msbs_n16(Core_Natural16_Greatest, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u16(Core_Natural16_Greatest, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u16(4, Core_Natural16_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural32 u, v; \
  u = dx_mul_n32(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

int dx_safe_mul_n32_tests() {
  Core_Natural32 u, v;

  // 0 * 0 = (0, 0)
  u = dx_mul_u32(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 0 = (MAX, 0)
  u = dx_mul_u32(Core_Natural32_Greatest, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 0 * MAX = (MAX, 0)
  u = dx_mul_u32(0, Core_Natural32_Greatest, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 1 = (MAX, 0)
  u = dx_mul_u32(Core_Natural32_Greatest, 1, &v);
  if (u != Core_Natural32_Greatest) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 1 * MAX = (MAX, 0)
  u = dx_mul_u32(1, Core_Natural32_Greatest, &v);
  if (u != Core_Natural32_Greatest) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  uint32_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint32_t)(Core_Natural32_Greatest << 1);
  b = get_msbs_n32(Core_Natural32_Greatest, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u32(Core_Natural32_Greatest, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u32(2, Core_Natural32_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint32_t)(Core_Natural32_Greatest << 2);
  b = get_msbs_n32(Core_Natural32_Greatest, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u32(Core_Natural32_Greatest, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u32(4, Core_Natural32_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  Core_Natural64 u, v; \
  u = dx_mul_n64(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

int dx_safe_mul_n64_tests() {
  Core_Natural64 u, v;

  // 0 * 0 = (0, 0)
  u = dx_mul_u64(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 0 = (MAX, 0)
  u = dx_mul_u64(Core_Natural64_Greatest, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 0 * MAX = (MAX, 0)
  u = dx_mul_u64(0, Core_Natural64_Greatest, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 1 = (MAX, 0)
  u = dx_mul_u64(Core_Natural64_Greatest, 1, &v);
  if (u != Core_Natural64_Greatest) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 1 * MAX = (MAX, 0)
  u = dx_mul_u64(1, Core_Natural64_Greatest, &v);
  if (u != Core_Natural64_Greatest) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  uint64_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint64_t)(Core_Natural64_Greatest << 1);
  b = get_msbs_n64(Core_Natural64_Greatest, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u64(Core_Natural64_Greatest, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u64(2, Core_Natural64_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint64_t)(Core_Natural64_Greatest << 2);
  b = get_msbs_n64(Core_Natural64_Greatest, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u64(Core_Natural64_Greatest, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u64(4, Core_Natural64_Greatest, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

Core_Result Core_Tests_safeMulNxTests() {
  if (dx_safe_mul_n8_tests()) {
    return Core_Failure;
  }
  if (dx_safe_mul_n16_tests()) {
    return Core_Failure;
  }
  if (dx_safe_mul_n32_tests()) {
    return Core_Failure;
  }
  if (dx_safe_mul_n64_tests()) {
    return Core_Failure;
  }
  return Core_Success;
}
