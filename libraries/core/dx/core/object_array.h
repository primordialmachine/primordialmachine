#if !defined(DX_CORE_OBJECT_ARRAY_H_INCLUDED)
#define DX_CORE_OBJECT_ARRAY_H_INCLUDED

#include "dx/core/object.h"
#include "dx/core/inline_object_array.h"

DX_DECLARE_OBJECT_TYPE("dx.object_array",
                       dx_object_array,
                       dx_object);

static inline dx_object_array* DX_OBJECT_ARRAY(void* p) {
  return (dx_object_array*)p;
}

struct dx_object_array {
  dx_object _parent;
  dx_inline_object_array backend;
};

static inline dx_object_array_dispatch* DX_OBJECT_ARRAY_DISPATCH(void* p) {
  return (dx_object_array_dispatch*)p;
}

struct dx_object_array_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Initialize this dx_object_array object.
/// @param SELF A pointer to this dx_object_array object.
/// @param initial_capacity The initialc capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a initial_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_object_array_construct(dx_object_array* SELF, dx_size initial_capacity);

dx_result dx_object_array_create(dx_object_array** RETURN, dx_size initial_capacity);

/// @brief Increase the capacity.
/// @param SELF A pointer to this dx_object_array object.
/// @param additional_capacity The amount to increase the capacity by.
///// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
static inline dx_result dx_object_array_increase_capacity(dx_object_array* SELF, dx_size additional_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_increase_capacity(&SELF->backend, additional_capacity);
}

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this dx_object_array object.
/// @param required_free_capacitiy The required free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a required_free_capacity is too big
/// @errpr #DX_ERROR_ALLOCATION_FAILED an allocation failed
static inline dx_result dx_object_array_ensure_free_capacity(dx_object_array* SELF, dx_size required_free_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_ensure_free_capacity(&SELF->backend, required_free_capacity);
}

/// @brief Append an element.
/// @param SELF A pointer to this dx_object_array object.
/// @param pointer The element.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a pointer is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static inline dx_result dx_object_array_append(dx_object_array* SELF, dx_object* pointer) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_append(&SELF->backend, pointer);
}

/// @brief Prepend an element.
/// @param SELF A pointer to this dx_object_array object.
/// @param pointer The element.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a pointer is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static inline dx_result dx_object_array_prepend(dx_object_array* SELF, dx_object* pointer) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_prepend(&SELF->backend, pointer);
}

/// @brief Insert an object.
/// @param SELF A pointer to this dx_object_array object.
/// @param pointer A pointer to the object.
/// @param index The index.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a pointer is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of this dx_object_array object
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static inline dx_result dx_object_array_insert(dx_object_array* SELF, dx_object* pointer, dx_size index) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_insert(&SELF->backend, pointer, index);
}

/// @brief Get the pointer to the object at the specified index.
/// @param RETURN A pointer to a <code>dx_object*</code> variable.
/// @param SELF A pointer to this dx_object_array object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned a pointer to the object
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result dx_object_array_get_at(dx_object** RETURN, dx_object_array const* SELF, dx_size index) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_get_at(RETURN, &SELF->backend, index);
}

/// @brief Get the size, in objects.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this dx_object_array object.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result dx_object_array_get_size(dx_size* RETURN, dx_object_array const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_get_size(RETURN, &SELF->backend);
}

/// @brief Get the capacity, in objects.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this dx_object_array object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result dx_object_array_get_capacity(dx_size* RETURN, dx_object_array const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_get_capacity(RETURN, &SELF->backend);
}

/// @brief Get the free capacity, in objects.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this dx_object_array object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
static inline dx_result dx_object_array_get_free_capacity(dx_size* RETURN, dx_object_array const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_get_free_capacity(RETURN, &SELF->backend);
}

/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this dx_object_array object.
/// @param n The number of objects to remove.
/// @method-call
/// @remarks If this function fails, no elements were removed.
/// @error #DX_ERROR_INVALID_ARGUMENT @a n is greater than the size of the dx_object_array object.
static inline dx_result dx_object_array_pop_back_n(dx_object_array* SELF, dx_size n) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_pop_back_n(&SELF->backend, n);
}

/// @brief Remove all elements.
/// @param SELF A pointer to this dx_object_array object.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result dx_object_array_clear(dx_object_array* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_object_array_clear(&SELF->backend);
}

#endif // DX_CORE_OBJECT_ARRAY_H_INCLUDED
