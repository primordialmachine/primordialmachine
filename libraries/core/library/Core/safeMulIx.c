// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/safeMulIx.h"

#if defined(Core_safeMulI8_withAsm) && 1 == Core_safeMulI8_withAsm
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_imul16.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_imul16.h"
  #else
    #error("environment not supported")
  #endif
#endif

Core_Result Core_safeMulI8(int8_t* RETURN, int8_t x, int8_t y, int8_t* z) {
#if defined(Core_safeMulI8_withAsm) && 1 == Core_safeMulI8_withAsm
  int8_t lo, hi;
  lo = _dx_imul16(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  int16_t t = (int16_t)x * (int16_t)y;
  *z = (int8_t)((t & 0xff00) >> 8);
  *RETURN = (int8_t)(t & 0x00ff);
#endif
  return Core_Success;
}

#if defined(Core_safeMulI16_withAsm) && 1 == Core_safeMulI16_withAsm
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_imul32.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_imul32.h"
  #else
    #error("environment not (yet) supported")
  #endif
#endif

Core_Result Core_safeMulI16(int16_t* RETURN, int16_t x, int16_t y, int16_t* z) {
#if defined(Core_safeMulI16_withAsm) && 1 == Core_safeMulI16_withAsm
  int16_t lo, hi;
  lo = _dx_imul32(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  int32_t t = (int32_t)x * (int32_t)y;
  *z = (int16_t)((t & 0xffff0000) >> 16);
  *RETURN = (int16_t)(t & 0x0000ffff);
#endif
  return Core_Success;
}

#if defined(Core_safeMulI32_withAsm) && 1 == Core_safeMulI32_withAsm
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_imul64.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_imul64.h"
  #else
    #error("environment not supported")
  #endif
#endif

Core_Result Core_safeMulI32(int32_t* RETURN, int32_t x, int32_t y, int32_t* z) {
#if defined(Core_safeMulI32_withAsm) && 1 == Core_safeMulI32_withAsm
  int32_t lo, hi;
  lo = _dx_imul64(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  int64_t t = (int64_t)x * (int64_t)y;
  *z = (int32_t)((t & 0xffffffff00000000) >> 32);
  *RETURN = (int32_t)(t & 0x00000000ffffffff);
#endif
  return Core_Success;
}

#if defined(Core_safeMulI64_withAsm) && 1 == Core_safeMulI64_withAsm
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_imul128.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_imul128.h"
  #else
    #error("environment not (yet) supported")
  #endif
#endif

Core_Result Core_safeMulI64(int64_t* RETURN, int64_t x, int64_t y, int64_t* z) {
#if defined(Core_safeMulI64_withAsm) && 1 == Core_safeMulI64_withAsm
  int64_t lo, hi;
  lo = _dx_imul128(x, y, &hi);
  *z = hi;
  *RETURN = lo;
#else
  #error("environment not (yet) supported")
#endif
  return Core_Success;
}
