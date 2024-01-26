/// @file dx/core/inline_pointer_deque.h
/// @brief A deque for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

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
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @method{dx_inline_pointer_deque}
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_pointer_deque_initialize(dx_inline_pointer_deque* SELF, Core_Size initial_capacity, DX_INLINE_POINTER_DEQUE_CONFIGURATION const* configuration);

/// @brief Uninitialize this dx_inline_pointer_deque object.
/// @method{dx_inline_pointer_deque}
void dx_inline_pointer_deque_uninitialize(dx_inline_pointer_deque* SELF);

/// @brief Increase the capacity.
/// @param required_additional_capacity The amount to increase the capacity by.
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_increaseCapacity(dx_inline_pointer_deque* SELF, Core_Size required_additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param required_free_capacity The required free capacity.
/// @error #Core_Error_AllocationFailed @a required_free_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The free capacity is greater than or equal to the specified required free capacity.
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_ensureFreeCapacity(dx_inline_pointer_deque* SELF, Core_Size required_free_capacity);

/// @brief Get the size, in elements.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the size.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_getSize(Core_Size* RETURN, dx_inline_pointer_deque const* SELF);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_getCapacity(Core_Size* RETURN, dx_inline_pointer_deque const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_getFreeCapacity(Core_Size* RETURN, dx_inline_pointer_deque const* SELF);

/// @brief Remove all elements.
/// @param SELF A pointer to this dx_inline_pointer_deque object.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
Core_Result dx_inline_pointer_deque_clear(dx_inline_pointer_deque* SELF);

/// @brief Append an element.
/// @param pointer The element.
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_push_back(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer);

/// @brief Prepend an element.
/// @param pointer The element.
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_push_front(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer);

/// @brief Insert an element.
/// @param pointer The element.
/// @param index The index.
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this dx_inline_pointer_deque object
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_insert(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer, Core_Size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of this dx_inline_pointer_deque object
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_get_at(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, Core_Size index);

/// @brief Remove the pointer at the specified index.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of this dx_inline_pointer_deque object
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_remove(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, Core_Boolean steal, Core_Size index);

/// @brief Pop the first pointer of the deque.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the first pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid the deque is empty
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_pop_front(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, Core_Boolean steal);

/// @brief Pop the last pointer of the deque.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the last pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid the deque is empty
/// @method{dx_inline_pointer_deque}
Core_Result dx_inline_pointer_deque_pop_back(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, Core_Boolean steal);

#endif // DX_INLINE_POINTER_DEQUE_H_INCLUDED
