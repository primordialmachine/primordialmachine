#include "Core/Intrinsic/_umul128/_umul128.h"

#include "dx/core/Configure.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

#include <intrin.h>

uint64_t _dx_umul128(uint64_t x, uint64_t y, uint64_t* z) {
  return _umul128(x, y, z);
}

#endif

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

// thanks to the xmrrig project
uint64_t _dx_umul128(uint64_t x, uint64_t y, uint64_t* z) {
  // x   = ab = a * 2^32 + b
  // multiplicand = cd = c * 2^32 + d
  //   ab * cd
  // = (a * 2^32 + b) * (c * 2^32 + d)
  // = a * 2^32 * (c * 2^32 + d) + b * (c * 2^32 + d)
  // = a * 2^32 * c * 2^32 + a * 2^32 * d + b * c * 2^32 + b * d
  // = a * c * 2^64 + (a * d + b * c) * 2^32 + b * d
  uint64_t a = x >> 32;
  uint64_t b = x & 0xFFFFFFFF;
  uint64_t c = y >> 32;
  uint64_t d = y & 0xFFFFFFFF;

  // uint64_t ac = a * c;
  uint64_t ad = a * d;
  // uint64_t bc = b * c;
  uint64_t bd = b * d;

  uint64_t adbc = ad + (b * c);
  uint64_t adbc_carry = adbc < ad ? 1 : 0;

  // x * y = z * 2^64 + w
  uint64_t w = bd + (adbc << 32);
  uint64_t w_carry = w < bd ? 1 : 0;
  *z = (a * c) + (adbc >> 32) + (adbc_carry << 32) + w_carry;

  return w;
}

#endif
