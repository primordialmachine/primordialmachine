// Copyright (c) 2018-2023 Michael Heilmann
#include "dx/core/inline_pointer_array.h"

#include "Core/Memory.h"
#include "Core/safeMulNx.h"
#include "dx/core/_get_best_array_size.h"

/// @brief The greatest capacity, in elements, of a pointer array.
static Core_Size const GREATEST_CAPACITY = Core_Size_Greatest / sizeof(void*);

/// @brief The least capacity, in elements, of a pointer array.
static Core_Size const LEAST_CAPACITY = 0;

typedef struct _dx_impl {

  /// @brief A pointer to an array of @a capacity @a (void *) elements.
  void** elements;
  /// @brief The capacity, in elements, of the array pointed to by @a array.
  Core_Size capacity;
  /// @brief The number of elements in this array.
  Core_Size size;

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
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_Size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration);

/// @internal
/// @brief Uninitialize this _dx_impl object.
/// @param SELF A pointer to this initialized _dx_impl object.
static void _dx_impl_uninitialize(_dx_impl* SELF);

/// @internal
/// @brief Get the size, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @method-call
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static inline Core_Result _dx_impl_get_size(Core_Size* RETURN, _dx_impl const* self);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @method-call
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static inline Core_Result _dx_impl_get_capacity(Core_Size* RETURN, _dx_impl const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @method-call
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static inline Core_Result _dx_impl_get_free_capacity(Core_Size* RETURN, _dx_impl const* SELF);

/// @internal
/// @brief Increase the capacity.
/// @param self A pointer to this _dx_impl object.
/// @param additional_capacity The amount to increase the capacity by.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
static Core_Result _dx_impl_increase_capacity(_dx_impl* SELF, Core_Size additional_capacity);

/// @internal
/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_free_capacity The required free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed @a required_free_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_ensure_free_capacity(_dx_impl* SELF, Core_Size required_free_capacity);

/// @internal
/// @brief Remove all elements.
/// @param SELF A pointer to this _dx_impl object.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static Core_Result _dx_impl_clear(_dx_impl* SELF);

/// @internal
/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>dx_inline_pointer_array_element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of the array.
static Core_Result _dx_impl_get_at(dx_inline_pointer_array_element* RETURN, _dx_impl const* SELF, Core_Size index);

/// @internal
/// @brief Insert an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @param index The index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of the array
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_insert(_dx_impl* SELF, dx_inline_pointer_array_element pointer, Core_Size index);

/// @internal
/// @brief Append an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_append(_dx_impl* SELF, dx_inline_pointer_array_element pointer);

/// @internal
/// @brief Prepend an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_prepend(_dx_impl* SELF, dx_inline_pointer_array_element pointer);

/// @internal
/// @brief Remove the specified number of elements.
/// @param SELF A pointer to this _dx_impl object.
/// @param n The number of objects to remove.
/// @method-call
/// @remarks If this function fails, no elements were removed.
/// @error #Core_Error_ArgumentInvalid @a n is greater than the size of the _dx_impl object.
static Core_Result _dx_impl_pop_back_n(_dx_impl* SELF, Core_Size n);

static Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_Size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size overflow = 0;
  Core_Size initial_capacity_bytes;
  Core_safeMulSz(&initial_capacity_bytes, initial_capacity, sizeof(dx_inline_pointer_array_element), &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  void** elements = NULL;
  if (Core_Memory_allocate((void**)&elements, initial_capacity_bytes)) {
    return Core_Failure;
  }
  SELF->size = 0;
  SELF->elements = elements;
  SELF->capacity = initial_capacity;
  SELF->added_callback = configuration->added_callback;
  SELF->removed_callback = configuration->removed_callback;
  return Core_Success;
}

static void _dx_impl_uninitialize(_dx_impl* SELF) {
  _dx_impl_clear(SELF);
  Core_Memory_deallocate(SELF->elements);
  SELF->elements = NULL;
  SELF->capacity = 0;
}

static inline Core_Result _dx_impl_get_size(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->size;
  return Core_Success;
}

static inline Core_Result _dx_impl_get_capacity(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity;
  return Core_Success;
}

static inline Core_Result _dx_impl_get_free_capacity(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity - SELF->size;
  return Core_Success;
}

static Core_Result _dx_impl_increase_capacity(_dx_impl* SELF, Core_Size additional_capacity) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!additional_capacity) {
    return Core_Success;
  }
  Core_setError(Core_Error_NoError);
  Core_Size new_capacity = 0;
  if (dx_get_best_array_size(&new_capacity, SELF->capacity, additional_capacity, LEAST_CAPACITY, GREATEST_CAPACITY, true)) {
    return Core_Failure;
  }
  Core_Size overflow = 0;
  Core_Size new_capacity_bytes;
  Core_safeMulSz(&new_capacity_bytes, new_capacity, sizeof(dx_inline_pointer_array_element), &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  if (Core_Memory_reallocate((void**)&SELF->elements, SELF->elements, new_capacity_bytes)) {
    return Core_Failure;
  }
  SELF->capacity = new_capacity;
  return Core_Success;
}

static Core_Result _dx_impl_ensure_free_capacity(_dx_impl* SELF, Core_Size required_free_capacity) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size available_free_capacity = SELF->capacity - SELF->size;
  if (available_free_capacity >= required_free_capacity) {
    return Core_Success;
  }
  Core_Size additional_capacity = required_free_capacity - available_free_capacity;
  return _dx_impl_increase_capacity(SELF, additional_capacity);
}

static Core_Result _dx_impl_clear(_dx_impl* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
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
  return Core_Success;
}

static Core_Result _dx_impl_get_at(dx_inline_pointer_array_element* RETURN, _dx_impl const* SELF, Core_Size index) {
  if (!SELF || index >= SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->elements[index];
  return Core_Success;
}

static Core_Result _dx_impl_insert(_dx_impl* SELF, dx_inline_pointer_array_element pointer, Core_Size index) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (index > SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_dx_impl_ensure_free_capacity(SELF, 1)) {
    return Core_Failure;
  }
  if (SELF->added_callback) {
    SELF->added_callback(&pointer);
  }
  if (index != SELF->size) {
    Core_Memory_move(SELF->elements + index + 1,
                     SELF->elements + index + 0,
                     (SELF->size - index) * sizeof(dx_inline_pointer_array_element));
  }
  SELF->elements[index] = pointer;
  SELF->size++;
  return Core_Success;
}

static Core_Result _dx_impl_append(_dx_impl* SELF, dx_inline_pointer_array_element pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insert(SELF, pointer, SELF->size);
}

static Core_Result _dx_impl_prepend(_dx_impl* SELF, dx_inline_pointer_array_element pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insert(SELF, pointer, 0);
}

static Core_Result _dx_impl_pop_back_n(_dx_impl* SELF, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (n > SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (n == 0) {
    return Core_Success;
  }
  if (SELF->removed_callback) {
    for (Core_Size i = SELF->size - n, k = SELF->size; i < k; ++i) {
      SELF->removed_callback(SELF->elements[i]);
    }
  }
  SELF->size -= n;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

static inline _dx_impl const* _DX_IMPL_CONST(void* p) {
  return (_dx_impl const*)p;
}

Core_Result dx_inline_pointer_array_initialize(dx_inline_pointer_array *SELF, Core_Size initial_capacity, DX_INLINE_POINTER_ARRAY_CONFIGURATION const* configuration) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_Memory_allocate(&SELF->pimpl, sizeof(_dx_impl))) {
    return Core_Failure;
  }
  if (_dx_impl_initialize(_DX_IMPL(SELF->pimpl), initial_capacity, configuration)) {
    Core_Memory_deallocate(SELF->pimpl);
    SELF->pimpl = NULL;
    return Core_Failure;
  }
 return Core_Success;
}

void dx_inline_pointer_array_uninitialize(dx_inline_pointer_array* SELF) {
  _dx_impl_uninitialize(_DX_IMPL(SELF->pimpl));
  Core_Memory_deallocate(SELF->pimpl);
  SELF->pimpl = NULL;
}

Core_Result dx_inline_pointer_array_increase_capacity(dx_inline_pointer_array* SELF, Core_Size additional_capacity) {
  return _dx_impl_increase_capacity(_DX_IMPL(SELF->pimpl), additional_capacity);
}

Core_Result dx_inline_pointer_array_ensure_free_capacity(dx_inline_pointer_array* SELF, Core_Size required_free_capacity) {
  return _dx_impl_ensure_free_capacity(_DX_IMPL(SELF->pimpl), required_free_capacity);
}

Core_Result dx_inline_pointer_array_append(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer) {
  return _dx_impl_append(_DX_IMPL(SELF->pimpl), pointer);
}

Core_Result dx_inline_pointer_array_prepend(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer) {
  return _dx_impl_prepend(_DX_IMPL(SELF->pimpl), pointer);
}

Core_Result dx_inline_pointer_array_insert(dx_inline_pointer_array* SELF, dx_inline_pointer_array_element pointer, Core_Size index) {
  return _dx_impl_insert(_DX_IMPL(SELF->pimpl), pointer, index);
}

Core_Result dx_inline_pointer_array_get_at(dx_inline_pointer_array_element* RETURN, dx_inline_pointer_array const* SELF, Core_Size index) {
  return _dx_impl_get_at(RETURN, _DX_IMPL_CONST(SELF->pimpl), index);
}

Core_Result dx_inline_pointer_array_get_size(Core_Size* RETURN, dx_inline_pointer_array const* SELF) {
  return _dx_impl_get_size(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result dx_inline_pointer_array_get_capacity(Core_Size* RETURN, dx_inline_pointer_array const* SELF) {
  return _dx_impl_get_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result dx_inline_pointer_array_get_free_capacity(Core_Size* RETURN, dx_inline_pointer_array const* SELF) {
  return _dx_impl_get_free_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result dx_inline_pointer_array_clear(dx_inline_pointer_array* SELF) {
  return _dx_impl_clear(_DX_IMPL(SELF->pimpl));
}

Core_Result dx_inline_pointer_array_pop_back_n(dx_inline_pointer_array* SELF, Core_Size n) {
  return _dx_impl_pop_back_n(_DX_IMPL(SELF->pimpl), n);
}