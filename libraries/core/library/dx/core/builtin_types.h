/// @file dx/core/builtin_types.h
/// @brief Aliases for types builtin into the compiler.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_BUILTIN_TYPES_H_INCLUDED)
#define DX_CORE_BUILTIN_TYPES_H_INCLUDED

#include "dx/core/configuration.h"

// size_t, SIZE_MAX
#include <stddef.h>

// bool, true, false
#include <stdbool.h>

// uintx_t, UINTx_MAX
// intx_t, INTx_MIN, INTx_MAX
#include <inttypes.h>

// FLT_RADIX
#include <float.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_BuiltinTypes
/// @brief An alias for the <code>void</code> C type.
typedef void dx_void;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_BuiltinTypes
/// @brief A value that can assume two values indicating logically true and logically false.
/// @remark An alias for <code>bool</code>.
typedef bool dx_bool;
/// @ingroup Core_BuiltinTypes
/// @brief The value of type @a dx_bool indicating logically true.
#define DX_TRUE (true)
/// @ingroup Core_BuiltinTypes
/// @brief The value of type @a dx_bool indicating logically false.
#define DX_FALSE (false)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_BuiltinTypes
/// @brief A natural value of at least 32 bits width.
/// @remark An alias for <code>size_t</code>.
typedef size_t dx_size;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_size.
#define DX_SIZE_LEAST (0)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_size.
#define DX_SIZE_GREATEST (SIZE_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_BuiltinTypes
/// @brief A natural value of 8 bits width.
/// @remark An alias for <code>uint8_t</code>.
typedef uint8_t dx_n8;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_n8.
#define DX_N8_LEAST (0)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_n8.
#define DX_N8_GREATEST (UINT8_MAX)

/// @ingroup Core_BuiltinTypes
/// @brief A natural value of 16 bits width.
/// @remark An alias for <code>uint16_t</code>.
typedef uint16_t dx_n16;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_n16.
#define DX_N16_LEAST (0)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_n16.
#define DX_N16_GREATEST (UINT16_MAX)

/// @ingroup Core_BuiltinTypes
/// @brief A natural value of 32 bits width.
/// @remark An alias for <code>uint32_t</code>.
typedef uint32_t dx_n32;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_n32.
#define DX_N32_LEAST (0)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_n32.
#define DX_N32_GREATEST (UINT32_MAX)

/// @ingroup Core_BuiltinTypes
/// @brief A natural value of 64 bits width.
/// @remark An alias for <code>uint64_t</code>.
typedef uint64_t dx_n64;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_n64.
#define DX_N64_LEAST (0)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_n64.
#define DX_N64_GREATEST (UINT64_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_BuiltinTypes
/// @brief An integer value of 8 bits width.
/// @remark An alias for <code>int8_t</code>.
typedef int8_t dx_i8;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_i8.
#define DX_I8_LEAST (INT8_MIN)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_i8.
#define DX_I8_GREATEST (INT8_MAX)

/// @ingroup Core_BuiltinTypes
/// @brief An integer value of 16 bits width.
/// @remark An alias for <code>int16_t</code>.
typedef int16_t dx_i16;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_i16.
#define DX_I16_LEAST (INT16_MIN)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_i16.
#define DX_I16_GREATEST (INT16_MAX)

/// @ingroup Core_BuiltinTypes
/// @brief An integer value of 32 bits width.
/// @remark An alias for <code>int32_t</code>.
typedef int32_t dx_i32;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_i32.
#define DX_I32_LEAST (INT32_MIN)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_i32.
#define DX_I32_GREATEST (INT32_MAX)

/// @ingroup Core_BuiltinTypes
/// @brief An integer value of 64 bits width.
/// @remark An alias for <code>int64_t</code>.
typedef int64_t dx_i64;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_i64.
#define DX_I64_LEAST (INT64_MIN)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_i64.
#define DX_I64_GREATEST (INT64_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_BuiltinTypes
/// @brief A IEEE 754 32 bit floating point value.
/// @remark An alias for <code>float</code>.
typedef float dx_f32;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_f32.
#define DX_F32_LEAST (-FLT_MAX)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_f32.
#define DX_F32_GREATEST (FLT_MAX)
/// @ingroup Core_BuiltinTypes
/// @brief The "positive infinity" value of type @a dx_f32.
#define DX_F32_POSITIVE_INFINITY (+INFINITY)
/// @ingroup Core_BuiltinTypes
/// @brief The "negative infinity" value of type @a dx_f32.
#define DX_F32_NEGATIVE_INFINITY (-INFINITY)
/// @ingroup Core_BuiltinTypes
/// @brief The "no number" value of type @a dx_f32.
#define DX_F32_NO_NUMBER (NAN)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest integer such that <code>10</code> raised to that power is a normalized @a dx_f32 value.
#define DX_F32_GREATEST_10_EXP (FLT_MAX_10_EXP)
/// @ingroup Core_BuiltinTypes
/// @brief The least integer such that <code>10</code> raised to that power is it a normalized @a dx_f32 value.
#define DX_F32_LEAST_10_EXP (FLT_MIN_10_EXP)
// All machines we're aware of define this to 2 except of IBM 360 and derivatives.
#if defined(FLT_RADIX) && 2 == FLT_RADIX
  /// @ingroup Core_BuiltinTypes
  /// @brief The greatest integer such that <code>2</code> raised to that power is a normalized @a dx_f32 value.
  #define DX_F32_GREATEST_2_EXP (FLT_MAX_EXP)
  /// @ingroup Core_BuiltinTypes
  /// @brief The least integer such that <code>2</code> raised to that power is a normalized @a dx_f32 value.
  #define DX_F32_LEAST_2_EXP (FLT_MIN_EXP)
#else
  #error("environment not (yet) supported")
#endif

/// @ingroup Core_BuiltinTypes
/// @brief A IEEE 754 64 bit floating point value.
/// @remark An alias for <code>double</code>.
typedef double dx_f64;
/// @ingroup Core_BuiltinTypes
/// @brief The least value of the type @a dx_f64.
#define DX_F64_LEAST (-DLB_MAX)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest value of the type @a dx_f64.
#define DX_F64_GREATEST (DBL_MAX)
/// @ingroup Core_BuiltinTypes
/// @brief The "positive infinity" value of type @a dx_f64.
#define DX_F64_POSITIVE_INFINITY (+INFINITY)
/// @ingroup Core_BuiltinTypes
/// @brief The "negative infinity" value of type @a dx_f64.
#define DX_F64_NEGATIVE_INFINITY (-INFINITY)
/// @ingroup Core_BuiltinTypes
/// @brief The "no number" value of type @a dx_f64.
#define DX_F64_NO_NUMBER (NAN)
/// @ingroup Core_BuiltinTypes
/// @brief The greatest integer such that <code>10</code> raised to that power is a normalized @a dx_f64 value.
#define DX_F64_GREATEST_10_EXP (DBL_MAX_10_EXP)
/// @ingroup Core_BuiltinTypes
/// @brief The least integer such that <code>10</code> raised to that power is a normalized @a dx_f64 value.
#define DX_F64_LEAST_10_EXP (DBL_MIN_10_EXP)
// All machines we're aware of define this to 2 except of IBM 360 and derivatives.
#if defined(FLT_RADIX) && 2 == FLT_RADIX
  /// @ingroup Core_BuiltinTypes
  /// @brief The greatest integer such that <code>2</code> raised to that power is a normalized @a dx_f64 value.
  #define DX_F64_GREATEST_2_EXP (DBL_MAX_EXP)
  /// @ingroup Core_BuiltinTypes
  /// @brief The least integer such that <code>2</code> raised to that power is a normalized @a dx_f64 value.
  #define DX_F64_LEAST_2_EXP (DBL_MIN_EXP)
#else
  #error("environment not (yet) supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_BUILTIN_TYPES_H_INCLUDED
