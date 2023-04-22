/// @file dx/core/inline_pointer_array.h
/// @brief A deque for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_INLINE_POINTER_ARRAY_H_INCLUDED)
#define DX_CORE_INLINE_POINTER_ARRAY_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup Core_Aggregates
/// @brief The elements of a dx_inline_pointer_array are pointers.
typedef void* dx_inline_pointer_array_element;

/// @ingroup Core_Aggregates
/// @brief Type of a callback invoked if an element was added.
/// @param element A pointer to the element.
typedef void (dx_inline_pointer_array_added_callback)(dx_inline_pointer_array_element* element);

/// @ingroup Core_Aggregates
/// @brief Type of a callback invoked if an element was removed.
/// @param element A pointer to the element.
typedef void (dx_inline_pointer_array_removed_callback)(dx_inline_pointer_array_element* element);

/// @ingroup Core_Aggregates
/// @brief A dynamic array of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct dx_inline_pointer_array dx_inline_pointer_array;

struct dx_inline_pointer_array {
  void* pimpl;
}; // struct dx_inline_pointer_array

/// @ingroup Core_Aggregates
/// @brief Configuration of a pointer array.
typedef struct DX_INLINE_POINTER_ARRAY_CONFIGURATION {

  /// @brief A pointer to the @a _dx_added_callback function or a null pointer.
  dx_inline_pointer_array_added_callback* added_callback;

  /// @brief A pointer to the @a _dx_removed_callback function or  a null pointer.
  dx_inline_pointer_array_removed_callback* removed_callback;

} DX_INLINE_POINTER_ARRAY_CONFIGURATION;

/// @brief Initialize this dx_inline_pointer_array object.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a initial_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_array_initialize(dx_inline_pointer_array* SELF, dx_size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration);

/// @brief Uninitialize this dx_inline_pointer_array object.
/// @param SELF A pointer to this dx_inline_pointer_array object.
void dx_inline_pointer_array_uninitialize(dx_inline_pointer_array* SELF);

/// @brief Increase the capacity.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param additional_capacity The amount to increase the capacity by.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
dx_result dx_inline_pointer_array_increase_capacity(dx_inline_pointer_array* SELF, dx_size additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this dx_pointer_deque object.
/// @param required_free_capacity The required free capacity.
/// @default-runtime-calling-convention
/// @error #DX_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ALLOCATION_FAILED @a required_free_capacity is too big
/// @error #DX_ALLOCATION_FAILED an allocation failed
/// @success The free capacity is greater than or equal to the specified required free capacity.
dx_result dx_inline_pointer_array_ensure_free_capacity(dx_inline_pointer_array* SELF, dx_size required_free_capacity);

/// @brief Append an element.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param pointer The element.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_array_append(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer);

/// @brief Prepend an element.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param pointer The element.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_array_prepend(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer);

/// @brief Insert an element.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param pointer The element.
/// @param index The index.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of this dx_inline_pointer_array object
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_array_insert(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer, dx_size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>void*</code> variable.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than or equal to the size of this dx_inline_pointer_array object
dx_result dx_inline_pointer_array_get_at(dx_inline_pointer_array_element* RETRUN, dx_inline_pointer_array const* SELF, dx_size index);

/// @brief Get the size, in elements.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @success <code>*RETURN</code> was assigned the size.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
dx_result dx_inline_pointer_array_get_size(dx_size* RETURN, dx_inline_pointer_array const* SELF);

/// @brief Get the capacity, in elements.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_array_get_capacity(dx_size* RETURN, dx_inline_pointer_array const* SELF);

/// @brief Get the free capacity, in elements.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_array_get_free_capacity(dx_size* RETURN, dx_inline_pointer_array const* SELF);

/// @brief Remove all elements.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_array_clear(dx_inline_pointer_array* SELF);

/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this dx_inline_pointer_array object.
/// @param n The number of objects to remove.
/// @default-runtime-calling-convention
/// @remarks If this function fails, no elements were removed.
/// @error #DX_ERROR_INVALID_ARGUMENT @a n is greater than the size of the dx_inline_pointer_array object.
dx_result dx_inline_pointer_array_pop_back_n(dx_inline_pointer_array* SELF, dx_size n);

#endif // DX_INLINE_CORE_POINTER_ARRAY_H_INCLUDED
