/// @file dx/core/inline_pointer_deque.h
/// @brief A deque for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(DX_INLINE_POINTER_DEQUE_H_INCLUDED)
#define DX_INLINE_POINTER_DEQUE_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup Core_Aggregates
/// @brief The elements of a Core_InlineArrayDequeP are pointers.
typedef void* Core_InlineArrayDequeP_element;

/// @brief Type of a callback invoked if an element was added.
/// @param element A pointer to the element.
typedef void (Core_InlineArrayDequeP_added_callback)(Core_InlineArrayDequeP_element* element);

/// @brief Type of a callback invoked if an element was removed.
/// @param element A pointer to the element.
typedef void (Core_InlineArrayDequeP_removed_callback)(Core_InlineArrayDequeP_element* element);

/// @ingroup Core_Aggregates
/// @brief Configuration of a pointer deque.
typedef struct Core_InlineArrayDequeP_Configuration {

  /// @brief A pointer to the @a _dx_added_callback function or a null pointer.
  Core_InlineArrayDequeP_added_callback* added_callback;

  /// @brief A pointer to the @a _dx_removed_callback function or  a null pointer.
  Core_InlineArrayDequeP_removed_callback* removed_callback;

} Core_InlineArrayDequeP_Configuration;

/// @brief A dynamic deque of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct Core_InlineArrayDequeP Core_InlineArrayDequeP;

struct Core_InlineArrayDequeP {
  void* pimpl;
};

/// @brief Initialize this Core_InlineArrayDequeP object.
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @method{Core_InlineArrayDequeP}
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result Core_InlineArrayDequeP_initialize(Core_InlineArrayDequeP* SELF, Core_Size initial_capacity, Core_InlineArrayDequeP_Configuration const* configuration);

/* http://localhost/core#core-inlinearraydeque-uninitialize */
void Core_InlineArrayDequeP_uninitialize(Core_InlineArrayDequeP* SELF);

/* http://localhost/core#core-inlinearraydeque-clear */
Core_Result Core_InlineArrayDequeP_clear(Core_InlineArrayDequeP* SELF);

/* http://localhost/core#core-inlinearraydeque-ensurefreecapacity */
Core_Result Core_InlineArrayDequeP_ensureFreeCapacity(Core_InlineArrayDequeP* SELF, Core_Size requiredFreeCapacity);

/* http://localhost/core#core-inlinearraydeque-getcapacity */
Core_Result Core_InlineArrayDequeP_getCapacity(Core_Size* RETURN, Core_InlineArrayDequeP const* SELF);

/* http://localhost/core#core-inlinearraydeque-getfreecapacity */
Core_Result Core_InlineArrayDequeP_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayDequeP const* SELF);

/* http://localhost/core#core-inlinearraydeque-getsize */
Core_Result Core_InlineArrayDequeP_getSize(Core_Size* RETURN, Core_InlineArrayDequeP const* SELF);

/* http://localhost/core#core-inlinearraydeque-increaseincreasecapacity */
Core_Result Core_InlineArrayDequeP_increaseCapacity(Core_InlineArrayDequeP* SELF, Core_Size requiredAdditionalCapacity);

/// @brief Append an element.
/// @param pointer The element.
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_push_back(Core_InlineArrayDequeP* SELF, Core_InlineArrayDequeP_element pointer);

/// @brief Prepend an element.
/// @param pointer The element.
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_push_front(Core_InlineArrayDequeP* SELF, Core_InlineArrayDequeP_element pointer);

/// @brief Insert an element.
/// @param pointer The element.
/// @param index The index.
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this Core_InlineArrayDequeP object
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_insert(Core_InlineArrayDequeP* SELF, Core_InlineArrayDequeP_element pointer, Core_Size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>Core_InlineArrayDequeP_element</code> variable.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of this Core_InlineArrayDequeP object
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_get_at(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Size index);

/// @brief Remove the pointer at the specified index.
/// @param RETURN A pointer to a <code>Core_InlineArrayDequeP_element</code> variable.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of this Core_InlineArrayDequeP object
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_remove(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Boolean steal, Core_Size index);

/// @brief Pop the first pointer of the deque.
/// @param RETURN A pointer to a <code>Core_InlineArrayDequeP_element</code> variable.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the first pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid the deque is empty
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_pop_front(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Boolean steal);

/// @brief Pop the last pointer of the deque.
/// @param RETURN A pointer to a <code>Core_InlineArrayDequeP_element</code> variable.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the last pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid the deque is empty
/// @method{Core_InlineArrayDequeP}
Core_Result Core_InlineArrayDequeP_pop_back(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Boolean steal);

#endif // DX_INLINE_POINTER_DEQUE_H_INCLUDED
