// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/safeMulIx.h"

#include "Core/Intrinsic/_imul16/_imul16.h"

Core_Result Core_safeMulI8(Core_Integer8* RETURN, Core_Integer8 x, Core_Integer8 y, Core_Integer8* z) {
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

#include "Core/Intrinsic/_imul32/_imul32.h"

Core_Result Core_safeMulI16(Core_Integer16* RETURN, Core_Integer16 x, Core_Integer16 y, Core_Integer16* z) {
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

#include "Core/Intrinsic/_imul64/_imul64.h"

Core_Result Core_safeMulI32(Core_Integer32* RETURN, Core_Integer32 x, Core_Integer32 y, Core_Integer32* z) {
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

#include "Core/Intrinsic/_imul128/_imul128.h"

Core_Result Core_safeMulI64(Core_Integer64* RETURN, Core_Integer64 x, Core_Integer64 y, Core_Integer64* z) {
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
