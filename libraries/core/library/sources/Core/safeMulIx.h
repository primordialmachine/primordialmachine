// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_SAFEMULIX_H_INCLUDED)
#define CORE_SAFEMULIX_H_INCLUDED

#include "Core/FundamentalTypes.h"

/// @ingroup core
/// Compute the product of two <code>int8_t</code> values and return the low and the high word of the result separatly.
/// @param RETURN A pointer to a <code>int8_t</code> variable.
/// @param x The multiplier/first factor.
/// @param y The multiplicand/second factor.
/// @param [out] c A pointer to a <code>int8_t</variable>.
/// @success
/// The high word of the product was assigned to <code>*c</code>.
/// The low word of the product was assigned to <code>*RETURN</code>.
/// @error Core_Error_ArgumentInvalid <code>RETURN</code> is a null pointer.
/// @error Core_Error_ArgumentInvalid <code>z</code> is a null pointer.
/// @warning If <code>c</code> does not point to a <code>int8_t</code> variable, the behavior of this function is undefined.
Core_Result Core_safeMulI8(Core_Integer8* RETURN, Core_Integer8 x, Core_Integer8 y, Core_Integer8* z);

Core_Result Core_safeMulI16(Core_Integer16* RETURN, Core_Integer16 x, Core_Integer16 y, Core_Integer16* z);

Core_Result Core_safeMulI32(Core_Integer32* RETURN, Core_Integer32 x, Core_Integer32 y, Core_Integer32* z);

Core_Result Core_safeMulI64(Core_Integer64* RETURN, Core_Integer64 x, Core_Integer64 y, Core_Integer64* z);

#endif // CORE_SAFEMULIX_H_INCLUDED
