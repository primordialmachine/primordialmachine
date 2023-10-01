#if !defined(DX_NEXT_POWER_OF_TWO_H_INCLUDED)
#define DX_NEXT_POWER_OF_TWO_H_INCLUDED

#include "dx/core/core.h"

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>dx_n8</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than @a x.
dx_result dx_next_power_of_two_gt_n8(dx_n8* RETURN, dx_n8 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>dx_n8</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_result dx_next_power_of_two_gte_n8(dx_n8* RETURN, dx_n8 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>dx_n16</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than @a x.
dx_result dx_next_power_of_two_gt_n16(dx_n16* RETURN, dx_n16 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>dx_n16</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_result dx_next_power_of_two_gte_n16(dx_n16* RETURN, dx_n16 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>dx_n32</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than @a x.
dx_result dx_next_power_of_two_gt_n32(dx_n32* RETURN, dx_n32 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>dx_n32</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_result dx_next_power_of_two_gte_n32(dx_n32* RETURN, dx_n32 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>dx_n64</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than @a x.
dx_result dx_next_power_of_two_gt_n64(dx_n64* RETURN, dx_n64 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>dx_n64</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_result dx_next_power_of_two_gte_n64(dx_n64* RETURN, dx_n64 x);

/// @brief Get the next power of two greater than the specified value.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param x The value.
/// @success <code>*RETURN<code> was assigned the next power of two greater than @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than @a x.
dx_result dx_next_power_of_two_gt_sz(dx_size* RETURN, dx_size x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the next power of two greater than or equal to @a x.
/// @procedure-call
/// @error #DX_ERROR_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_result dx_next_power_of_two_gte_sz(dx_size* RETURN, dx_size x);

#if defined(DX_NEXT_POWER_OF_TWO_WITH_TESTS) && 1 == DX_NEXT_POWER_OF_TWO_WITH_TESTS

dx_result dx_next_power_of_two_tests();

#endif

#endif // DX_NEXT_POWER_OF_TWO_H_INCLUDED
