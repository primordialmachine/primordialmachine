// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/safeAddNx.h"

Core_Result Core_safeAddN8(Core_Natural8* RETURN, Core_Natural8 a, Core_Natural8 b, Core_Natural8* c) {
  if (!RETURN || !c) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Natural8 t = a + b;
  *c = Core_Natural8_Greatest - a < b; // if there is an overflow (aka a "high word"), then all bits are zero except of the lsb which is 1.
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_safeAddN16(Core_Natural16* RETURN, Core_Natural16 a, Core_Natural16 b, Core_Natural16* c) {
  Core_Natural16 t = a + b;
  *c = Core_Natural16_Greatest - a < b; // if there is an overflow (aka a "high word"), then all bits are zero except of the lsb which is 1.
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_safeAddN32(Core_Natural32* RETURN, Core_Natural32 a, Core_Natural32 b, Core_Natural32* c) {
  Core_Natural32 t = a + b;
  *c = Core_Natural32_Greatest - a < b; // if there is an overflow (aka a "high word"), then all bits are zero except of the lsb which is 1.
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_safeAddN64(Core_Natural64* RETURN, Core_Natural64 a, Core_Natural64 b, Core_Natural64* c) {
  Core_Natural64 t = a + b;
  *c = Core_Natural64_Greatest - a < b; // if there is an overflow (aka a "high word"), then all bits are zero except of the lsb which is 1.
  *RETURN = t;
  return Core_Success;
}

Core_Result Core_safeAddSz(Core_Size* RETURN, Core_Size a, Core_Size b, Core_Size* c) {
  Core_Size t = a + b;
  *c = Core_Size_Greatest - a < b; // if there is an overflow (aka a "non-zero high word"), then all bits are zero except of the lsb which is 1.
  *RETURN = t;
  return Core_Success;
}
