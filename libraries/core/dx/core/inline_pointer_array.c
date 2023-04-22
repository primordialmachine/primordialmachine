// Copyright (c) 2018-2023 Michael Heilmann
#include "dx/core/inline_pointer_array.h"

#include "dx/core/memory.h"
#include "dx/core/_get_best_array_size.h"

/// @brief The greatest capacity, in elements, of a pointer array.
static dx_size const GREATEST_CAPACITY = DX_SIZE_GREATEST / sizeof(void*);

/// @brief The least capacity, in elements, of a pointer array.
static dx_size const LEAST_CAPACITY = 0;

typedef struct _dx_impl {

  /// @brief A pointer to an array of @a capacity @a (void *) elements.
  void** elements;
  /// @brief The capacity, in elements, of the array pointed to by @a array.
  dx_size capacity;
  /// @brief The number of elements in this array.
  dx_size size;

  /// @brief A pointer to the @a dx_added_callback function or a null pointer.
  dx_inline_pointer_array_added_callback* added_callback;
  /// @brief A pointer to the @a dx_removed_callback function or  a null pointer.
  dx_inline_pointer_array_removed_callback* removed_callback;

} _dx_impl; // struct _dx_impl

/// @internal
/// @brief Initialize this _dx_impl object object.
/// @param SELF A pointer to this uninitalized _dx_impl object.
/// @param initial_capacity The initial capacity. Can be @a 0.
/// @param configuration A pointer to the configuration.
/// @default-return
/// @default-failure
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a initial_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_initialize(_dx_impl* SELF, dx_size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration);

/// @internal
/// @brief Uninitialize this _dx_impl object.
/// @param SELF A pointer to this initialized _dx_impl object.
static void _dx_impl_uninitialize(_dx_impl* SELF);

/// @internal
/// @brief Get the size, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @default-runtime-calling-convention
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result _dx_impl_get_size(dx_size* RETURN, _dx_impl const* self);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @default-runtime-calling-convention
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result _dx_impl_get_capacity(dx_size* RETURN, _dx_impl const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @default-runtime-calling-convention
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static inline dx_result _dx_impl_get_free_capacity(dx_size* RETURN, _dx_impl const* SELF);

/// @internal
/// @brief Increase the capacity.
/// @param self A pointer to this _dx_impl object.
/// @param additional_capacity The amount to increase the capacity by.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
static dx_result _dx_impl_increase_capacity(_dx_impl* SELF, dx_size additional_capacity);

/// @internal
/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_free_capacity The required free capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a required_free_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_ensure_free_capacity(_dx_impl* SELF, dx_size required_free_capacity);

/// @internal
/// @brief Remove all elements.
/// @param SELF A pointer to this _dx_impl object.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static dx_result _dx_impl_clear(_dx_impl* SELF);

/// @internal
/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>dx_inline_pointer_array_element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than or equal to the size of the array.
static dx_result _dx_impl_get_at(dx_inline_pointer_array_element* RETURN, _dx_impl const* SELF, dx_size index);

/// @internal
/// @brief Insert an element.
/// @param self A pointer to this _dx_impl object.
/// @param pointer The element.
/// @param index The index.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of the array
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_insert(_dx_impl* self, dx_inline_pointer_array_element pointer, dx_size index);

/// @internal
/// @brief Append an element.
/// @param self A pointer to this _dx_impl object.
/// @param pointer The element.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_append(_dx_impl* self, dx_inline_pointer_array_element pointer);

/// @internal
/// @brief Prepend an element.
/// @param self A pointer to this _dx_impl object.
/// @param pointer The element.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_prepend(_dx_impl* self, dx_inline_pointer_array_element pointer);

/// @internal
/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this _dx_impl object.
/// @param n The number of objects to remove.
/// @default-runtime-calling-convention
/// @remarks If this function fails, no elements were removed.
/// @error #DX_ERROR_INVALID_ARGUMENT @a n is greater than the size of the _dx_impl object.
static dx_result _dx_impl_pop_back_n(_dx_impl* SELF, dx_size n);

static dx_result _dx_impl_initialize(_dx_impl* SELF, dx_size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size overflow = 0;
  dx_size initial_capacity_bytes = dx_mul_sz(initial_capacity, sizeof(dx_inline_pointer_array_element), &overflow);
  if (overflow) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  void** elements = NULL;
  if (dx_memory_allocate(&elements, initial_capacity_bytes)) {
    return DX_FAILURE;
  }
  SELF->size = 0;
  SELF->elements = elements;
  SELF->capacity = initial_capacity;
  SELF->added_callback = configuration->added_callback;
  SELF->removed_callback = configuration->removed_callback;
  return DX_SUCCESS;
}

static void _dx_impl_uninitialize(_dx_impl* SELF) {
  _dx_impl_clear(SELF);
  dx_memory_deallocate(SELF->elements);
  SELF->elements = NULL;
  SELF->capacity = 0;
}

static inline dx_result _dx_impl_get_size(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->size;
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_get_capacity(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity;
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_get_free_capacity(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity - SELF->size;
  return DX_SUCCESS;
}

static dx_result _dx_impl_increase_capacity(_dx_impl* SELF, dx_size additional_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!additional_capacity) {
    return DX_SUCCESS;
  }
  dx_set_error(DX_NO_ERROR);
  dx_size new_capacity = 0;
  if (dx_get_best_array_size(&new_capacity, SELF->capacity, additional_capacity, LEAST_CAPACITY, GREATEST_CAPACITY, true)) {
    return DX_FAILURE;
  }
  dx_size overflow = 0;
  dx_size new_capacity_bytes = dx_mul_sz(new_capacity, sizeof(dx_inline_pointer_array_element), &overflow);
  if (overflow) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  dx_inline_pointer_array_element* new_elements = dx_memory_reallocate(SELF->elements, new_capacity_bytes);
  if (!new_elements) {
    return DX_FAILURE;
  }
  SELF->capacity = new_capacity;
  SELF->elements = new_elements;
  return DX_SUCCESS;
}

static dx_result _dx_impl_ensure_free_capacity(_dx_impl* SELF, dx_size required_free_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size available_free_capacity = SELF->capacity - SELF->size;
  if (available_free_capacity >= required_free_capacity) {
    return DX_SUCCESS;
  }
  dx_size additional_capacity = required_free_capacity - available_free_capacity;
  return _dx_impl_increase_capacity(SELF, additional_capacity);
}

static dx_result _dx_impl_clear(_dx_impl* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (SELF->removed_callback) {
    dx_inline_pointer_array_removed_callback* removed_callback = SELF->removed_callback;
    while (SELF->size > 0) {
      void* element = SELF->elements[--SELF->size];
      removed_callback(&element);
    }
  } else {
    SELF->size = 0;
  }
  return DX_SUCCESS;
}

static dx_result _dx_impl_get_at(dx_inline_pointer_array_element* RETURN, _dx_impl const* SELF, dx_size index) {
  if (!SELF || index >= SELF->size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->elements[index];
  return DX_SUCCESS;
}

static dx_result _dx_impl_insert(_dx_impl* SELF, dx_inline_pointer_array_element pointer, dx_size index) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (index > SELF->size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (_dx_impl_ensure_free_capacity(SELF, 1)) {
    return DX_FAILURE;
  }
  if (SELF->added_callback) {
    SELF->added_callback(&pointer);
  }
  if (index != SELF->size) {
    dx_memory_move(SELF->elements + index + 1,
                   SELF->elements + index + 0,
                   (SELF->size - index) * sizeof(dx_inline_pointer_array_element));
  }
  SELF->elements[index] = pointer;
  SELF->size++;
  return DX_SUCCESS;
}

static dx_result _dx_impl_append(_dx_impl* SELF, dx_inline_pointer_array_element pointer) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_insert(SELF, pointer, SELF->size);
}

static dx_result _dx_impl_prepend(_dx_impl* SELF, dx_inline_pointer_array_element pointer) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_insert(SELF, pointer, 0);
}

static dx_result _dx_impl_pop_back_n(_dx_impl* SELF, dx_size n) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (n > SELF->size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (n == 0) {
    return DX_SUCCESS;
  }
  if (SELF->removed_callback) {
    for (dx_size i = SELF->size - n, k = SELF->size; i < k; ++i) {
      SELF->removed_callback(SELF->elements[i]);
    }
  }
  SELF->size -= n;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

static inline _dx_impl const* _DX_IMPL_CONST(void* p) {
  return (_dx_impl const*)p;
}

dx_result dx_inline_pointer_array_initialize(dx_inline_pointer_array *SELF, dx_size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (dx_memory_allocate(&SELF->pimpl, sizeof(_dx_impl))) {
    return DX_FAILURE;
  }
  if (_dx_impl_initialize(_DX_IMPL(SELF->pimpl), initial_capacity, configuration)) {
    dx_memory_deallocate(SELF->pimpl);
    SELF->pimpl = NULL;
    return DX_FAILURE;
  }
 return DX_SUCCESS;
}

void dx_inline_pointer_array_uninitialize(dx_inline_pointer_array* SELF) {
  _dx_impl_uninitialize(_DX_IMPL(SELF->pimpl));
  dx_memory_deallocate(SELF->pimpl);
  SELF->pimpl = NULL;
}

dx_result dx_inline_pointer_array_increase_capacity(dx_inline_pointer_array* SELF, dx_size additional_capacity) {
  return _dx_impl_increase_capacity(_DX_IMPL(SELF->pimpl), additional_capacity);
}

dx_result dx_inline_pointer_array_ensure_free_capacity(dx_inline_pointer_array* SELF, dx_size required_free_capacity) {
  return _dx_impl_ensure_free_capacity(_DX_IMPL(SELF->pimpl), required_free_capacity);
}

dx_result dx_inline_pointer_array_append(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer) {
  return _dx_impl_append(_DX_IMPL(SELF->pimpl), pointer);
}

dx_result dx_inline_pointer_array_prepend(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer) {
  return _dx_impl_prepend(_DX_IMPL(SELF->pimpl), pointer);
}

dx_result dx_inline_pointer_array_insert(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer, dx_size index) {
  return _dx_impl_insert(_DX_IMPL(SELF->pimpl), pointer, index);
}

dx_result dx_inline_pointer_array_get_at(dx_inline_pointer_array_element* RETURN, dx_inline_pointer_array const* self, dx_size index) {
  return _dx_impl_get_at(RETURN, _DX_IMPL_CONST(self->pimpl), index);
}

dx_result dx_inline_pointer_array_get_size(dx_size* RETURN, dx_inline_pointer_array const* SELF) {
  return _dx_impl_get_size(RETURN, _DX_IMPL(SELF->pimpl));
}

dx_result dx_inline_pointer_array_get_capacity(dx_size* RETURN, dx_inline_pointer_array const* SELF) {
  return _dx_impl_get_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

dx_result dx_inline_pointer_array_get_free_capacity(dx_size* RETURN, dx_inline_pointer_array const* SELF) {
  return _dx_impl_get_free_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

dx_result dx_inline_pointer_array_clear(dx_inline_pointer_array* SELF) {
  return _dx_impl_clear(_DX_IMPL(SELF->pimpl));
}

dx_result dx_inline_pointer_array_pop_back_n(dx_inline_pointer_array* SELF, dx_size n) {
  return _dx_impl_pop_back_n(_DX_IMPL(SELF->pimpl), n);
}