#if !defined(DX_CORE_INLINE_OBJECT_ARRAY_H_INCLUDED)
#define DX_CORE_INLINE_OBJECT_ARRAY_H_INCLUDED

#include "dx/core/inline_pointer_array.h"
#include "dx/core/object.h"

/// @brief A dynamic array of (pointers to) objects. Also supports null pointers.
/// Support for reference counting objects.
typedef struct dx_inline_object_array dx_inline_object_array;

static inline dx_inline_object_array* DX_INLINE_OBJECT_ARRAY(void* p) {
  return (dx_inline_object_array*)p;
}

struct dx_inline_object_array {
  dx_inline_pointer_array backend;
};

/// @brief Initialize this dx_inline_object_array object.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param initial_capacity The initialc capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a initial_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_object_array_initialize(dx_inline_object_array* SELF, dx_size initial_capacity);

/// @brief Uninitialize this dx_inline_object_array object.
/// @param self A pointer to this dx_inline_object_array object.
void dx_inline_object_array_uninitialize(dx_inline_object_array* SELF);

/// @brief Increase the capacity.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param additional_capacity The amount to increase the capacity by.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
dx_result dx_inline_object_array_increase_capacity(dx_inline_object_array* SELF, dx_size additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param required_free_capacitiy The required free capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a required_free_capacity is too big
/// @errpr #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_object_array_ensure_free_capacity(dx_inline_object_array* SELF, dx_size required_free_capacity);

/// @brief Append an element.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param pointer The element.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a pointer is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_object_array_append(dx_inline_object_array* SELF, dx_object* pointer);

/// @brief Prepend an element.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param pointer The element.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a pointer is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_object_array_prepend(dx_inline_object_array* SELF, dx_object* pointer);

/// @brief Insert an object.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param pointer A pointer to the object.
/// @param index The index.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a pointer is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of this dx_inline_object_array object
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_object_array_insert(dx_inline_object_array* SELF, dx_object* pointer, dx_size index);

/// @brief Get the pointer to the object at the specified index.
/// @param RETURN A pointer to a <code>dx_object*</code> variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned a pointer to the object
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
dx_result dx_inline_object_array_get_at(dx_object** RETURN, dx_inline_object_array const* SELF, dx_size index);

/// @brief Get the size, in objects.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @success <code>*RETURN</code> was assigned the size.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
dx_result dx_inline_object_array_get_size(dx_size* RETURN, dx_inline_object_array const* SELF);

/// @brief Get the capacity, in objects.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
dx_result dx_inline_object_array_get_capacity(dx_size* RETURN, dx_inline_object_array const* SELF);

/// @brief Get the free capacity, in objects.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
dx_result dx_inline_object_array_get_free_capacity(dx_size* RETURN, dx_inline_object_array const* SELF);

/// @brief Remove all elements.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @default-runtime-calling-convention
dx_result dx_inline_object_array_clear(dx_inline_object_array* SELF);

/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param n The number of objects to remove.
/// @default-runtime-calling-convention
/// @remarks If this function fails, no elements were removed.
/// @error #DX_ERROR_INVALID_ARGUMENT @a n is greater than the size of the dx_inline_object_array object.
dx_result dx_inline_object_array_pop_back_n(dx_inline_object_array* SELF, dx_size n);

#endif // DX_CORE_INLINE_OBJECT_ARRAY_H_INCLUDED
