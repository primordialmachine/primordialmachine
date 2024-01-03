/// @file dx/core/error.h
/// @brief Functionality related to error propagation and handling.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_ERROR_H_INCLUDED)
#define DX_CORE_ERROR_H_INCLUDED

#include "dx/core/configuration.h"
#include <inttypes.h>

// http://localhost/core#core-error
typedef int Core_Error;

enum _Core_Error {
  // http://localhost/core#core-error-noerror
  Core_Error_NoError = (0),

  // http://localhost/core#core-error-allocationfailed
  Core_Error_AllocationFailed,

  // http://localhost/core#core-error-argumentinvalid
  Core_Error_ArgumentInvalid,

  // http://localhost/core#core-error-operationinvalid
  Core_Error_OperationInvalid,

  // http://localhost/core#core-error-environmentfailed
  Core_Error_EnvironmentFailed,

  // http://localhost/core#core-error-lexicalanalysisfailed
  Core_Error_LexicalAnalysisFailed,

  // http://localhost/core#core-error-syntacticalanalysisfailed
  Core_Error_SyntacticalAnalysisFailed,

  // http://localhost/core#core-error-semanticalanalysisfailed
  Core_Error_SemanticalAnalysisFailed,

  // http://localhost/core#core-error-exists
  Core_Error_Exists,

  // http://localhost/core#core-error-notexists
  Core_Error_NotExists,

  // http://localhost/core#core-error-empty
  Core_Error_Empty,

  // http://localhost/core#core-error-notempty
  Core_Error_NotEmpty,

  // http://localhost/core#core-error-notinitialized
  Core_Error_NotInitialized,

  // http://localhost/core#core-error-notinitialized
  Core_Error_Initialized,

  // http://localhost/core#core-error-numericoverflow
  Core_Error_NumericOverflow,

  // http://localhost/core#core-error-notimplemented
  Core_Error_NotImplemented,

  // http://localhost/core#core-error-conversionfailed
  Core_Error_ConversionFailed,

  // http://localhost/core#core-error-decodingfailed
  Core_Error_DecodingFailed,

  // http://localhost/core#core-error-encodingfailed
  Core_Error_EncodingFailed,

  // http://localhost/core#core-error-notfound
  Core_Error_NotFound,

  // http://localhost/core#core-error-found
  Core_Error_Found,

};

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
