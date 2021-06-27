/// @file Runtime/PrimitiveTypes.h
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
/// @brief C-level representations of <code>Boolean</code>, <code>ForeignProcedure</code>, <code>Integer</code>, <code>Real</code>, and <code>Void</code>.
#if !defined(MACHINE_RUNTIME_PRIMITIVETYPES_H_INCLUDED)
#define MACHINE_RUNTIME_PRIMITIVETYPES_H_INCLUDED

#if !defined(MACHINE_RUNTIME_PRIVATE)
#error("Do not include this file directly, include `_Runtime.h` instead.")
#endif

#include <stdbool.h>
#include <stdint.h>

#if defined(INLINE)
#error('INLINE already defined')
#endif
#define INLINE static inline

#if defined(NORETURN)
#error('NORETURN already defined')
#endif
#define NORETURN __declspec(noreturn)

#if defined(DEPRECATED)
#error('DEPRECATED already defined')
#endif
#define DEPRECATED
//#define DEPRECATED __declspec(deprecated)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The C-level representation of the <code>void</code> value.
#define Machine_Void_Void (0)

/// @brief The C-level representation of the <code>Void</code> type.
typedef int Machine_Void;

INLINE bool Machine_Void_getHashValue(Machine_Void x) {
  // 1249  is merely one positive and sufficiently big prime numbers.
  return 1249;
}

INLINE bool Machine_Void_isEqualTo(Machine_Void x, Machine_Void y) {
  return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The C-level representation of the <code>true</code> value.
#define Machine_Boolean_True (true)

/// @brief The C-level representation of the <code>false</code> value.
#define Machine_Boolean_False (false)

/// @brief The C-level representation of the <code>Boolean</code> type.
typedef bool Machine_Boolean;

INLINE size_t Machine_Boolean_getHashValue(Machine_Boolean x) {
  // 1231 and 1237 are merely two positive and sufficiently big prime numbers.
  return x ? 1231 : 1237;
}

INLINE bool Machine_Boolean_isEqualTo(Machine_Boolean x, Machine_Boolean y) {
  return x == y;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The C-level representation of the <code>Integer</code> type.
typedef int32_t Machine_Integer;

/// @brief The greatest <code>Integer</code> value.
#define Machine_Integer_Greatest INT32_MAX

/// @brief The least <code>Integer</code> value.
#define Machine_Integer_LEast INT32_MIN

INLINE size_t Machine_Integer_getHashValue(Machine_Integer x) {
  return (size_t)x;
}

INLINE bool Machine_Integer_isEqualTo(Machine_Integer x, Machine_Integer y) {
  return x == y;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Machine_Value Machine_Value;

/// @brief The C-level representation of the <code>ForeignProcedure</code> type.
typedef Machine_Value(Machine_ForeignProcedure)(size_t numberOfArguments, const Machine_Value* arguments);

INLINE size_t Machine_ForeignProcedure_getHashValue(Machine_ForeignProcedure* x) {
  return (size_t)(uintptr_t)x;
}

INLINE bool Machine_ForeignProcedure_isEqualTo(Machine_ForeignProcedure* x, Machine_ForeignProcedure* y) {
  return x == y;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef float Machine_Real;

size_t Machine_Real_getHashValue(Machine_Real x);

bool Machine_Real_isEqualTo(Machine_Real x, Machine_Real y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // MACHINE_RUNTIME_PRIMITIVETYPES_H_INCLUDED
