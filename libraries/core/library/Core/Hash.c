// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#include "Core/Hash.h"

#include <math.h>

Core_Result Core_hashPointer(Core_Size* RETURN, void const* x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = (Core_Size)(uintptr_t)x;
  return Core_Success;
}

Core_Result Core_hashBytes(Core_Size* RETURN, void const* p, Core_Size n) {
  if (!RETURN || !p) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size h = n;
  Core_Size step = (n >> 5) + 1;
  for (; n >= step; n -= step) {
    h ^= (h << 5) + (h >> 2) + ((unsigned char)(((char const*)p)[n - 1]));
  }
  *RETURN = h;
  return Core_Success;
}


Core_Result Core_hashB(Core_Size* RETURN, Core_Boolean x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = x ? 1231 : 1237; // These are arbitrary prime number as e.g. used in Boolean.getHashCode in Java.
  return Core_Success;
}

Core_Result Core_hashR32(Core_Size* RETURN, Core_Real32 x) {
  static const Core_Boolean foldNoNumbers = Core_False;
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // This condition evaluates to false both for x being positive zero as well as x being negative zero.
  if (0.0 != x) {
    if (foldNoNumbers && isnan(x)) {
      *RETURN = (Core_Size)-3;
    } else {
      // If foldNoNumber is false the different "no numbers" do NOT map to the same hash value.
      return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Real32));
    }
  } else {
    // Both -0 and +0 map to 0.
    *RETURN = 0;
  }
  return Core_Success;
}

Core_Result Core_hashR64(Core_Size* RETURN, Core_Real64 x) {
  static const Core_Boolean foldNoNumbers = Core_False;
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // This condition evaluates to false both for x being positive zero as well as x being negative zero.
  if (0.0 != x) {
    if (foldNoNumbers && isnan(x)) {
      *RETURN = (Core_Size)-3;
    } else {
      // If foldNoNumber is false the different "no numbers" do NOT map to the same hash value.
      return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Real64));
    }
  } else {
    // Both -0 and +0 map to 0.
    *RETURN = 0;
  }
  return Core_Success;
}

Core_Result Core_hashI8(Core_Size* RETURN, Core_Integer8 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Integer8));
}

Core_Result Core_hashI16(Core_Size* RETURN, Core_Integer16 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Integer16));
}

Core_Result Core_hashI32(Core_Size* RETURN, Core_Integer32 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Integer32));
}

Core_Result Core_hashI64(Core_Size* RETURN, Core_Integer64 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Integer64));
}

Core_Result Core_hashN8(Core_Size* RETURN, Core_Natural8 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Natural8));
}

Core_Result Core_hashN16(Core_Size* RETURN, Core_Natural16 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Natural16));
}

Core_Result Core_hashN32(Core_Size* RETURN, Core_Natural32 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Natural32));
}

Core_Result Core_hashN64(Core_Size* RETURN, Core_Natural64 x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Natural64));
}

Core_Result Core_hashSz(Core_Size* RETURN, Core_Size x) {
  return Core_hashBytes(RETURN, (void const*)&x, sizeof(Core_Size));
}

Core_Result Core_combineHashes(Core_Size* RETURN, Core_Size x, Core_Size y) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // This is just another black-magic formula used by boost and glib.
  *RETURN = x + 0x9e3779b9 + (y << 6) + (y >> 2);
  return Core_Success;
}
