#if !defined(DX_CORE_CONVERT_H_INCLUDED)
#define DX_CORE_CONVERT_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup core
/// @brief Convert a string to a <code>Core_Integer8</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Integer8</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_i8(char const* p, Core_Size n, Core_Integer8* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Natural8</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Natural8</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_n8(char const* p, Core_Size n, Core_Natural8* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Integer16</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Integer16</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_i16(char const* p, Core_Size n, Core_Integer16* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Natural16</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Natural16</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_n16(char const* p, Core_Size n, Core_Natural16* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Integer32</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Integer32</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_i32(char const* p, Core_Size n, Core_Integer32* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Natural32</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Natural32</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_n32(char const* p, Core_Size n, Core_Natural32* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Integer64</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Integer64</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_i64(char const* p, Core_Size n, Core_Integer64* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Natural64</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Natural64</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_n64(char const* p, Core_Size n, Core_Natural64* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Size</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Size</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_sz(char const* p, Core_Size n, Core_Size* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Real32</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Real32</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_f32(char const* p, Core_Size n, Core_Real32* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Real64</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Real64</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_f64(char const* p, Core_Size n, Core_Real64* target);

/// @ingroup core
/// @brief Convert a string to a <code>Core_Boolean</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>Core_Boolean</code> variable.
/// @error #Core_Error_ArgumentInvalid @a target was not dereferenced.
/// @procedure
Core_Result dx_convert_utf8bytes_to_bool(char const* p, Core_Size n, Core_Boolean* target);

#endif // DX_CORE_CONVERT_H_INCLUDED
