/// @file dx/core/error.h
/// @brief Functionality related to error propagation and handling.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_ERROR_H_INCLUDED)
#define DX_CORE_ERROR_H_INCLUDED

#include "dx/core/configuration.h"
#include <inttypes.h>

/// @ingroup Core
/// @brief The integer type of values indicating error status.
typedef int Core_Error;

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating the absence of an error.
/// This is the only Core_Error_* symbolic constant evaluating to to the zero value.
#define Core_Error_NoError (0)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a failed allocation.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_AllocationFailed (1)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of an invalid argument.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_ArgumentInvalid (2)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of an invalid operation.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_OperationInvalid (3)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a failed environment.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_EnvironmentFailed (4)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a failed conversion.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_ConversionFailed (5)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a failed decoding.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_DecodingFailed (6)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a lexical error.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_LexicalError (7)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a syntactical error.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_SyntacticalError (8)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of a semantical error.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_SemanticalError (9)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because something was not found.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_NotFound (10)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because something exists.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_Exists (11)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because of an overflow.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_Overflow (12)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because an operation was not (yet) implemented.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_NotImplemented (13)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because something is empty.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_Empty (14)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because something is not (yet) intialized.
/// The Core_Error value this symbolic constant is evaluating to is guaranteed to be non-zero.
#define Core_Error_NotInitialized (15)

/// @ingroup Core
/// @brief Symbolic constant for a Core_Error value indicating a failure because something is (already) intialized.
/// This value is guaranteed to be a non-zero value.
#define Core_Error_Initialized (16)

/// @ingroup Core
/// @brief Get the value of the error variable.
/// @return The value.
Core_Error Core_getError();

/// @ingroup Core
/// @brief Set the value of the error variable.
/// @param error The value.
void Core_setError(Core_Error error);

/// @ingroup Core
/// @brief The type of a result value. There are only two values of this type, #Core_Success and #Core_Failure.
typedef uint8_t Core_Result;

/// @ingroup Core
/// @brief The Core_Result value indicating success.
#define Core_Success (0)

/// @ingroup Core
/// @brief The Core_Result value indicating a failure.
#define Core_Failure (1)

#endif // DX_CORE_ERROR_H_INCLUDED
