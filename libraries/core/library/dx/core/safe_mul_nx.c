#include "dx/core/safe_mul_nx.h"

#include "dx/core/configuration.h"

#if defined(DX_MUL_N8_ASM) && 1 == DX_MUL_N8_ASM
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_umul16.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_umul16.h"
  #else
    #error("environment not (yet) supported")
  #endif
#endif

uint8_t dx_mul_u8(uint8_t x, uint8_t y, uint8_t* z) {
#if defined(DX_MUL_N8_ASM) && 1 == DX_MUL_N8_ASM
  uint8_t lo, hi;
  lo = _dx_umul16(x, y, &hi);
  *z = hi;
  return lo;
#else
  uint16_t t = (uint16_t)x * (uint16_t)y;
  *z = (uint8_t)((t & 0xff00) >> 8);
  return (uint8_t)(t & 0x00ff);
#endif
}

#if defined(DX_MUL_N16_ASM) && 1 == DX_MUL_N16_ASM
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_umul32.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_umul32.h"
  #else
    #error("environment not (yet) supported")
  #endif
#endif

uint16_t dx_mul_u16(uint16_t x, uint16_t y, uint16_t* z) {
#if defined(DX_MUL_N16_ASM) && 1 == DX_MUL_N16_ASM
  uint16_t lo, hi;
  lo = _dx_umul32(x, y, &hi);
  *z = hi;
  return lo;
#else
  uint32_t t = (uint32_t)x * (uint32_t)y;
  *z = (uint16_t)((t & 0xffff0000) >> 16);
  return (uint16_t)(t & 0x0000ffff);
#endif
}

#if defined(DX_MUL_N32_ASM) && 1 == DX_MUL_N32_ASM
  #if defined(_M_X64)
    #include "dx/core/asm/x64/_umul64.h"
  #elif defined(_M_IX86)
    #include "dx/core/asm/x86/_umul64.h"
  #else
    #error("environment not (yet) supported")
  #endif
#endif

uint32_t dx_mul_u32(uint32_t x, uint32_t y, uint32_t* z) {
#if defined(DX_MUL_N32_ASM) && 1 == DX_MUL_N32_ASM
  uint32_t lo, hi;
  lo = _dx_umul64(x, y, &hi);
  *z = hi;
  return lo;
#else
  uint64_t t = (uint64_t)x * (uint64_t)y;
  *z = (uint32_t)((t & 0xffffffff00000000) >> 32);
  return (uint32_t)(t & 0x00000000ffffffff);
#endif
}

#if defined(_M_X64)
  #include "dx/core/asm/x64/_umul128.h"
#elif defined(_M_IX86)
  #include "dx/core/asm/x86/_umul128.h"
#else
  #error("environment not (yet) supported")
#endif

uint64_t dx_mul_u64(uint64_t x, uint64_t y, uint64_t* z) {
  uint64_t lo, hi;
  lo = _dx_umul128(x, y, &hi);
  *z = hi;
  return lo;
}

Core_Size dx_mul_sz(Core_Size a, Core_Size b, Core_Size* c) {
#if defined(_M_X64)
  return dx_mul_u64(a, b, c);
#elif defined(_M_IX86)
  return dx_mul_u32(a, b, c);
#else
  #error("environment not (yet) supported")
#endif
}
