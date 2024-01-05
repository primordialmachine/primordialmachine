// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_FUNDAMENTALTYPES_H_INCLUDED)
#define CORE_FUNDAMENTALTYPES_H_INCLUDED

#include "dx/core/configuration.h"
#include "Core/Error.h"
#include "Core/Result.h"

// size_t, SIZE_MAX
#include <stddef.h>

// bool, true, false
#include <stdbool.h>

// uintx_t, UINTx_MAX
// intx_t, INTx_MIN, INTx_MAX
#include <inttypes.h>

// FLT_RADIX
#include <float.h>

// Forward declaration.
typedef struct Core_Type Core_Type;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FundamentalTypes
/// @brief An alias for the <code>void</code> C type.
typedef void Core_Void;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-boolean */
typedef bool Core_Boolean;

/* http://localhost/core/ring-1/#core-true */
#define Core_True (true)

/* http://localhost/core/ring-1/#core-false */
#define Core_False (false)

Core_Result Core_Boolean_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-size */
typedef size_t Core_Size;

/* http://localhost/core/ring-1/#core-size-least */
#define Core_Size_Least (0)

/* http://localhost/core/ring-1/#core-size-greatest */
#define Core_Size_Greatest (SIZE_MAX)

Core_Result Core_Size_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-natural8 */
typedef uint8_t Core_Natural8;

/* http://localhost/core/ring-1/#core-natural8-least */
#define Core_Natural8_Least (UINT8_C(0))

/* http://localhost/core/ring-1/#core-natural8-greatest */
#define Core_Natural8_Greatest (UINT8_MAX)

Core_Result Core_Natural8_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-natural16 */
typedef uint16_t Core_Natural16;

/* http://localhost/core/ring-1/#core-natural16-least */
#define Core_Natural16_Least (UINT16_C(0))

/* http://localhost/core/ring-1/#core-natural16-greatest */
#define Core_Natural16_Greatest (UINT16_MAX)

Core_Result Core_Natural16_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-natural32 */
typedef uint32_t Core_Natural32;

/* http://localhost/core/ring-1/#core-natural32-least */
#define Core_Natural32_Least (UINT32_C(0))

/* http://localhost/core/ring-1/#core-natural32-greatest */
#define Core_Natural32_Greatest (UINT32_MAX)

Core_Result Core_Natural32_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-natural64 */
typedef uint64_t Core_Natural64;

/* http://localhost/core/ring-1/#core-natural64-least */
#define Core_Natural64_Least (UINT64_C(0))

/* http://localhost/core/ring-1/#core-natural64-greatest */
#define Core_Natural64_Greatest (UINT64_MAX)

Core_Result Core_Natural64_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-integer8 */
typedef int8_t Core_Integer8;

/* http://localhost/core/ring-1/#core-integer8-least */
#define Core_Integer8_Least (INT8_MIN)

/* http://localhost/core/ring-1/#core-integer8-greatest */
#define Core_Integer8_Greatest (INT8_MAX)

Core_Result Core_Integer8_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-integer16 */
typedef int16_t Core_Integer16;

/* http://localhost/core/ring-1/#core-integer16-least */
#define Core_Integer16_Least (INT16_MIN)

/* http://localhost/core/ring-1/#core-integer16-greatest */
#define Core_Integer16_Greatest (INT16_MAX)

Core_Result Core_Integer16_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-integer32 */
typedef int32_t Core_Integer32;

/* http://localhost/core/ring-1/#core-integer32-least */
#define Core_Integer32_Least (INT32_MIN)

/* http://localhost/core/ring-1/#core-integer32-greatest */
#define Core_Integer32_Greatest (INT32_MAX)

Core_Result Core_Integer32_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-integer64 */
typedef int64_t Core_Integer64;

/* http://localhost/core/ring-1/#core-integer64-least */
#define Core_Integer64_Least (INT64_MIN)

/* http://localhost/core/ring-1/#core-integer64-greatest */
#define Core_Integer64_Greatest (INT64_MAX)

Core_Result Core_Integer64_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-real32 */
typedef float Core_Real32;

/* http://localhost/core/ring-1/#core-real32-least */
#define Core_Real32_Least (-FLT_MAX)

/* http://localhost/core/ring-1/#core-real32-greatest */
#define Core_Real32_Greatest (+FLT_MAX)

/* http://localhost/core/ring-1/#core-real32-positiveinfinity */
#define Core_Real32_PositiveInfinity (+INFINITY)

/* http://localhost/core/ring-1/#core-real32-negativeinfinity */
#define Core_Real32_NegativeInfinity (-INFINITY)

/* http://localhost/core/ring-1/#core-real32-nonumber */
#define Core_Real32_NoNumber (NAN)

/* http://localhost/core/ring-1/#core-real32-GreatestBaseTenExponent */
#define Core_Real32_GreatestBaseTenExponent (FLT_MAX_10_EXP)

/* http://localhost/core/ring-1/#core-real32-LeastBaseTenExponent */
#define Core_Real32_LeastBaseTenExponent (FLT_MIN_10_EXP)

#if !defined(FLT_RADIX) || 2 != FLT_RADIX
  #error(environment not supported: FLT_RADIX is not defined as 2)
#endif

/* http://localhost/core/ring-1/#core-real32-leastbasetwoexponent */
#define Core_Real32_LeastBaseTwoExponent (FLT_MIN_EXP)

/* http://localhost/core/ring-1/#core-real32-greatestbasetwoexponent */
#define Core_Real32_GreatestBaseTwoExponent (FLT_MAX_EXP)

/* http://localhost/core/ring-1/#core-isinfinity */
Core_Result Core_isInfinityR32(Core_Boolean* RETURN, Core_Real32 x);

/* http://localhost/core/ring-1/#core-isnonumber */
Core_Result Core_isNoNumberR32(Core_Boolean* RETURN, Core_Real32 x);

Core_Result Core_Real32_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core/ring-1/#core-real64 */
typedef double Core_Real64;

/* http://localhost/core/ring-1/#core-real64-least */
#define Core_Real64_Least (-DBL_MAX)

/* http://localhost/core/ring-1/#core-real64-greatest */
#define Core_Real64_Greatest (+DBL_MAX)

/* http://localhost/core/ring-1/#core-real64-positiveinfinity */
#define Core_Real64_PositiveInfinity (+INFINITY)

/* http://localhost/core/ring-1/#core-real64-negativeinfinity */
#define Core_Real64_NegativeInfinity (-INFINITY)

/* http://localhost/core/ring-1/#core-real64-nonumber */
#define Core_Real64_NoNumber (NAN)

/* http://localhost/core/ring-1/#core-real64-greatestbasetenexponent */
#define Core_Real64_GreatestBaseTenExponent (DBL_MAX_10_EXP)

/* http://localhost/core/-ring1/#core-real64-leastbasetenexponent */
#define Core_Real64_LeastBaseTenExponent (DBL_MIN_10_EXP)

#if !defined(FLT_RADIX) || 2 != FLT_RADIX
  #error(FLT_RADIX is not defined as 2)
#endif

/* http://localhost/core/ring-1/#core-real64-leastbasetwoexponent */
#define Core_Real64_LeastBaseTwoExponent (DBL_MIN_EXP)

/* http://localhost/core/ring-1/#core-real64-greatestbasetwoexponent */
#define Core_Real64_GreatestBaseTwoExponent (DBL_MAX_EXP)

/* http://localhost/core/ring-1/#core-isinfinity */
Core_Result Core_isInfinityR64(Core_Boolean* RETURN, Core_Real64 x);

/* http://localhost/core/ring-1/#core-isnonumber */
Core_Result Core_isNoNumberR64(Core_Boolean* RETURN, Core_Real64 x);

Core_Result Core_Real64_getType(Core_Type** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_FUNDAMENTALTYPES_H_INCLUDED
