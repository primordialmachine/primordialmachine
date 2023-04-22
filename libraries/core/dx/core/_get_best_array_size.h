#if !defined(DX_CORE_GET_BEST_ARRAY_SIZE_H_INCLUDED)
#define DX_CORE_GET_BEST_ARRAY_SIZE_H_INCLUDED

#include "dx/core/error.h"
#include "dx/core/builtin_types.h"

/// @brief Get the best size.
/// @param new A pointer to a <code>dx_size</code> variable.
/// @param current The current size.
/// @param additional The required additional size.
/// @param least The least size of an array.
/// @param greatest The greatest size of an array.
/// @param saturate if the computed best size is greater than the greatest size and this is true, then set
/// @remarks
/// Compute a new best size x for which greatest >= x >= max(current, least) holds.
/// The function works as follows:
///   - compute a best new value x >= max(current, least) that is a power of 2
///   - if that value is greater than greatest
///     - if saturate is true
///       - compute a best new value x = greatest
///     - else
///       - fail
dx_result dx_get_best_array_size(dx_size* RETURN, dx_size current, dx_size additional, dx_size least, dx_size greatest, dx_bool saturate);

#endif // DX_CORE_GET_BEST_ARRAY_SIZE_H_INCLUDED
