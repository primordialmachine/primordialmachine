// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_NEXTPOWEROFTWO_H_INCLUDED)
#define CORE_NEXTPOWEROFTWO_H_INCLUDED

#include "Core/FundamentalTypes.h"

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>Core_Natural8</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than @a x.
Core_Result Core_nextPowerOfTwoGtN8(Core_Natural8* RETURN, Core_Natural8 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>Core_Natural8</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than or equal to @a x.
Core_Result Core_nextPowerOfTwoGteN8(Core_Natural8* RETURN, Core_Natural8 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>Core_Natural16</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than @a x.
Core_Result Core_nextPowerOfTwoGtN16(Core_Natural16* RETURN, Core_Natural16 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>Core_Natural16</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than or equal to @a x.
Core_Result Core_nextPowerOfTwoGteN16(Core_Natural16* RETURN, Core_Natural16 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>Core_Natural32</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than @a x.
Core_Result Core_nextPowerOfTwoGtN32(Core_Natural32* RETURN, Core_Natural32 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>Core_Natural32</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than or equal to @a x.
Core_Result Core_nextPowerOfTwoGteN32(Core_Natural32* RETURN, Core_Natural32 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>Core_Natural64</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than @a x.
Core_Result Core_nextPowerOfTwoGtN64(Core_Natural64* RETURN, Core_Natural64 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>Core_Natural64</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than or equal to @a x.
Core_Result Core_nextPowerOfTwoGteN64(Core_Natural64* RETURN, Core_Natural64 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param x The value.
/// @success <code>*RETURN<code> was assigned the next power of two greater than @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than @a x.
Core_Result Core_nextPowerOfTwoGtSz(Core_Size* RETURN, Core_Size x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure
/// @error #Core_Error_Overflow if there is no representable next power of 2 greater than or equal to @a x.
Core_Result Core_nextPowerOfTwoGteSz(Core_Size* RETURN, Core_Size x);

#endif // CORE_NEXTPOWEROFTWO_H_INCLUDED
