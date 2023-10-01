/// @file dx/core/visuals/depth_test_function.h
/// @brief Enumeration of depth test functions.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_VISUALS_DEPTH_TEST_FUNCTION_H_INCLUDED)
#define DX_CORE_VISUALS_DEPTH_TEST_FUNCTION_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Visuals
/// @brief Enumeration of depth test functions.
/// @details A depth test function identifies a technique to that compares depth value of an incoming fragment to its corresponding depth value in the depth buffer.
DX_DECLARE_ENUMERATION_TYPE("dx.depth_test_function",
                            dx_depth_test_function);

enum dx_depth_test_function {

  /// @brief The incoming fragment always passes.
  dx_depth_test_function_always,

  /// @brief The incoming fragment never passes.
  dx_depth_test_function_never,

  /// @brief The incoming fragment passes if its depth value is equal to its corresponding depth value in the depth buffer.
  dx_depth_test_function_equal,

  /// @brief The incoming fragement passes if its depth value is not equal to its corresponding depth value in the depth buffer.
  dx_depth_test_function_not_equal,

  /// @brief The incoming fragment passes if its depth value is less than its corresponding depth value in the depth buffer.
  dx_depth_test_function_less_than,

  /// @brief The incoming fragment passes if its depth value is less than or equal to its corresponding depth value in the depth buffer.
  dx_depth_test_function_less_than_or_equal_to,

  /// @brief The incoming fragment passes if its depth value is greater than its corresponding depth value in the depth buffer.
  dx_depth_test_function_greater_than,

  /// @brief The incoming fragment passes if its depth value is greater than or equal to its corresponding depth value in the depth buffer.
  dx_depth_test_function_greater_than_or_equal_to,

};

#endif // DX_CORE_VISUALS_DEPTH_TEST_FUNCTION_H_INCLUDED
