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
/// The special value DX_NO_ERROR evaluates to @a 0 and indicates the absence of an error.
typedef int dx_error;

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating the absence of an error.
/// The dx_error value indicating the absence of an error is the zero value.
#define DX_NO_ERROR (0)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed allocation.
/// This value is guaranteed to be a non-zero value.
#define DX_ERROR_ALLOCATION_FAILED (1)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of an invalid argument.
/// This value is guaranteed to be a non-zero value.
#define DX_ERROR_INVALID_ARGUMENT (2)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of an invalid operation.
/// This value is guaranteed to be a non-zero value.
#define DX_ERROR_INVALID_OPERATION (3)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed environment.
/// This value is guaranteed to be a non-zero value.
#define DX_ERROR_ENVIRONMENT_FAILED (4)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed conversion.
/// This value is guaranteed to be a non-zero value.
#define DX_ERROR_CONVERSION_FAILED (5)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed decoding.
/// This value is guaranteed to be a non-zero value.
#define DX_ERROR_DECODING_FAILED (6)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a lexical error.
#define DX_ERROR_LEXICAL_ERROR (7)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a syntactical error.
#define DX_ERROR_SYNTACTICAL_ERROR (8)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a semantical error.
#define DX_ERROR_SEMANTICAL_ERROR (9)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something was not found.
#define DX_ERROR_NOT_FOUND (10)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something exists.
#define DX_ERROR_EXISTS (11)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of an overflow.
#define DX_ERROR_OVERFLOW (12)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because an operation was not (yet) implemented.
#define DX_ERROR_NOT_IMPLEMENTED (13)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something is empty.
#define DX_ERROR_IS_EMPTY (14)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something is not (yet) intialized.
#define DX_ERROR_NOT_INITIALIZED (15)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something is (already) intialized.
#define DX_ERROR_INITIALIZED (16)

/// @ingroup core
/// @brief Get the value of the error variable.
/// @return The value.
dx_error dx_get_error();

/// @ingroup core
/// @brief Set the value of the error variable.
/// @param error The value.
void dx_set_error(dx_error error);

/// @ingroup core
/// @brief The type of a result value. There are only two values of this type, #DX_SUCCESS and #DX_FAILURE.
typedef uint8_t dx_result;

/// @ingroup core
/// @brief The dx_result value indicating success.
#define DX_SUCCESS (0)

/// @ingroup core
/// @brief The dx_result value indicating a failure.
#define DX_FAILURE (1)

#endif // DX_CORE_ERROR_H_INCLUDED
