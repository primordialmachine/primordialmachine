#if !defined(CORE_INTRINSIC__UMUL64__UMUL64_H_INCLUDED)
#define CORE_INTRINSIC__UMUL64__UMUL64_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x64/_umul64_x64.h"

  // compute the product of two uint32_t values
  // store the high word of the uint64_t result in *z
  // return the low word of the uint64_t result
  static inline uint32_t _dx_umul64(uint32_t x, uint32_t y, uint32_t* z) {
    return _dx_umul64_x64(x, y, z);
  }

#endif

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x86/_umul64_x86_fastcall.h"

  // compute the product of two uint16_t values
  // store the high word of the uint32_t result in *z
  // return the low word of the uint32_t result
  static inline uint32_t _dx_umul64(uint32_t x, uint32_t y, uint32_t* z) {
    return _dx_umul64_x86_fastcall(x, y, z);
  }

#endif

#endif // CORE_INTRINSIC__UMUL64__UMUL64_H_INCLUDED
