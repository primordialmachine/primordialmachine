#if !defined(CORE_INTRINSIC__IMUL16__IMUL16_H_INCLUDED)
#define CORE_INTRINSIC__IMUL16__IMUL16_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x64/_imul16_x64.h"

  // compute the product of two int8_t values
  // store the high word of the int16_t result in *z
  // return the low word of the int16_t result
  static inline int8_t _dx_imul16(int8_t x, int8_t y, int8_t* z) {
    return _dx_imul16_x64(x, y, z);
  }

#endif

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x86/_imul16_x86_fastcall.h"
  
  // compute the product of two int8_t values
  // store the high word of the int16_t result in *z
  // return the low word of the int16_t result
  static inline int8_t _dx_imul16(int8_t x, int8_t y, int8_t* z) {
    return _dx_imul16_x86_fastcall(x, y, z);
  }

#endif

#endif // CORE_INTRINSIC__IMUL16__IMUL16_H_INCLUDED
