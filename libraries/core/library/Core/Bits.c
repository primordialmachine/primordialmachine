// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Bits.h"

Core_Result Core_makeBitMaskN8(Core_Natural8* RETURN, Core_Size i, Core_Size n) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= 8) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > 8) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // "mask" is initially empty.
  Core_Natural8 mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Core_Size j = i, m = i + n; j < m; ++j) {
    mask |= UINT8_C(1) << (8 - j - 1);
  }
  *RETURN = mask;
  return Core_Success;
}

Core_Result Core_clearBitsN8(Core_Natural8* RETURN, Core_Natural8 v, Core_Size i, Core_Size n) {
  Core_Natural8 mask = 0;
  if (Core_makeBitMaskN8(&mask, i, n)) {
    return Core_Failure;
  }
  // Invert the bits of the mask.
  mask = ~mask;
  // Perform masking.
  *RETURN = v & mask;
  // Succeed.
  return Core_Success;
}

Core_Result Core_makeBitMaskN16(Core_Natural16* RETURN, Core_Size i, Core_Size n) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= 16) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > 16) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // "mask" is initially empty.
  Core_Natural16 mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Core_Size j = i, m = i + n; j < m; ++j) {
    mask |= UINT16_C(1) << (16 - j - 1);
  }
  *RETURN = mask;
  return Core_Success;
}

Core_Result Core_clearBitsN16(Core_Natural16* RETURN, Core_Natural16 v, Core_Size i, Core_Size n) {
  Core_Natural16 mask = 0;
  if (Core_makeBitMaskN16(&mask, i, n)) {
    return Core_Failure;
  }
  // Invert the bits in the mask.
  mask = ~mask;
  // Perform masking.
  *RETURN = v & mask;
  // Succeed.
  return Core_Success;
}

Core_Result Core_makeBitMaskN32(Core_Natural32* RETURN, Core_Size i, Core_Size n) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= 32) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > 32) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // "mask" is initially empty.
  Core_Natural32 mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Core_Size j = i, m = i + n; j < m; ++j) {
    mask |= UINT32_C(1) << (32 - j - 1);
  }
  *RETURN = mask;
  return Core_Success;
}

Core_Result Core_clearBitsN32(Core_Natural32* RETURN, Core_Natural32 v, Core_Size i, Core_Size n) {
  Core_Natural32 mask = 0;
  if (Core_makeBitMaskN32(&mask, i, n)) {
    return Core_Failure;
  }
  // Invert the bits in the mask.
  mask = ~mask;
  // Perform masking.
  *RETURN = v & mask;
  // Succeed.
  return Core_Success;
}

Core_Result Core_makeBitMaskN64(Core_Natural64* RETURN, Core_Size i, Core_Size n) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= 64) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > 64) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // "mask" is initially empty.
  Core_Natural64 mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Core_Size j = i, m = i + n; j < m; ++j) {
    mask |= UINT64_C(1) << (64 - j - 1);
  }
  *RETURN = mask;
  return Core_Success;
}

Core_Result Core_clearBitsN64(Core_Natural64* RETURN, Core_Natural64 v, Core_Size i, Core_Size n) {
  Core_Natural64 mask = 0;
  if (Core_makeBitMaskN64(&mask, i, n)) {
    return Core_Failure;
  }
  // Invert the bits in the mask.
  mask = ~mask;
  // Perform masking.
  *RETURN = v & mask;
  // Succeed.
  return Core_Success;
}
