#if !defined(CORE_INTRINSIC__UMUL16__UMUL16_H_INCLUDED)
#define CORE_INTRINSIC__UMUL16__UMUL16_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x64/_umul16_x64.h"

  // compute the product of two uint8_t values
  // store the high word of the uint16_t result in *z
  // return the low word of the uint16_t result
  static inline uint8_t _dx_umul16(uint8_t x, uint8_t y, uint8_t* z) {
    return _dx_umul16_x64(x, y, z);
  }

#endif

#if Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x86/_umul16_x86_fastcall.h"

  // compute the product of two uint16_t values
  // store the high word of the uint32_t result in *z
  // return the low word of the uint32_t result
  static inline uint8_t _dx_umul16(uint8_t x, uint8_t y, uint8_t* z) {
    return _dx_umul16_x86_fastcall(x, y, z);
  }

#endif

#endif // CORE_INTRINSIC__UMUL16__UMUL16_H_INCLUDED
