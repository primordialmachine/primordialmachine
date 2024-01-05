// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_MAKEBITMASK_H_INCLUDED)
#define CORE_MAKEBITMASK_H_INCLUDED

#include "Core/FundamentalTypes.h"

// Create a Bit mask for the Bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_makeBitMaskN8(Core_Natural8* RETURN, Core_Size i, Core_Size n);

// Clear the bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_clearBitsN8(Core_Natural8* RETURN, Core_Natural8 v, Core_Size i, Core_Size n);

// Create a mask for the Bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_makeBitMaskN16(Core_Natural16* RETURN, Core_Size i, Core_Size n);

// Clear the bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_clearBitsN16(Core_Natural16* RETURN, Core_Natural16 v, Core_Size i, Core_Size n);

// Create a mask for the Bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_makeBitMaskN32(Core_Natural32* RETURN, Core_Size i, Core_Size n);

// Clear the bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_clearBitsN32(Core_Natural32* RETURN, Core_Natural32 v, Core_Size i, Core_Size n);

// Create a mask for the Bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_makeBitMaskN64(Core_Natural64* RETURN, Core_Size i, Core_Size n);

// Clear the bits [i, i + n).
// @param i The index of the first Bits.
// @param i The number of Bits.
Core_Result Core_clearBitsN64(Core_Natural64* RETURN, Core_Natural64 v, Core_Size i, Core_Size n);

#endif // CORE_MAKEBITMASK_H_INCLUDED
