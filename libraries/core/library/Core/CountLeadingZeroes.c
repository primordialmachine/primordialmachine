// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#include "Core/CountLeadingZeroes.h"

#if defined(_MSC_VER)
  #include <intrin.h>
#endif

Core_Result Core_countLeadingZeroesN8(Core_Size* RETURN, Core_Natural8 x) {
  // Maps a value from 0000 (0x0) to 1111 (0xf) the value's number of leading bits.
  static const short lookup[16] = {
    4, //  0: 0000
    3, //  1: 0001
    2, //  2: 0010
    2, //  3: 0011
    1, //  4: 0100
    1, //  5: 0101
    1, //  6: 0110
    1, //  7: 0111
    0, //  8: 1000
    0, //  9: 1001
    0, // 10: 1010
    0, // 11: 1011
    0, // 12: 1100
    0, // 13: 1101
    0, // 14: 1110
    0, // 15: 1111
  };
  uint8_t upper = (x & 0xf0) >> 4;
  uint8_t lower = (x & 0x0f) >> 0;
  *RETURN = upper ? lookup[upper] : 4 + lookup[lower];
  return Core_Success;
}

Core_Result Core_countLeadingZeroesN16(Core_Size* RETURN, Core_Natural16 x) {
  Core_Natural16 t;
  t = (x & 0xff00) >> 8;
  if (t) {
    return Core_countLeadingZeroesN8(RETURN, (Core_Natural8)t);
  }
  t = (x & 0x00ff) >> 0;
  Core_Size temporary;
  Core_countLeadingZeroesN8(&temporary, (Core_Natural8)t);
  *RETURN = 8 + temporary;
  return Core_Success;
}

Core_Result Core_countLeadingZeroesN32(Core_Size* RETURN, Core_Natural32 x) {
#if defined(_MSC_VER)
  unsigned long n;
  if (_BitScanReverse(&n, x)) {
    // x was not zero.
    *RETURN = (short)32 - ((short)n + 1);
  } else {
    // x was zero.
    *RETURN = (short)32;
  }
#else
  Core_Natural32 t;
  t = (x & 0xffff0000) >> 16;
  if (t) {
    return Core_countLeadingZeroesN16(RETURN, (Core_Natural16)t);
  }
  t = (x & 0x0000ffff) >> 0;
  Core_Size temporary;
  Core_countLeadingZeroesN16(&temporary, (Core_Natural16)t);
  *RETURN = 16 + temporary;
#endif
  return Core_Success;
}

Core_Result Core_countLeadingZeroesN64(Core_Size* RETURN, Core_Natural64 x) {
  Core_Natural32 t;
  t = (x & 0xffffffff00000000) >> 32;
  if (t) {
    return Core_countLeadingZeroesN32(RETURN, (Core_Natural32)t);
  }
  t = (x & 0x00000000ffffffff) >> 0;
  Core_Size temporary;
  Core_countLeadingZeroesN32(&temporary, (Core_Natural32)t);
  *RETURN = 32 + temporary;
  return Core_Success;
}

Core_Result Core_countLeadingZeroesSz(Core_Size* RETURN, Core_Size x) {
#if defined(_M_X64)
  return Core_countLeadingZeroesN64(RETURN, x);
#elif defined(_M_IX86)
  return Core_countLeadingZeroesN32(RETURN, x);
#else
  #error("environment not (yet) supported")
#endif
}
