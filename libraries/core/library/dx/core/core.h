#if !defined(DX_CORE_CORE_H_INCLUDED)
#define DX_CORE_CORE_H_INCLUDED

#include "dx/core/annotations.h"
#include "dx/core/configuration.h"
#include "dx/core/error.h"
#include "dx/core/builtin_types.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// assert
#include <assert.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Macro causing a debug break if the expression evaluates to logically false.
/// @param expression The expression.
#define DX_DEBUG_ASSERT(expression) assert(expression)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief This must be aligned to 64-bit boundaries on 64-bit systems and to 32-bit boundaries on 32-bit systems.
#if defined(_WIN64)
  typedef int64_t dx_reference_counter;
#else
  typedef int32_t dx_reference_counter;
#endif

// @return The resulting incremented value.
dx_reference_counter dx_reference_counter_increment(dx_reference_counter* reference_counter);

// @return The resulting decremented value.
dx_reference_counter dx_reference_counter_decrement(dx_reference_counter* reference_counter);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Write an utf-8 string to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The length of the array.
/// @pre
/// - @a p points to an array of at least @a n Bytes and
/// - the first @a n Bytes of that array are a valid utf-8 string
/// @pre
/// - utf8 is supported by standard output
void dx_log(char const *p, dx_size n);

/// @{
/// @brief Write a number to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param value The number.

void dx_log_i64(dx_i64 value);

void dx_log_i32(dx_i32 value);

void dx_log_i16(dx_i16 value);

void dx_log_i8(dx_i8 value);

void dx_log_n64(dx_n64 value);

void dx_log_n32(dx_n32 value);

void dx_log_n16(dx_n16 value);

void dx_log_n8(dx_n8 value);

/// @}

/// @brief Write a pointer to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param p The pointer.
void dx_log_p(void const* p);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @{
/// @ingroup core
/// @brief Compare two floating-point values.
/// @param a,b The floating-point values to compare.
/// @param epsilon An epsilon used by some comparison methods.
/// Must not be nan.
/// @param method
/// Denotes the method used for comparison.
/// The following methods are available:
/// - @a 1: <code>a == b direct comparison</code>
/// - @a 2: <code>|a-b| &leq; epsilon</code>
/// - @a 3: <code>|a-b| &leq; epsilon * max(|a|,|b|)</code>
/// In either method, a and b are considered equal if they are both nan
/// @return @a true if @a a and @a b are equal.
/// @a false if they are not equal or an error occurred.
bool dx_almost_equal_f32(dx_f32 a, dx_f32 b, int method, dx_f32 epsilon);
bool dx_almost_equal_f64(dx_f64 a, dx_f64 b, int method, dx_f64 epsilon);
/// @}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Compute the hash value of a pointer.
/// @param x The pointer.
/// @return The hash value.
dx_size dx_hash_pointer(void const* );

/// @ingroup core
/// @brief Compute the hash value of a <code>dx_bool</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_bool(dx_bool x);

/// @ingroup core
/// @brief Compute the hash value of an array of Bytes.
/// @param p A pointer to the array of Bytes.
/// @param n The length of the array.
/// @return The hash value.
dx_size dx_hash_bytes(void const* p, dx_size n);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_f32</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_f32(dx_f32 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_f64</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_f64(dx_f64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i8</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i8(dx_i8 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i16</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i16(dx_i16 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i32</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i32(dx_i32 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i64</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i64(dx_i64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n8</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n8(dx_n8 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n16</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n16(dx_n16 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n32</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n32(dx_n32 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n64</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n64(dx_n64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_size</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_sz(dx_size x);

/// @ingroup core
/// @brief Combine two hash values.
/// @param x The first hash value.
/// @param y The second hash value.
/// @return The combination of the first hash value and the second hash value.
dx_size dx_combine_hash(dx_size x, dx_size y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @{
/// @ingroup core
/// @brief Get if a floating point value is a subnormal value.
/// @param x The floating point value.
/// @return @a true if the floating point value is a subnormal value, @a false otherwise.
dx_bool dx_fp32_is_subnormal(dx_f32 x);
dx_bool dx_fp64_is_subnormal(dx_f64 x);
/// @}

/// @{
/// @ingroup core
/// @brief Get if a floating point value is a not a number value.
/// @param x The floating point value.
/// @return @a true if the floating point value is a not a number value, @a false otherwise.
dx_bool dx_fp32_is_nan(dx_f32 x);
dx_bool dx_fp64_is_nan(dx_f64 x);
/// @}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_CORE_H_INCLUDED
