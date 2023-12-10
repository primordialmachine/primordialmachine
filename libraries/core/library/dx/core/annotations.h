/// @file dx/core/annotations.h
/// @brief Annotations for types and variables.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_ANNOTATIONS_H_INCLUDED)
#define DX_CORE_ANNOTATIONS_H_INCLUDED

#include "dx/core/configuration.h"

/// @ingroup Core_Annotations
/// @brief Variable annotation indicating a thread local storage.
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_Clang) && !defined(DOXYGEN)
  #define Core_ThreadLocal() __thread  /**< @hideinitializer */
#elif (Core_Compiler_C == Core_Compiler_C_Msvc) && !defined(DOXYGEN)
  #define Core_ThreadLocal() __declspec(thread) /**< @hideinitializer */
#else
  #error("C compiler not (yet) supported")
#endif

/// @ingroup Core_Annotations
/// @brief Function annotation indicating that a function never returns.
/// @details
/// This function annotation indicates that a function never returns.
/// If it returns, it only returns by raising an exception or by calling `longjmp`.
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_Clang) && !defined(DOXYGEN)
  #define DX_NO_RETURN() __attribute__ ((noreturn)) /**< @hideinitializer */
#elif (Core_Compiler_C == Core_Compiler_C_Msvc) && !defined(DOXYGEN)
  #define DX_NO_RETURN() __declspec(noreturn) /**< @hideinitializer */
#else
  #error("C compiler not (yet) supported")
#endif

/// @ingroup Core_Annotations
/// @brief Expression annotation providing branch prediction information, indicating an expression is likely to evaluate to logically true.
/// @details
/// This expression annotation indicates that the expression `expression` is likely to evaluate to logically false.
///
/// For example, given contemporary compilers and environments, it is likely that `malloc` for a small size of memory
/// is very likely to succeed. Hence, in the following code snipped, we tell tell the compiler that the test for success
/// of such a call to `malloc` is likely to to evaluate to logically true and the code in branch `(A)` is likely to
/// be executed whilst consequently the code in branch `(X)` is not likely to be execute.
/// @code
/// char *p = malloc(sizeof(char) * 1024);
/// if (DX_LIKELY(p)) {
///   /* (A) Foaming with joy, continue execution. */
/// } else {
///   /* (X) In the heart with pain, abort execution. */
/// }
/// @endcode
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_CLANG) && !defined(DOXYGEN)
  #define DX_LIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 1)) /**< @hideinitializer */
#else
  #define DX_LIKELY(expression) (expression) /**< @hideinitializer */
#endif

/// @ingroup Core_Annotations
/// @brief Expression annotation providing branch prediction information, indicating an expression is likely to evaluate to logically false.
/// @details
/// This expression annotation indicates that the expression `expression` is likely to evaluate to logically false.
///
/// For example, given contemporary compilers and environments, it is likely that `malloc` for a small size of memory
/// is very likely to succeed. Hence, in the following code snipped, we tell tell the compiler that the test for failure
/// of such a call to `malloc` is likely to to evaluate to logically false and the code in branch `(A)` is likely to
/// be executed whilst consequently the code in branch `(X)` is not likely to be execute.
/// @code
/// char *p = malloc(sizeof(char) * 1024);
/// if (DX_UNLIKELY(!p)) {
///   /* (X) In the heart with pain, abort execution. */
/// } else {
///   /* (A) Foaming with joy, continue execution. */
/// }
/// @endcode
#if (Core_Compiler_C == Core_Compiler_C_Gcc || Core_Compiler_C == Core_Compiler_C_Clang) && !defined(DOXYGEN)
  #define DX_UNLIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 0)) /**< @hideinitializer */
#else
  #define DX_UNLIKELY(expression) (expression) /**< @hideinitializer */
#endif

#endif // DX_CORE_ANNOTATIONS_H_INCLUDED
