/// @file dx/core/inline_pointer_deque.h
/// @brief A deque for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_INLINE_POINTER_DEQUE_H_INCLUDED)
#define DX_INLINE_POINTER_DEQUE_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup Core_Aggregates
/// @brief The elements of a dx_inline_pointer_deque are pointers.
typedef void* dx_inline_pointer_deque_element;

/// @brief Type of a callback invoked if an element was added.
/// @param element A pointer to the element.
typedef void (dx_inline_pointer_deque_added_callback)(dx_inline_pointer_deque_element* element);

/// @brief Type of a callback invoked if an element was removed.
/// @param element A pointer to the element.
typedef void (dx_inline_pointer_deque_removed_callback)(dx_inline_pointer_deque_element* element);

/// @ingroup Core_Aggregates
/// @brief Configuration of a pointer deque.
typedef struct DX_INLINE_POINTER_DEQUE_CONFIGURATION {

  /// @brief A pointer to the @a _dx_added_callback function or a null pointer.
  dx_inline_pointer_deque_added_callback* added_callback;

  /// @brief A pointer to the @a _dx_removed_callback function or  a null pointer.
  dx_inline_pointer_deque_removed_callback* removed_callback;

} DX_INLINE_POINTER_DEQUE_CONFIGURATION;

/// @brief A dynamic deque of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct dx_inline_pointer_deque dx_inline_pointer_deque;

struct dx_inline_pointer_deque {
  void* pimpl;
};

/// @brief Initialize this dx_inline_pointer_deque object.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @method-call
/// @error #DX_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ALLOCATION_FAILED @a initial_capacity is too big
/// @error #DX_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_deque_initialize(dx_inline_pointer_deque* SELF, dx_size initial_capacity, DX_INLINE_POINTER_DEQUE_CONFIGURATION const* configuration);

/// @brief Uninitialize this dx_inline_pointer_deque object.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
void dx_inline_pointer_deque_uninitialize(dx_inline_pointer_deque* SELF);

/// @brief Increase the capacity.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param required_additional_capacity The amount to increase the capacity by.
/// @method-call
/// @error #DX_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
dx_result dx_inline_pointer_deque_increase_capacity(dx_inline_pointer_deque* SELF, dx_size required_additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param required_free_capacity The required free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a required_free_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The free capacity is greater than or equal to the specified required free capacity.
dx_result dx_inline_pointer_deque_ensure_free_capacity(dx_inline_pointer_deque* SELF, dx_size required_free_capacity);

/// @brief Get the size, in elements.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_deque_get_size(dx_size* RETURN, dx_inline_pointer_deque const* SELF);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_deque_get_capacity(dx_size* RETURN, dx_inline_pointer_deque const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_deque_get_free_capacity(dx_size* RETURN, dx_inline_pointer_deque const* SELF);

/// @brief Remove all elements.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_pointer_deque_clear(dx_inline_pointer_deque* SELF);

/// @brief Append an element.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param pointer The element.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_deque_push_back(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer);

/// @brief Prepend an element.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param pointer The element.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_deque_push_front(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer);

/// @brief Insert an element.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param pointer The element.
/// @param index The index.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of this dx_inline_pointer_deque object
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_pointer_deque_insert(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer, dx_size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than or equal to the size of this dx_inline_pointer_deque object
dx_result dx_inline_pointer_deque_get_at(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_size index);

/// @brief Remove the pointer at the specified index.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param SELF A pointer to the dx_inline_pointer_deque object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than or equal to the size of this dx_inline_pointer_deque object
dx_result dx_inline_pointer_deque_remove(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_bool steal, dx_size index);

/// @brief Pop the first pointer of the deque.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param SELF A pointer to this deque.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the first pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT the deque is empty
dx_result dx_inline_pointer_deque_pop_front(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_bool steal);

/// @brief Pop the last pointer of the deque.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param SELF A pointer to this deque.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the last pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT the deque is empty
dx_result dx_inline_pointer_deque_pop_back(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_bool steal);

#endif // DX_INLINE_POINTER_DEQUE_H_INCLUDED
