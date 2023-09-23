#include "dx/core/inline_pointer_deque.h"

#include "dx/core/memory.h"
#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/next_power_of_two.h"
#include "dx/core/_get_best_array_size.h"

/// @brief The greatest capacity, in elements, of a pointer array.
static dx_size const GREATEST_CAPACITY = DX_SIZE_GREATEST / sizeof(void*);

/// @brief The least capacity, in elements, of a pointer array.
static dx_size const LEAST_CAPACITY = 0;

static inline dx_size MAX(dx_size self, dx_size other);

static inline dx_size MIN(dx_size self, dx_size other);

static inline MOD(dx_size index, dx_size capacity);

static inline dx_size MAX(dx_size self, dx_size other) {
  return self > other ? self : other;
}

static inline dx_size MIN(dx_size self, dx_size other) {
  return self < other ? self : other;
}

static inline MOD(dx_size index, dx_size capacity) {
  return index % capacity;
}

typedef struct _dx_impl {
  
  /// @brief A pointer to an array of @a capacity @a (void *) elements.
  void** elements;
  /// @brief The capacity, in elements, of the array pointed to by @a array.
  dx_size capacity;
  /// @brief The number of elements in this array.
  dx_size size;
  
  /// @brief The position to read the next element from.
  dx_size read;

  /// @brief A pointer to the @a dx_added_callback function or a null pointer.
  dx_inline_pointer_deque_added_callback* added_callback;
  /// @brief A pointer to the @a dx_removed_callback function or  a null pointer.
  dx_inline_pointer_deque_removed_callback* removed_callback;

} _dx_impl; // struct _dx_impl

/// @brief Initialize this _dx_impl object.
/// @param self A pointer to this _dx_impl object.
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @default-return
/// @default-failure
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a initial_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_initialize(_dx_impl* self, dx_size initial_capacity, DX_INLINE_POINTER_DEQUE_CONFIGURATION const* configuration);

/// @brief Uninitialize this _dx_impl object.
/// @param self A pointer to this _dx_impl object.
static void _dx_impl_uninitialize(_dx_impl* self);

/// @brief Get the size, in elements.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static dx_result _dx_impl_get_size(dx_size* RETURN, _dx_impl const* SELF);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static dx_result _dx_impl_get_capacity(dx_size* RETURN, _dx_impl const* self);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static dx_result _dx_impl_get_free_capacity(dx_size* RETURN, _dx_impl const* SELF);

/// @brief Increase the capacity.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_additional_capacity The amount to increase the capacity by.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
static dx_result _dx_impl_increase_capacity(_dx_impl* SELF, dx_size required_additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_free_capacity The required free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a required_free_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The free capacity is greater than or equal to the specified required free capacity.
static dx_result _dx_impl_ensure_free_capacity(_dx_impl* SELF, dx_size required_free_capacity);

/// @brief Remove all elements.
/// @param SELF A pointer to this _dx_impl object.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
static dx_result _dx_impl_clear(_dx_impl* SELF);

/// @brief Append an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_push_back(_dx_impl* SELF, dx_inline_pointer_deque_element pointer);

/// @brief Prepend an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_push_front(_dx_impl* SELF, dx_inline_pointer_deque_element pointer);

/// @brief Insert an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @param index The index.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of this _dx_impl object
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
static dx_result _dx_impl_insert(_dx_impl* SELF, dx_inline_pointer_deque_element pointer, dx_size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a dx_inline_pointer_deque_element variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than the size of this _dx_impl object
static dx_result _dx_impl_get_at(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_size index);

/// @brief Remove the pointer at the specified index.
/// @param RETURN A pointer to a dx_inline_pointer_deque_element variable.
/// @param SELF A pointer to the _dx_impl object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @param index The index.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a index is greater than or equal to the size of this _dx_impl object
static dx_result _dx_impl_remove(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_bool steal, dx_size index);

/// @brief Pop the first pointer of the deque.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the first pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT the deque is empty.
static dx_result _dx_impl_pop_front(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_bool steal);

/// @brief Pop the last pointer of the deque.
/// @param RETURN A pointer to a <code>dx_inline_pointer_deque_element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the last pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT the deque is empty
static dx_result _dx_impl_pop_back(dx_inline_pointer_deque_element * RETURN, _dx_impl * SELF, dx_bool steal);

static dx_result _dx_impl_initialize(_dx_impl* self, dx_size initial_capacity, DX_INLINE_POINTER_DEQUE_CONFIGURATION const* configuration) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size overflow;
  dx_size initial_capacity_bytes = dx_mul_sz(initial_capacity, sizeof(dx_inline_pointer_deque_element), &overflow);
  if (overflow) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  void** elements = NULL;
  if (dx_memory_allocate(&elements, initial_capacity_bytes)) {
    return DX_FAILURE;
  }
  self->read = 0;
  self->size = 0;
  self->elements = elements;
  self->capacity = initial_capacity;
  self->added_callback = configuration->added_callback;
  self->removed_callback = configuration->removed_callback;
  return DX_SUCCESS;
}

static void _dx_impl_uninitialize(_dx_impl* self) {
  _dx_impl_clear(self);
  dx_memory_deallocate(self->elements);
  self->elements = NULL;
  self->capacity = 0; 
}

static dx_result _dx_impl_get_size(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->size;
  return DX_SUCCESS;
}

static dx_result _dx_impl_get_capacity(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity;
  return DX_SUCCESS;
}

static dx_result _dx_impl_get_free_capacity(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity - SELF->size;
  return DX_SUCCESS;
}

static dx_result _dx_impl_increase_capacity(_dx_impl* SELF, dx_size required_additional_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!required_additional_capacity) {
    return DX_SUCCESS;
  }
  dx_size new_capacity = 0;
  if (dx_get_best_array_size(&new_capacity, SELF->capacity, required_additional_capacity, LEAST_CAPACITY, GREATEST_CAPACITY, DX_TRUE)) {
    return DX_FAILURE;
  }
  void** new_elements = NULL;
  if (dx_memory_allocate(&new_elements, new_capacity * sizeof(void*))) {
    return DX_FAILURE;
  }
  for (dx_size i = 0, n = SELF->size; i < n; ++i) {
    new_elements[i] = SELF->elements[MOD(SELF->read + i, SELF->capacity)];
  }
  dx_memory_deallocate(SELF->elements);
  SELF->elements = new_elements;
  SELF->capacity = new_capacity;
  SELF->read = 0;
  return DX_SUCCESS;
}

static dx_result _dx_impl_ensure_free_capacity(_dx_impl* SELF, dx_size required_free_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size available_free_capacity = SELF->capacity - SELF->size;
  if (available_free_capacity > required_free_capacity) {
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
    while (SELF->size > 0) {
      void *element = SELF->elements[SELF->read];
      SELF->read = MOD(SELF->read + 1, SELF->capacity);
      SELF->size--;
    }
  } else {
    SELF->size = 0;
  }
  SELF->read = 0;
  return DX_SUCCESS;
}

static dx_result _dx_impl_push_back(_dx_impl* SELF, dx_inline_pointer_deque_element pointer) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_insert(SELF, pointer, SELF->size);
}

static dx_result _dx_impl_push_front(_dx_impl* SELF, dx_inline_pointer_deque_element pointer) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_insert(SELF, pointer, 0);
}

static dx_result _dx_impl_insert(_dx_impl* SELF, dx_inline_pointer_deque_element pointer, dx_size index) {
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
  dx_size const capacity = SELF->capacity; // The capacity does not change anymore for the rest of this
                                           // function but is frequently referenced, hence it is cached
                                           // here.

  // The goal of the following algorithm is to minimize the number of shift operations when inserting into an index
  // smaller than size.

  // Assume the deque is not empty.
  // Treating the deque as an infinite array,
  // ..., a[0],...,a[read],...,a[read+index], ..., a[read+size-1]
  // then
  // a) the index of the first element is "read" and
  // b) the index of the last element is "read + size - 1" and
  // c) "read <= index <= read + size".
  //
  // So to insert at index "index", one can either shift up the elements
  // "a[read + index],... a[read+size-1]" or
  // shift down the elements
  // "a[read] ... a[read + index-1]"
  // to make room for the new element.
  //
  // The choice made here is to minimize the number of shifts:
  // if "index < size / 2" then "a[read] ... a[read + index - 1]" is shifted down
  // and otherwise "a[read + index],... a[read+size-1]" is shifted up.
  if (index < SELF->size / 2) {
    // If a' ist the new queue and a is the old queue, then the operation
    // can be described as follows:
    // - a'[read-1] = a[read], ..., a'[read - 1 + index] = a[read + index]
    // - a'[read+index] = newElement
    // - read' = read - 1
    // - size' = size + 1
    // As read' = read - 1, one can also write
    // - a'[read'] = a[read' + 1], ..., a'[read + index] = a[read' + index + 1]
    // which is the form used here.
    dx_size offset = MOD(SELF->read - 1, capacity);
    for (dx_size j = 0; j < index; ++j) {
      SELF->elements[MOD(offset + j, capacity)] = SELF->elements[MOD(offset + j + 1, capacity)];
    }
    SELF->read = MOD(SELF->read - 1, capacity);
  } else {
    // If a' ist the new queue and a is the old queue, then the operation
    // can be described as follows:
    // - a'[read + size] = a[read + size - 1], ..., a'[read + index + 1] = a[read + index]
    // - a'[read + index] = newElement
    // - read' = read
    // - size' = size+1
    dx_size offset = SELF->read;
    for (dx_size j = SELF->size; j > index; --j) {
      SELF->elements[MOD(offset + j, capacity)] = SELF->elements[MOD(offset + j - 1, capacity)];
    }
  }
  SELF->elements[MOD(SELF->read + index, capacity)] = pointer;
  SELF->size++;
  if (SELF->added_callback) {
    SELF->added_callback(&pointer);
  }
  return DX_SUCCESS;
}

static dx_result _dx_impl_get_at(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_size index) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (index >= SELF->size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->elements[MOD(SELF->read + index, SELF->capacity)];
  return DX_SUCCESS;
}

static dx_result _dx_impl_remove(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_bool steal, dx_size index) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (index >= SELF->size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size const capacity = SELF->capacity;
  void* element = SELF->elements[MOD(SELF->read + index, capacity)];
  if (index < SELF->size / 2) {
    // - a'[read] = a[read-1], ..., a'[read + index] = a[read + index - 1] 
    // - read' = read + 1
    // - size' = size - 1
    dx_size offset = SELF->read;
    for (dx_size j = index; j > 0; --j) {
      SELF->elements[MOD(offset + j, capacity)] = SELF->elements[MOD(offset + j - 1, capacity)];
    }
    SELF->read = MOD(SELF->read + 1, capacity);
  } else {
    // - a'[read+index] = a[read+index+1], ..., a'[read+size-2] = a[read+size-1]
    // - read' = read
    // - size' = size - 1
    // shift a[i+1],..,a[n-1] left one position
    for (dx_size j = index; j < SELF->size - 1; ++j) {
      SELF->elements[MOD(SELF->read + j, capacity)] = SELF->elements[MOD(SELF->read + j + 1, capacity)];
    }
  }
  SELF->size--;
  if (!steal && SELF->removed_callback) {
    SELF->removed_callback(&element);
  }
  *RETURN = element;
  return DX_SUCCESS;
}

static dx_result _dx_impl_pop_front(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_bool steal) {
  if (SELF->size == 0) {
    dx_set_error(DX_ERROR_IS_EMPTY);
    return DX_FAILURE;
  }
  return _dx_impl_remove(RETURN, SELF, steal, 0);
}

static dx_result _dx_impl_pop_back(dx_inline_pointer_deque_element* RETURN, _dx_impl* SELF, dx_bool steal ) {
  if (SELF->size == 0) {
    dx_set_error(DX_ERROR_IS_EMPTY);
    return DX_FAILURE;
  }
  return _dx_impl_remove(RETURN, SELF, steal, SELF->size - 1);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

dx_result dx_inline_pointer_deque_initialize(dx_inline_pointer_deque* SELF, dx_size initial_capacity, DX_INLINE_POINTER_DEQUE_CONFIGURATION const* configuration) {
  _dx_impl* pimpl = NULL;
  if (dx_memory_allocate(&pimpl, sizeof(_dx_impl))) {
    return DX_FAILURE;
  }
  if (_dx_impl_initialize(pimpl, initial_capacity, configuration)) {
    dx_memory_deallocate(pimpl);
    pimpl = NULL;
    return DX_FAILURE;
  }
  SELF->pimpl = pimpl;
  return DX_SUCCESS;
}

void dx_inline_pointer_deque_uninitialize(dx_inline_pointer_deque* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  SELF->pimpl = NULL;
  _dx_impl_uninitialize(pimpl);
  dx_memory_deallocate(pimpl);
}

dx_result dx_inline_pointer_deque_increase_capacity(dx_inline_pointer_deque* SELF, dx_size required_additional_capacity) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_increase_capacity(pimpl, required_additional_capacity);
}

dx_result dx_inline_pointer_deque_ensure_free_capacity(dx_inline_pointer_deque* SELF, dx_size required_free_capacity) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_ensure_free_capacity(pimpl, required_free_capacity);
}

dx_result dx_inline_pointer_deque_get_size(dx_size* RETURN, dx_inline_pointer_deque const* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_get_size(RETURN, pimpl);
}

dx_result dx_inline_pointer_deque_get_capacity(dx_size* RETURN, dx_inline_pointer_deque const* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_get_capacity(RETURN, pimpl);
}

dx_result dx_inline_pointer_deque_get_free_capacity(dx_size* RETURN, dx_inline_pointer_deque const* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_get_free_capacity(RETURN, pimpl);
}

dx_result dx_inline_pointer_deque_clear(dx_inline_pointer_deque* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_clear(pimpl);
}

dx_result dx_inline_pointer_deque_push_back(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_push_back(pimpl, pointer);
}

dx_result dx_inline_pointer_deque_push_front(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_push_front(pimpl, pointer);
}

dx_result dx_inline_pointer_deque_insert(dx_inline_pointer_deque* SELF, dx_inline_pointer_deque_element pointer, dx_size index) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_insert(pimpl, pointer, index);
}

dx_result dx_inline_pointer_deque_get_at(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_size index) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_get_at(RETURN, pimpl, index);
}

dx_result dx_inline_pointer_deque_remove(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_bool steal, dx_size index) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_remove(RETURN, pimpl, steal, index);
}

dx_result dx_inline_pointer_deque_pop_front(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_bool steal) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_pop_front(RETURN, pimpl, steal);
}

dx_result dx_inline_pointer_deque_pop_back(dx_inline_pointer_deque_element* RETURN, dx_inline_pointer_deque* SELF, dx_bool steal) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_pop_back(RETURN, pimpl, steal);
}
