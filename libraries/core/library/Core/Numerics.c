// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "Core/Numerics.h"

// sinf, sin
// cosf, cos
#include <math.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_minN8(Core_Natural8* RETURN, Core_Natural8 x, Core_Natural8 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minN16(Core_Natural16* RETURN, Core_Natural16 x, Core_Natural16 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minN32(Core_Natural32* RETURN, Core_Natural32 x, Core_Natural32 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minN64(Core_Natural64* RETURN, Core_Natural64 x, Core_Natural64 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minI8(Core_Integer8* RETURN, Core_Integer8 x, Core_Integer8 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minI16(Core_Integer16* RETURN, Core_Integer16 x, Core_Integer16 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minI32(Core_Integer32* RETURN, Core_Integer32 x, Core_Integer32 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minI64(Core_Integer64* RETURN, Core_Integer64 x, Core_Integer64 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

Core_Result Core_minR32(Core_Real32* RETURN, Core_Real32 x, Core_Real32 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  if (isnan(x) || isnan(y)) {
    *RETURN = Core_Real32_NoNumber;
  } else {
    *RETURN = x < y ? x : y;
  }
  return Core_Success;
}

Core_Result Core_minR64(Core_Real64* RETURN, Core_Real64 x, Core_Real64 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  if (isnan(x) || isnan(y)) {
    *RETURN = Core_Real64_NoNumber;
  } else {
    *RETURN = x < y ? x : y;
  }
  return Core_Success;
}

Core_Result Core_minSz(Core_Size* RETURN, Core_Size x, Core_Size y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x < y ? x : y;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_maxN8(Core_Natural8* RETURN, Core_Natural8 x, Core_Natural8 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxN16(Core_Natural16* RETURN, Core_Natural16 x, Core_Natural16 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxN32(Core_Natural32* RETURN, Core_Natural32 x, Core_Natural32 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxN64(Core_Natural64* RETURN, Core_Natural64 x, Core_Natural64 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxI8(Core_Integer8* RETURN, Core_Integer8 x, Core_Integer8 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxI16(Core_Integer16* RETURN, Core_Integer16 x, Core_Integer16 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxI32(Core_Integer32* RETURN, Core_Integer32 x, Core_Integer32 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxI64(Core_Integer64* RETURN, Core_Integer64 x, Core_Integer64 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

Core_Result Core_maxR32(Core_Real32* RETURN, Core_Real32 x, Core_Real32 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  if (isnan(x) || isnan(y)) {
    *RETURN = Core_Real32_NoNumber;
  } else {
    *RETURN = x > y ? x : y;
  }
  return Core_Success;
}

Core_Result Core_maxR64(Core_Real64* RETURN, Core_Real64 x, Core_Real64 y) {
  if (!RETURN) {
    return Core_Failure;
  }
  if (isnan(x) || isnan(y)) {
    *RETURN = Core_Real64_NoNumber;
  } else {
    *RETURN = x > y ? x : y;
  }
  return Core_Success;
}

Core_Result Core_maxSz(Core_Size* RETURN, Core_Size x, Core_Size y) {
  if (!RETURN) {
    return Core_Failure;
  }
  *RETURN = x > y ? x : y;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_sinR32(Core_Real32* RETURN, Core_Real32 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (isinf(x) || isnan(x)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = sinf(x);
  return Core_Success;
}

Core_Result Core_sinR64(Core_Real64* RETURN, Core_Real64 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (isinf(x) || isnan(x)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = sin(x);
  return Core_Success;
}

Core_Result Core_cosR32(Core_Real32* RETURN, Core_Real32 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (isinf(x) || isnan(x)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = cosf(x);
  return Core_Success;
}

Core_Result Core_cosR64(Core_Real64* RETURN, Core_Real64 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (isinf(x) || isnan(x)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = cos(x);
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_floorR32(Core_Real32* RETURN, Core_Real32 x) {
  *RETURN = floorf(x);
  return Core_Success;
}

Core_Result Core_floorR64(Core_Real64* RETURN, Core_Real64 x) {
  *RETURN = floor(x);
  return Core_Success;
}

Core_Result Core_ceilR32(Core_Real32* RETURN, Core_Real32 x) {
  *RETURN = ceilf(x);
  return Core_Success;
}

Core_Result Core_ceilR64(Core_Real64* RETURN, Core_Real64 x) {
  *RETURN = ceil(x);
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
