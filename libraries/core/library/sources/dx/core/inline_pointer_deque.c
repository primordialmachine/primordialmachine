#include "dx/core/inline_pointer_deque.h"

#include "Core/Memory.h"
#include "Core/safeAddNx.h"
#include "Core/safeMulNx.h"
#include "Core/NextPowerOfTwo.h"
#include "dx/core/_get_best_array_size.h"

/// @brief The greatest capacity, in elements, of a pointer array.
static Core_Size const GREATEST_CAPACITY = Core_Size_Greatest / sizeof(void*);

/// @brief The least capacity, in elements, of a pointer array.
static Core_Size const LEAST_CAPACITY = 0;

// Macro-like function to compute index % capacity.
static inline Core_Size MOD(Core_Size index, Core_Size capacity);

static inline Core_Size MOD(Core_Size index, Core_Size capacity) {
  return index % capacity;
}

typedef struct _dx_impl {
  
  /// @brief A pointer to an array of @a capacity @a (void *) elements.
  void** elements;
  /// @brief The capacity, in elements, of the array pointed to by @a array.
  Core_Size capacity;
  /// @brief The number of elements in this array.
  Core_Size size;
  
  /// @brief The position to read the next element from.
  Core_Size read;

  /// @brief A pointer to the @a dx_added_callback function or a null pointer.
  Core_InlineArrayDequeP_added_callback* added_callback;
  /// @brief A pointer to the @a dx_removed_callback function or  a null pointer.
  Core_InlineArrayDequeP_removed_callback* removed_callback;

} _dx_impl; // struct _dx_impl

/// @brief Initialize this _dx_impl object.
/// @param SELF A pointer to this _dx_impl object.
/// @param initial_capacity The initial capacity.
/// @param configuration A pointer to the configuration.
/// @default-return
/// @default-failure
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_Size initial_capacity, Core_InlineArrayDequeP_Configuration const* configuration);

/// @brief Uninitialize this _dx_impl object.
/// @param SELF A pointer to this _dx_impl object.
static void _dx_impl_uninitialize(_dx_impl* SELF);

/// @brief Get the size, in elements.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static Core_Result _dx_impl_getSize(Core_Size* RETURN, _dx_impl const* SELF);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static Core_Result _dx_impl_getCapacity(Core_Size* RETURN, _dx_impl const* self);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static Core_Result _dx_impl_getFreeCapacity(Core_Size* RETURN, _dx_impl const* SELF);

/// @brief Increase the capacity.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_additional_capacity The amount to increase the capacity by.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
static Core_Result _dx_impl_increaseCapacity(_dx_impl* SELF, Core_Size required_additional_capacity);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_free_capacity The required free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a required_free_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The free capacity is greater than or equal to the specified required free capacity.
static Core_Result _dx_impl_ensureFreeCapacity(_dx_impl* SELF, Core_Size required_free_capacity);

/// @brief Remove all elements.
/// @param SELF A pointer to this _dx_impl object.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static Core_Result _dx_impl_clear(_dx_impl* SELF);

/// @brief Append an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_push_back(_dx_impl* SELF, Core_InlineArrayDequeP_element pointer);

/// @brief Prepend an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_push_front(_dx_impl* SELF, Core_InlineArrayDequeP_element pointer);

/// @brief Insert an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param pointer The element.
/// @param index The index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this _dx_impl object
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_insert(_dx_impl* SELF, Core_InlineArrayDequeP_element pointer, Core_Size index);

/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a Core_InlineArrayDequeP_element variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this _dx_impl object
static Core_Result _dx_impl_get_at(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Size index);

/// @brief Remove the pointer at the specified index.
/// @param RETURN A pointer to a Core_InlineArrayDequeP_element variable.
/// @param SELF A pointer to the _dx_impl object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @param index The index.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of this _dx_impl object
static Core_Result _dx_impl_remove(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Boolean steal, Core_Size index);

/// @brief Pop the first pointer of the deque.
/// @param RETURN A pointer to a <code>Core_InlineArrayDequeP_element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the first pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid the deque is empty.
static Core_Result _dx_impl_pop_front(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Boolean steal);

/// @brief Pop the last pointer of the deque.
/// @param RETURN A pointer to a <code>Core_InlineArrayDequeP_element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param steal The "removed" callback is not invoked if this is @a true.
/// @success <code>*RETURN</code> was assigned the last pointer of the deque.
/// @warning If the object was destroyed due to a call to the removed callback, then the returned pointer is invalid.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid the deque is empty
static Core_Result _dx_impl_pop_back(Core_InlineArrayDequeP_element * RETURN, _dx_impl * SELF, Core_Boolean steal);

static Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_Size initial_capacity, Core_InlineArrayDequeP_Configuration const* configuration) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size overflow;
  Core_Size initial_capacity_bytes;
  Core_safeMulSz(&initial_capacity_bytes, initial_capacity, sizeof(Core_InlineArrayDequeP_element), &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  void** elements = NULL;
  if (Core_Memory_allocate((void**)&elements, initial_capacity_bytes)) {
    return Core_Failure;
  }
  SELF->read = 0;
  SELF->size = 0;
  SELF->elements = elements;
  SELF->capacity = initial_capacity;
  SELF->added_callback = configuration->added_callback;
  SELF->removed_callback = configuration->removed_callback;
  return Core_Success;
}

static void _dx_impl_uninitialize(_dx_impl* self) {
  _dx_impl_clear(self);
  Core_Memory_deallocate(self->elements);
  self->elements = NULL;
  self->capacity = 0; 
}

static Core_Result _dx_impl_getSize(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->size;
  return Core_Success;
}

static Core_Result _dx_impl_getCapacity(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity;
  return Core_Success;
}

static Core_Result _dx_impl_getFreeCapacity(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity - SELF->size;
  return Core_Success;
}

static Core_Result _dx_impl_increaseCapacity(_dx_impl* SELF, Core_Size required_additional_capacity) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!required_additional_capacity) {
    return Core_Success;
  }
  Core_Size new_capacity = 0;
  if (dx_get_best_array_size(&new_capacity, SELF->capacity, required_additional_capacity, LEAST_CAPACITY, GREATEST_CAPACITY, Core_True)) {
    return Core_Failure;
  }
  void** new_elements = NULL;
  if (Core_Memory_allocate((void**)&new_elements, new_capacity * sizeof(void*))) {
    return Core_Failure;
  }
  for (Core_Size i = 0, n = SELF->size; i < n; ++i) {
    new_elements[i] = SELF->elements[MOD(SELF->read + i, SELF->capacity)];
  }
  Core_Memory_deallocate(SELF->elements);
  SELF->elements = new_elements;
  SELF->capacity = new_capacity;
  SELF->read = 0;
  return Core_Success;
}

static Core_Result _dx_impl_ensureFreeCapacity(_dx_impl* SELF, Core_Size required_free_capacity) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size available_free_capacity = SELF->capacity - SELF->size;
  if (available_free_capacity > required_free_capacity) {
    return Core_Success;
  }
  Core_Size additional_capacity = required_free_capacity - available_free_capacity;
  return _dx_impl_increaseCapacity(SELF, additional_capacity);
}

static Core_Result _dx_impl_clear(_dx_impl* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
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
  return Core_Success;
}

static Core_Result _dx_impl_push_back(_dx_impl* SELF, Core_InlineArrayDequeP_element pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insert(SELF, pointer, SELF->size);
}

static Core_Result _dx_impl_push_front(_dx_impl* SELF, Core_InlineArrayDequeP_element pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insert(SELF, pointer, 0);
}

static Core_Result _dx_impl_insert(_dx_impl* SELF, Core_InlineArrayDequeP_element pointer, Core_Size index) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (index > SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_dx_impl_ensureFreeCapacity(SELF, 1)) {
    return Core_Failure;
  }
  Core_Size const capacity = SELF->capacity; // The capacity does not change anymore for the rest of this
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
    Core_Size offset = MOD(SELF->read - 1, capacity);
    for (Core_Size j = 0; j < index; ++j) {
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
    Core_Size offset = SELF->read;
    for (Core_Size j = SELF->size; j > index; --j) {
      SELF->elements[MOD(offset + j, capacity)] = SELF->elements[MOD(offset + j - 1, capacity)];
    }
  }
  SELF->elements[MOD(SELF->read + index, capacity)] = pointer;
  SELF->size++;
  if (SELF->added_callback) {
    SELF->added_callback(&pointer);
  }
  return Core_Success;
}

static Core_Result _dx_impl_get_at(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Size index) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (index >= SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->elements[MOD(SELF->read + index, SELF->capacity)];
  return Core_Success;
}

static Core_Result _dx_impl_remove(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Boolean steal, Core_Size index) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (index >= SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size const capacity = SELF->capacity;
  void* element = SELF->elements[MOD(SELF->read + index, capacity)];
  if (index < SELF->size / 2) {
    // - a'[read] = a[read-1], ..., a'[read + index] = a[read + index - 1] 
    // - read' = read + 1
    // - size' = size - 1
    Core_Size offset = SELF->read;
    for (Core_Size j = index; j > 0; --j) {
      SELF->elements[MOD(offset + j, capacity)] = SELF->elements[MOD(offset + j - 1, capacity)];
    }
    SELF->read = MOD(SELF->read + 1, capacity);
  } else {
    // - a'[read+index] = a[read+index+1], ..., a'[read+size-2] = a[read+size-1]
    // - read' = read
    // - size' = size - 1
    // shift a[i+1],..,a[n-1] left one position
    for (Core_Size j = index; j < SELF->size - 1; ++j) {
      SELF->elements[MOD(SELF->read + j, capacity)] = SELF->elements[MOD(SELF->read + j + 1, capacity)];
    }
  }
  SELF->size--;
  if (!steal && SELF->removed_callback) {
    SELF->removed_callback(&element);
  }
  *RETURN = element;
  return Core_Success;
}

static Core_Result _dx_impl_pop_front(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Boolean steal) {
  if (SELF->size == 0) {
    Core_setError(Core_Error_Empty);
    return Core_Failure;
  }
  return _dx_impl_remove(RETURN, SELF, steal, 0);
}

static Core_Result _dx_impl_pop_back(Core_InlineArrayDequeP_element* RETURN, _dx_impl* SELF, Core_Boolean steal ) {
  if (SELF->size == 0) {
    Core_setError(Core_Error_Empty);
    return Core_Failure;
  }
  return _dx_impl_remove(RETURN, SELF, steal, SELF->size - 1);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

Core_Result Core_InlineArrayDequeP_initialize(Core_InlineArrayDequeP* SELF, Core_Size initial_capacity, Core_InlineArrayDequeP_Configuration const* configuration) {
  _dx_impl* pimpl = NULL;
  if (Core_Memory_allocate(&pimpl, sizeof(_dx_impl))) {
    return Core_Failure;
  }
  if (_dx_impl_initialize(pimpl, initial_capacity, configuration)) {
    Core_Memory_deallocate(pimpl);
    pimpl = NULL;
    return Core_Failure;
  }
  SELF->pimpl = pimpl;
  return Core_Success;
}

void Core_InlineArrayDequeP_uninitialize(Core_InlineArrayDequeP* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  SELF->pimpl = NULL;
  _dx_impl_uninitialize(pimpl);
  Core_Memory_deallocate(pimpl);
}

Core_Result Core_InlineArrayDequeP_increaseCapacity(Core_InlineArrayDequeP* SELF, Core_Size requiredAdditionalCapacity) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_increaseCapacity(pimpl, requiredAdditionalCapacity);
}

Core_Result Core_InlineArrayDequeP_ensureFreeCapacity(Core_InlineArrayDequeP* SELF, Core_Size requiredFreeCapacity) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_ensureFreeCapacity(pimpl, requiredFreeCapacity);
}

Core_Result Core_InlineArrayDequeP_getSize(Core_Size* RETURN, Core_InlineArrayDequeP const* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_getSize(RETURN, pimpl);
}

Core_Result Core_InlineArrayDequeP_getCapacity(Core_Size* RETURN, Core_InlineArrayDequeP const* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_getCapacity(RETURN, pimpl);
}

Core_Result Core_InlineArrayDequeP_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayDequeP const* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_getFreeCapacity(RETURN, pimpl);
}

Core_Result Core_InlineArrayDequeP_clear(Core_InlineArrayDequeP* SELF) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_clear(pimpl);
}

Core_Result Core_InlineArrayDequeP_push_back(Core_InlineArrayDequeP* SELF, Core_InlineArrayDequeP_element pointer) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_push_back(pimpl, pointer);
}

Core_Result Core_InlineArrayDequeP_push_front(Core_InlineArrayDequeP* SELF, Core_InlineArrayDequeP_element pointer) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_push_front(pimpl, pointer);
}

Core_Result Core_InlineArrayDequeP_insert(Core_InlineArrayDequeP* SELF, Core_InlineArrayDequeP_element pointer, Core_Size index) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_insert(pimpl, pointer, index);
}

Core_Result Core_InlineArrayDequeP_get_at(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Size index) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_get_at(RETURN, pimpl, index);
}

Core_Result Core_InlineArrayDequeP_remove(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Boolean steal, Core_Size index) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_remove(RETURN, pimpl, steal, index);
}

Core_Result Core_InlineArrayDequeP_pop_front(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Boolean steal) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_pop_front(RETURN, pimpl, steal);
}

Core_Result Core_InlineArrayDequeP_pop_back(Core_InlineArrayDequeP_element* RETURN, Core_InlineArrayDequeP* SELF, Core_Boolean steal) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  return _dx_impl_pop_back(RETURN, pimpl, steal);
}
