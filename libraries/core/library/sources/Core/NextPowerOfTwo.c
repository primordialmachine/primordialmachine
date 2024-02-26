// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/NextPowerOfTwo.h"

#include "Core/CountLeadingZeroes.h"

Core_Result Core_nextPowerOfTwoGtN8(Core_Natural8* RETURN, Core_Natural8 x) {
  static Core_Natural8 const greatestPowerOfTwo = ((Core_Natural8)1) << 7;
  if (x < 1) {
    *RETURN = 1;
    return Core_Success;
  }
  if (x >= greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  Core_Size i;
  Core_countLeadingZeroesN8(&i, x); // must succeed
  Core_Natural8 t = ((Core_Natural8)1) << (8 - i);
  if (i == 0 && t != x) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGteN8(Core_Natural8* RETURN, Core_Natural8 x) {
  static Core_Natural8 const greatestPowerOfTwo = ((Core_Natural8)1) << 7;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return Core_Success;
  }
  if (x > greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  Core_Natural8 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGtN16(Core_Natural16* RETURN, Core_Natural16 x) {
  static Core_Natural16 const greatestPowerOfTwo = ((Core_Natural16)1) << 15;
  if (x < 1) {
    *RETURN = 1;
    return Core_Success;
  }
  if (x >= greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  Core_Size i;
  Core_countLeadingZeroesN16(&i, x); // must succeed
  Core_Natural16 t = ((Core_Natural16)1) << (16 - i);
  if (i == 0 && t != x) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGteN16(Core_Natural16* RETURN, Core_Natural16 x) {
  static Core_Natural16 const greatestPowerOfTwo = ((Core_Natural16)1) << 15;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return Core_Success;
  }
  if (x > greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  Core_Natural16 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGtN32(Core_Natural32* RETURN, Core_Natural32 x) {
  static Core_Natural32 const greatestPowerOfTwo = ((Core_Natural32)1) << 31;
  if (x < 1) {
    *RETURN = 1;
    return Core_Success;
  }
  if (x >= greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  Core_Size i;
  Core_countLeadingZeroesN32(&i, x); // must succeed
  Core_Natural32 t = ((Core_Natural32)1) << (32 - i);
  if (i == 0 && t != x) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGteN32(Core_Natural32* RETURN, Core_Natural32 x) {
  static Core_Natural32 const greatestPowerOfTwo = ((Core_Natural32)1) << 31;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return Core_Success;
  }
  if (x > greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  Core_Natural32 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGtN64(Core_Natural64* RETURN, Core_Natural64 x) {
  static Core_Natural64 const greatestPowerOfTwo = ((Core_Natural64)1) << 63;
  if (x < 1) {
    *RETURN = 1;
    return Core_Success;
  }
  if (x >= greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  Core_Size i;
  Core_countLeadingZeroesN64(&i, x); // must succeed
  Core_Natural64 t = ((Core_Natural64)1) << (64 - i);
  if (i == 0 && t != x) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGteN64(Core_Natural64* RETURN, Core_Natural64 x) {
  static Core_Natural64 const greatestPowerOfTwo = ((Core_Natural64)1) << 63;
  if (x < 2) {
    // x = 0 and x = 1 both map to 1.
    *RETURN = 1;
    return Core_Success;
  }
  if (x > greatestPowerOfTwo) {
    Core_setError(Core_Error_NumericOverflow);
    return Core_Failure;
  }
  // x is either smaller than or equal to greatest power of two representable by the type.
  // consequently, the following loop will terminate as the condition y >= x will certainly be reached.
  Core_Natural64 y = 2;
  while (y < x) {
    y <<= 1;
  }
  *RETURN = y;
  return Core_Success;
}

Core_Result Core_nextPowerOfTwoGtSz(Core_Size* RETURN, Core_Size x) {
#if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
  #if defined(_M_X64)
    return Core_nextPowerOfTwoGtN64(RETURN, x);
  #elif defined(_M_IX86)
    return Core_nextPowerOfTwoGtN32(RETURN, x);
  #else
    #error("environment not (yet) supported")
  #endif
#elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem
  #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(_x86_64)
    return Core_nextPowerOfTwoGtN64(RETURN, x);
  #else
    return Core_nextPowerOfTwoGtN32(RETURN, x);
  #endif
#else
  #error("environment not (yet) supported")
#endif
}

Core_Result Core_nextPowerOfTwoGteSz(Core_Size* RETURN, Core_Size x) {
#if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
  #if defined(_M_X64)
    return Core_nextPowerOfTwoGteN64(RETURN, x);
  #elif defined(_M_IX86)
    return Core_nextPowerOfTwoGteN32(RETURN, x);
  #else
    #error("environment not (yet) supported")
  #endif
#elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem
  #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(_x86_64)
    return Core_nextPowerOfTwoGteN64(RETURN, x);
  #else
    return Core_nextPowerOfTwoGteN32(RETURN, x);
  #endif
#else
  #error("environment not (yet) supported")
#endif
}

