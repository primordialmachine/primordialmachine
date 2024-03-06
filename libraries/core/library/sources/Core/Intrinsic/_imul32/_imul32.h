#if !defined(CORE_INTRINSIC__IMUL32__IMUL32_H_INCLUDED)
#define CORE_INTRINSIC__IMUL32__IMUL32_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x64/_imul32_x64.h"

  // compute the product of two int16_t values
  // store the high word of the int32_t result in *z
  // return the low word of the int32_t result
  static inline int16_t _dx_imul32(int16_t x, int16_t y, int16_t* z) {
    return _dx_imul32_x64(x, y, z);
  }

#endif

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x86/_imul32_x86_fastcall.h"
  
  // compute the product of two int16_t values
  // store the high word of the int32_t result in *z
  // return the low word of the int32_t result
  static inline int16_t _dx_imul32(int16_t x, int16_t y, int16_t* z) {
    return _dx_imul32_x86_fastcall(x, y, z);
  }

#endif

#endif // CORE_INTRINSIC__IMUL32__IMUL32_H_INCLUDED
