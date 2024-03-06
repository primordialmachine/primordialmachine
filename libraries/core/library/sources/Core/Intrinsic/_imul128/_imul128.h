#if !defined(CORE_INTRINSIC__IMUL128__IMUL128_H_INCLUDED)
#define CORE_INTRINSIC__IMUL128__IMUL128_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x64/_imul128_x64.h"

  // compute the product of two int64_t values
  // store the high word of the int128_t_t result in *z
  // return the low word of the int128_t result
  static inline int64_t _dx_imul128(int64_t x, int64_t y, int64_t* z) {
    return _dx_imul128_x64(x, y, z);
  }

#endif

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x86/_imul128_x86.h"

  // compute the product of two int64_t values
  // store the high word of the int128_t_t result in *z
  // return the low word of the int128_t result
  static inline int64_t _dx_imul128(int64_t x, int64_t y, int64_t* z) {
    return _dx_imul128_x86(x, y, z);
  }

#endif

#endif // CORE_INTRINSIC__IMUL128__IMUL128_H_INCLUDED
