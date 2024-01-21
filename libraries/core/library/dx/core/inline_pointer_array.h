/// @file dx/core/inline_pointer_array.h
/// @brief A deque for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_INLINE_POINTER_ARRAY_H_INCLUDED)
#define DX_CORE_INLINE_POINTER_ARRAY_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup Core_Aggregates
/// @brief The elements of a Core_InlinePointerArray are pointers.
typedef void* Core_InlinePointerArray_element;

/// @ingroup Core_Aggregates
/// @brief Type of a callback invoked if an element was added.
/// @param element A pointer to the element.
typedef void (Core_InlinePointerArray_added_callback)(Core_InlinePointerArray_element* element);

/// @ingroup Core_Aggregates
/// @brief Type of a callback invoked if an element was removed.
/// @param element A pointer to the element.
typedef void (Core_InlinePointerArray_removed_callback)(Core_InlinePointerArray_element* element);

/// @ingroup Core_Aggregates
/// @brief A dynamic array of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct Core_InlinePointerArray Core_InlinePointerArray;

struct Core_InlinePointerArray {
  void* pimpl;
}; // struct Core_InlinePointerArray

/// @ingroup Core_Aggregates
/// @brief Configuration of a pointer array.
typedef struct DX_INLINE_POINTER_ARRAY_CONFIGURATION {

  /// @brief A pointer to the @a _dx_added_callback function or a null pointer.
  Core_InlinePointerArray_added_callback* added_callback;

  /// @brief A pointer to the @a _dx_removed_callback function or  a null pointer.
  Core_InlinePointerArray_removed_callback* removed_callback;

} DX_INLINE_POINTER_ARRAY_CONFIGURATION;

/// @brief Initialize this Core_InlinePointerArray object.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @method{Core_InlinePointerArray}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result Core_InlinePointerArray_initialize(Core_InlinePointerArray* SELF, Core_Size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration);

/// @brief Uninitialize this Core_InlinePointerArray object.
/// @param SELF A pointer to this Core_InlinePointerArray object.
void Core_InlinePointerArray_uninitialize(Core_InlinePointerArray* SELF);

/* http://localhost/core#core-inlinearray-increasecapacity */
/// @brief Increase the capacity.
/// @param additionalCapacity The amount to increase the capacity by.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
/// @method{Core_InlinePointerArray}
Core_Result Core_InlinePointerArray_increaseCapacity(Core_InlinePointerArray* SELF, Core_Size additional_capacity);

/* http://localhost/core#core-inlinearray-ensurefreecapacity */
/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param requiredFreeCapacity The required free capacity.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a requiredFreeCapacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The free capacity is greater than or equal to the specified required free capacity.
/// @method{Core_InlinePointerArray}
Core_Result Core_InlinePointerArray_ensureFreeCapacity(Core_InlinePointerArray* SELF, Core_Size requiredFreeCapacity);

/// @brief Append an element.
/// @param pointer The element.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{Core_InlinePointerArray}
Core_Result Core_InlinePointerArray_append(Core_InlinePointerArray* SELF, Core_InlinePointerArray_element pointer);

/// @brief Prepend an element.
/// @param pointer The element.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{Core_InlinePointerArray}
Core_Result Core_InlinePointerArray_prepend(Core_InlinePointerArray* SELF, Core_InlinePointerArray_element pointer);

/// @brief Insert an element.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @param pointer The element.
/// @param index The index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this Core_InlinePointerArray object
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result Core_InlinePointerArray_insert(Core_InlinePointerArray* SELF, Core_InlinePointerArray_element pointer, Core_Size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>void*</code> variable.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of this Core_InlinePointerArray object
Core_Result Core_InlinePointerArray_get_at(Core_InlinePointerArray_element* RETURN, Core_InlinePointerArray const* SELF, Core_Size index);

/// @brief Get the size, in elements.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
Core_Result Core_InlinePointerArray_getSize(Core_Size* RETURN, Core_InlinePointerArray const* SELF);

/// @brief Get the capacity, in elements.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
Core_Result Core_InlinePointerArray_getCapacity(Core_Size* RETURN, Core_InlinePointerArray const* SELF);

/// @brief Get the free capacity, in elements.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
Core_Result Core_InlinePointerArray_getFreeCapacity(Core_Size* RETURN, Core_InlinePointerArray const* SELF);

/// @brief Remove all elements.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
Core_Result Core_InlinePointerArray_clear(Core_InlinePointerArray* SELF);

/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this Core_InlinePointerArray object.
/// @param n The number of objects to remove.
/// @method-call
/// @remarks If this function fails, no elements were removed.
/// @error #Core_Error_ArgumentInvalid @a n is greater than the size of the Core_InlinePointerArray object.
Core_Result Core_InlinePointerArray_pop_back_n(Core_InlinePointerArray* SELF, Core_Size n);

#endif // DX_INLINE_CORE_POINTER_ARRAY_H_INCLUDED
