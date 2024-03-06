// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/safeMulNx.h"

#if defined(Core_safeMulN8_withAsm) && 1 == Core_safeMulN8_withAsm
  #include "Core/Intrinsic/_umul16/_umul16.h"
#endif

Core_Result Core_safeMulN8(Core_Natural8* RETURN, Core_Natural8 x, Core_Natural8 y, Core_Natural8* z) {
  if (!RETURN || !z) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
#if defined(Core_safeMulN8_withAsm) && 1 == Core_safeMulN8_withAsm
  Core_Natural8 lo, hi;
  lo = _dx_umul16(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  Core_Natural16 t = (Core_Natural16)x * (Core_Natural16)y;
  *z = (Core_Natural8)((t & 0xff00) >> 8);
  *RETURN = (Core_Natural8)(t & 0x00ff);
#endif
  return Core_Success;
}

#if defined(Core_safeMulN16_withAsm) && 1 == Core_safeMulN16_withAsm
  #include "Core/Intrinsic/_umul32/_umul32.h"
#endif

Core_Result Core_safeMulN16(Core_Natural16* RETURN, Core_Natural16 x, Core_Natural16 y, Core_Natural16* z) {
  if (!RETURN || !z) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
#if defined(Core_safeMulN16_withAsm) && 1 == Core_safeMulN16_withAsm
  Core_Natural16 lo, hi;
  lo = _dx_umul32(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  Core_Natural32 t = (Core_Natural32)x * (Core_Natural32)y;
  *z = (Core_Natural16)((t & 0xffff0000) >> 16);
  *RETURN = (Core_Natural16)(t & 0x0000ffff);
#endif
  return Core_Success;
}

#if defined(Core_safeMulN32_withAsm) && 1 == Core_safeMulN32_withAsm
  #include "Core/Intrinsic/_umul64/_umul64.h"
#endif

Core_Result Core_safeMulN32(Core_Natural32* RETURN, Core_Natural32 x, Core_Natural32 y, Core_Natural32* z) {
  if (!RETURN || !z) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
#if defined(Core_safeMulN32_withAsm) && 1 == Core_safeMulN32_withAsm
  Core_Natural32 lo, hi;
  lo = _dx_umul64(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  Core_Natural64 t = (Core_Natural64)x * (Core_Natural64)y;
  *z = (Core_Natural32)((t & 0xffffffff00000000) >> 32);
  *RETURN = (Core_Natural32)(t & 0x00000000ffffffff);
#endif
  return Core_Success;
}

#if Core_OperatingSystem_Windows == Core_OperatingSystem
  #if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture
    #include "Core/Intrinsic/_umul128/_umul128.h"
  #elif Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture
    #include "Core/Intrinsic/_umul128/_umul128.h"
  #else
    #error("environment not (yet) supported")
  #endif
#endif

Core_Result Core_safeMulN64(Core_Natural64* RETURN, Core_Natural64 x, Core_Natural64 y, Core_Natural64* z) {
  if (!RETURN || !z) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Natural64 lo, hi;
  lo = _dx_umul128(x, y, &hi);
  *z = hi;
  *RETURN = lo;
  return Core_Success;
}

Core_Result Core_safeMulSz(Core_Size* RETURN, Core_Size x, Core_Size y, Core_Size* z) {
#if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture
  return Core_safeMulN64(RETURN, x, y, z);
#elif Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture
  return Core_safeMulN32(RETURN, x, y, z);
#else
  #error("environment not (yet) supported")
#endif
}
