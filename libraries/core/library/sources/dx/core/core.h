#if !defined(DX_CORE_CORE_H_INCLUDED)
#define DX_CORE_CORE_H_INCLUDED

#include "Core/Annotations.h"
#include "dx/core/configuration.h"
#include "Core/FundamentalTypes.h"
#include "Core/Error.h"
#include "Core/Result.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Macro providing the name of the "self" variable.
#define SELF _self

/// @brief Macro providing the name of the "return" variable.
#define RETURN _return

/// @brief Macro providing the name of the "type" variable.
#define TYPE _type

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// assert
#include <assert.h>

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
void dx_log(char const *p, Core_Size n);

/// @{
/// @brief Write a number to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param value The number.

void dx_log_i64(Core_Integer64 value);

void dx_log_i32(Core_Integer32 value);

void dx_log_i16(Core_Integer16 value);

void dx_log_i8(Core_Integer8 value);

void dx_log_n64(Core_Natural64 value);

void dx_log_n32(Core_Natural32 value);

void dx_log_n16(Core_Natural16 value);

void dx_log_n8(Core_Natural8 value);

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
bool dx_almost_equal_f32(Core_Real32 a, Core_Real32 b, int method, Core_Real32 epsilon);
bool dx_almost_equal_f64(Core_Real64 a, Core_Real64 b, int method, Core_Real64 epsilon);
/// @}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @{
/// @ingroup core
/// @brief Get if a floating point value is a subnormal value.
/// @param x The floating point value.
/// @return @a true if the floating point value is a subnormal value, @a false otherwise.
Core_Boolean dx_fp32_is_subnormal(Core_Real32 x);
Core_Boolean dx_fp64_is_subnormal(Core_Real64 x);
/// @}

/// @{
/// @ingroup core
/// @brief Get if a floating point value is a not a number value.
/// @param x The floating point value.
/// @return @a true if the floating point value is a not a number value, @a false otherwise.
Core_Boolean dx_fp32_is_nan(Core_Real32 x);
Core_Boolean dx_fp64_is_nan(Core_Real64 x);
/// @}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_CORE_H_INCLUDED
