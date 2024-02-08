/// @file dx/core/annotations.h
/// @brief Annotations for types and variables.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_ANNOTATIONS_H_INCLUDED)
#define CORE_ANNOTATIONS_H_INCLUDED

#include "dx/core/configuration.h"

/* http://localhost/core#core-threadlocal */
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_Clang)
  #define Core_ThreadLocal() __thread
#elif (Core_Compiler_C == Core_Compiler_C_Msvc)
  #define Core_ThreadLocal() __declspec(thread)
#else
  #error("C compiler not (yet) supported")
#endif

/* http://localhost/core#core-noreturn */
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_Clang)
  #define Core_NoReturn() __attribute__ ((noreturn))
#elif (Core_Compiler_C == Core_Compiler_C_Msvc)
  #define Core_NoReturn() __declspec(noreturn)
#else
  #error("C compiler not (yet) supported")
#endif

/* http://localhost/core#core-likely */
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_CLANG)
  #define DX_LIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 1))
#else
  #define Core_Likely(expression) (expression)
#endif

/* http://localhost/core#core-unlikely */
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_Clang)
  #define DX_UNLIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 0))
#else
  #define Core_Unlikely(expression) (expression)
#endif

#endif // CORE_ANNOTATIONS_H_INCLUDED
