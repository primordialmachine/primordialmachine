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
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_object_array_initialize(dx_inline_object_array* SELF, Core_Size initial_capacity);

/// @brief Uninitialize this dx_inline_object_array object.
/// @param self A pointer to this dx_inline_object_array object.
void dx_inline_object_array_uninitialize(dx_inline_object_array* SELF);

/// @brief Increase the capacity.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param additional_capacity The amount to increase the capacity by.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
Core_Result dx_inline_object_array_increase_capacity(dx_inline_object_array* SELF, Core_Size additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param required_free_capacitiy The required free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a required_free_capacity is too big
/// @errpr #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_object_array_ensure_free_capacity(dx_inline_object_array* SELF, Core_Size required_free_capacity);

/// @brief Append an element.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param pointer The element.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a pointer is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_object_array_append(dx_inline_object_array* SELF, Core_Object* pointer);

/// @brief Prepend an element.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param pointer The element.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a pointer is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_object_array_prepend(dx_inline_object_array* SELF, Core_Object* pointer);

/// @brief Insert an object.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param pointer A pointer to the object.
/// @param index The index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a pointer is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this dx_inline_object_array object
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_object_array_insert(dx_inline_object_array* SELF, Core_Object* pointer, Core_Size index);

/// @brief Get the pointer to the object at the specified index.
/// @param RETURN A pointer to a <code>Core_Object*</code> variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned a pointer to the object
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
Core_Result dx_inline_object_array_get_at(Core_Object** RETURN, dx_inline_object_array const* SELF, Core_Size index);

/// @brief Get the size, in objects.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
Core_Result dx_inline_object_array_get_size(Core_Size* RETURN, dx_inline_object_array const* SELF);

/// @brief Get the capacity, in objects.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
Core_Result dx_inline_object_array_get_capacity(Core_Size* RETURN, dx_inline_object_array const* SELF);

/// @brief Get the free capacity, in objects.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
Core_Result dx_inline_object_array_get_free_capacity(Core_Size* RETURN, dx_inline_object_array const* SELF);

/// @brief Remove all elements.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @method-call
Core_Result dx_inline_object_array_clear(dx_inline_object_array* SELF);

/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this dx_inline_object_array object.
/// @param n The number of objects to remove.
/// @method-call
/// @remarks If this function fails, no elements were removed.
/// @error #Core_Error_ArgumentInvalid @a n is greater than the size of the dx_inline_object_array object.
Core_Result dx_inline_object_array_pop_back_n(dx_inline_object_array* SELF, Core_Size n);

#endif // DX_CORE_INLINE_OBJECT_ARRAY_H_INCLUDED
