// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_RESULT_H_INCLUDED)
#define CORE_RESULT_H_INCLUDED

#include "dx/core/configuration.h"
#include <inttypes.h>

// http://localhost/core#core-result
typedef uint8_t Core_Result;

// http://localhost/core#core-success
#define Core_Success (0)

// http://localhost/core#core-failure
#define Core_Failure (1)

#endif // CORE_RESULT_H_INCLUDED
