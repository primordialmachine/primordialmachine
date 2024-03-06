#if !defined(CORE_INTRINSIC__IMUL64__IMUL64_H_INCLUDED)
#define CORE_INTRINSIC__IMUL64__IMUL64_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x64/_imul64_x64.h"

  // compute the product of two int32_t values
  // store the high word of the int64_t result in *z
  // return the low word of the int64_t result
  static inline int32_t _dx_imul64(int32_t x, int32_t y, int32_t* z) {
    return _dx_imul64_x64(x, y, z);
  }

#endif

#if Core_OperatingSystem_Windows == Core_OperatingSystem && Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

  #include "dx/core/asm/x86/_imul64_x86_fastcall.h"
  
  // compute the product of two int32_t values
  // store the high word of the int64_t result in *z
  // return the low word of the int64_t result
  static inline int32_t _dx_imul64(int32_t x, int32_t y, int32_t* z) {
    return _dx_imul64_x86_fastcall(x, y, z);
  }

#endif

#endif // CORE_INTRINSIC__IMUL64__IMUL64_H_INCLUDED
