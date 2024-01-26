// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "Core/Collections/InlineArrayListP.h"

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

  /// @brief A pointer to the @a Core_InlineArrayListP_AddedCallback function or a null pointer.
  Core_InlineArrayListP_AddedCallback* addedCallback;
  /// @brief A pointer to the @a Core_InlineArrayListP_RemovedCallback function or  a null pointer.
  Core_InlineArrayListP_RemovedCallback* removedCallback;

} _dx_impl; // struct _dx_impl

/// @internal
/// @brief Initialize this _dx_impl object object.
/// @param SELF A pointer to this uninitalized _dx_impl object.
/// @param initial_capacity The initial capacity. Can be @a 0.
/// @param configuration A pointer to the configuration.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a initial_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_Size initial_capacity, Core_InlineArrayListP_Configuration const* configuration);

/// @internal
/// @brief Get the pointer at the specified index.
/// @param RETURN A pointer to a <code>Core_InlineArrayListP_Element</code> variable.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @success <code>*RETURN</code> was assigned the pointer at the specified index.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the size of the array.
static Core_Result _dx_impl_get_at(Core_InlineArrayListP_Element* RETURN, _dx_impl const* SELF, Core_Size index);

/// @internal
/// @brief Append an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param element The element.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_append(_dx_impl* SELF, Core_InlineArrayListP_Element element);

/// @internal
/// @brief Append an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param elements The elements.
/// @param numberOfElements The number of elements.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_appendMany(_dx_impl* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements);

/// @internal
/// @brief Remove all elements.
/// @param SELF A pointer to this _dx_impl object.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static Core_Result _dx_impl_clear(_dx_impl* SELF);

/// @internal
/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this _dx_impl object.
/// @param required_free_capacity The required free capacity.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed @a required_free_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_ensure_free_capacity(_dx_impl* SELF, Core_Size required_free_capacity);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @method{_dx_impl}
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static inline Core_Result _dx_impl_get_capacity(Core_Size* RETURN, _dx_impl const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @method{_dx_impl}
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static inline Core_Result _dx_impl_get_free_capacity(Core_Size* RETURN, _dx_impl const* SELF);

/// @internal
/// @brief Get the size, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this _dx_impl object.
/// @method{_dx_impl}
/// @success <code>*RETURN</code> was assigned the capacity.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
static inline Core_Result _dx_impl_get_size(Core_Size* RETURN, _dx_impl const* self);

/// @internal
/// @brief Increase the capacity.
/// @param self A pointer to this _dx_impl object.
/// @param additional_capacity The amount to increase the capacity by.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
static Core_Result _dx_impl_increase_capacity(_dx_impl* SELF, Core_Size additional_capacity);

/// @internal
/// @brief Insert an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @param element The element.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of the array
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_insert(_dx_impl* SELF, Core_Size index, Core_InlineArrayListP_Element element);

/// @internal
/// @brief Insert an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param index The index.
/// @param elements The elements.
/// @param numberOfElements
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of the array
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_insertMany(_dx_impl* SELF, Core_Size index, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements);

/// @internal
/// @brief Prepend an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param element The element.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_prepend(_dx_impl* SELF, Core_InlineArrayListP_Element element);

/// @internal
/// @brief Prepend an element.
/// @param SELF A pointer to this _dx_impl object.
/// @param elements The elements.
/// @param numberOfElements The number of elements.
/// @method{_dx_impl}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
static Core_Result _dx_impl_prependMany(_dx_impl* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements);

static Core_Result _dx_impl_removeMany(_dx_impl* SELF, Core_Size start, Core_Size length);

static Core_Result _dx_impl_removeManyBack(_dx_impl* SELF, Core_Size length);

static Core_Result _dx_impl_removeManyFront(_dx_impl* SELF, Core_Size length);

/// @internal
/// @brief Uninitialize this _dx_impl object.
/// @param SELF A pointer to this initialized _dx_impl object.
static void _dx_impl_uninitialize(_dx_impl* SELF);

static Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_Size initial_capacity, Core_InlineArrayListP_Configuration const* configuration) {
  if (!SELF || !configuration) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size overflow = 0;
  Core_Size initialCapacityBytes;
  Core_safeMulSz(&initialCapacityBytes, initial_capacity, sizeof(Core_InlineArrayListP_Element), &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  void** elements = NULL;
  if (Core_Memory_allocate((void**)&elements, initialCapacityBytes)) {
    return Core_Failure;
  }
  SELF->size = 0;
  SELF->elements = elements;
  SELF->capacity = initial_capacity;
  SELF->addedCallback = configuration->addedCallback;
  SELF->removedCallback = configuration->removedCallback;
  return Core_Success;
}

static Core_Result _dx_impl_get_at(Core_InlineArrayListP_Element* RETURN, _dx_impl const* SELF, Core_Size index) {
  if (!SELF || index >= SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->elements[index];
  return Core_Success;
}

static Core_Result _dx_impl_append(_dx_impl* SELF, Core_InlineArrayListP_Element pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insert(SELF, SELF->size, pointer);
}

static Core_Result _dx_impl_appendMany(_dx_impl* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insertMany(SELF, SELF->size, elements, numberOfElements);
}

static Core_Result _dx_impl_clear(_dx_impl* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->removedCallback) {
    Core_InlineArrayListP_RemovedCallback* removed_callback = SELF->removedCallback;
    while (SELF->size > 0) {
      void* element = SELF->elements[--SELF->size];
      removed_callback(&element);
    }
  } else {
    SELF->size = 0;
  }
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

static inline Core_Result _dx_impl_get_size(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->size;
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
  Core_safeMulSz(&new_capacity_bytes, new_capacity, sizeof(Core_InlineArrayListP_Element), &overflow); // must succeed
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

static Core_Result _dx_impl_insert(_dx_impl* SELF, Core_Size index, Core_InlineArrayListP_Element pointer) {
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
  if (SELF->addedCallback) {
    SELF->addedCallback(&pointer);
  }
  if (index != SELF->size) {
    Core_Memory_move(SELF->elements + index + 1,
                     SELF->elements + index + 0,
                     (SELF->size - index) * sizeof(Core_InlineArrayListP_Element));
  }
  SELF->elements[index] = pointer;
  SELF->size++;
  return Core_Success;
}

static Core_Result _dx_impl_insertMany(_dx_impl* SELF, Core_Size index, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (index > SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_dx_impl_ensure_free_capacity(SELF, numberOfElements)) {
    return Core_Failure;
  }
  if (SELF->addedCallback) {
    for (Core_Size i = 0, n = numberOfElements; i < n; ++i) {
      SELF->addedCallback((Core_InlineArrayListP**)&elements[i]);
    }
  }
  if (index != SELF->size) {
    Core_Memory_move(SELF->elements + index + numberOfElements,
                     SELF->elements + index + 0,
                     (SELF->size - index) * sizeof(Core_InlineArrayListP_Element));
  }
  Core_Memory_copy(SELF->elements + index, elements, numberOfElements * sizeof(Core_InlineArrayListP_Element));
  SELF->size += numberOfElements;
  return Core_Success;
}

static Core_Result _dx_impl_prepend(_dx_impl* SELF, Core_InlineArrayListP_Element pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insert(SELF, 0, pointer);
}

static Core_Result _dx_impl_prependMany(_dx_impl* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_insertMany(SELF, 0, elements, numberOfElements);
}

static Core_Result _dx_impl_removeMany(_dx_impl* SELF, Core_Size start, Core_Size length) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (start + length > SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (length == 0) {
    return Core_Success;
  }
  if (SELF->removedCallback) {
    for (Core_Size i = start, n = start + length; i < n; ++i) {
      SELF->removedCallback(&SELF->elements[i]);
    }
  }
  // [a,b,c,d] 0,4 =>  index
  // l = 4 - 0 - 4 = 0
  // [a,b,c,d] 1,3
  // l = 4 - 3 - 1 = 0
  // [a,b,c,d] 1,2
  // l = 4 - 2 - 1 = 1
  Core_Memory_copy(SELF->elements + start,
                   SELF->elements + start + length,
                   sizeof(Core_InlineArrayListP_Element) * (SELF->size - length - start));
  SELF->size -= length;
  return Core_Success;
}

static Core_Result _dx_impl_removeManyBack(_dx_impl* SELF, Core_Size length) {
  return _dx_impl_removeMany(SELF, SELF->size - length, length);
}

static Core_Result _dx_impl_removeManyFront(_dx_impl* SELF, Core_Size length) {
  return _dx_impl_removeMany(SELF, 0, length);
}

static void _dx_impl_uninitialize(_dx_impl* SELF) {
  _dx_impl_clear(SELF);
  Core_Memory_deallocate(SELF->elements);
  SELF->elements = NULL;
  SELF->capacity = 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

static inline _dx_impl const* _DX_IMPL_CONST(void* p) {
  return (_dx_impl const*)p;
}

Core_Result Core_InlineArrayListP_initialize(Core_InlineArrayListP*SELF, Core_Size initialCapacity, Core_InlineArrayListP_Configuration const* configuration) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_Memory_allocate(&SELF->pimpl, sizeof(_dx_impl))) {
    return Core_Failure;
  }
  if (_dx_impl_initialize(_DX_IMPL(SELF->pimpl), initialCapacity, configuration)) {
    Core_Memory_deallocate(SELF->pimpl);
    SELF->pimpl = NULL;
    return Core_Failure;
  }
 return Core_Success;
}

Core_Result Core_InlineArrayListP_append(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element pointer) {
  return _dx_impl_append(_DX_IMPL(SELF->pimpl), pointer);
}

Core_Result Core_InlineArrayListP_appendMany(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements) {
  return _dx_impl_appendMany(_DX_IMPL(SELF->pimpl), elements, numberOfElements);
}

Core_Result Core_InlineArrayListP_clear(Core_InlineArrayListP* SELF) {
  return _dx_impl_clear(_DX_IMPL(SELF->pimpl));
}

Core_Result Core_InlineArrayListP_ensureFreeCapacity(Core_InlineArrayListP* SELF, Core_Size requiredFreeCapacity) {
  return _dx_impl_ensure_free_capacity(_DX_IMPL(SELF->pimpl), requiredFreeCapacity);
}

Core_Result Core_InlineArrayListP_get(Core_InlineArrayListP_Element* RETURN, Core_InlineArrayListP const* SELF, Core_Size index) {
  return _dx_impl_get_at(RETURN, _DX_IMPL_CONST(SELF->pimpl), index);
}

Core_Result Core_InlineArrayListP_getCapacity(Core_Size* RETURN, Core_InlineArrayListP const* SELF) {
  return _dx_impl_get_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result Core_InlineArrayListP_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayListP const* SELF) {
  return _dx_impl_get_free_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result Core_InlineArrayListP_getSize(Core_Size* RETURN, Core_InlineArrayListP const* SELF) {
  return _dx_impl_get_size(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result Core_InlineArrayListP_increaseCapacity(Core_InlineArrayListP* SELF, Core_Size additionalCapacity) {
  return _dx_impl_increase_capacity(_DX_IMPL(SELF->pimpl), additionalCapacity);
}

Core_Result Core_InlineArrayListP_insert(Core_InlineArrayListP* SELF, Core_Size index, Core_InlineArrayListP_Element pointer) {
  return _dx_impl_insert(_DX_IMPL(SELF->pimpl), index, pointer);
}

Core_Result Core_InlineArrayListP_insertMany(Core_InlineArrayListP* SELF, Core_Size index, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements) {
  return _dx_impl_insertMany(_DX_IMPL(SELF->pimpl), index, elements, numberOfElements);
}

Core_Result Core_InlineArrayListP_prepend(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element pointer) {
  return _dx_impl_prepend(_DX_IMPL(SELF->pimpl), pointer);
}

Core_Result Core_InlineArrayListP_prependMany(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements) {
  return _dx_impl_prependMany(_DX_IMPL(SELF->pimpl), elements, numberOfElements);
}

Core_Result Core_InlineArrayListP_removeMany(Core_InlineArrayListP* SELF, Core_Size start, Core_Size length) {
  return _dx_impl_removeMany(_DX_IMPL(SELF->pimpl), start, length);
}

Core_Result Core_InlineArrayListP_removeManyBack(Core_InlineArrayListP* SELF, Core_Size n) {
  return _dx_impl_removeManyBack(_DX_IMPL(SELF->pimpl), n);
}

Core_Result Core_InlineArrayListP_removeManyFront(Core_InlineArrayListP* SELF, Core_Size n) {
  return _dx_impl_removeManyFront(_DX_IMPL(SELF->pimpl), n);
}

void Core_InlineArrayListP_uninitialize(Core_InlineArrayListP* SELF) {
  _dx_impl_uninitialize(_DX_IMPL(SELF->pimpl));
  Core_Memory_deallocate(SELF->pimpl);
  SELF->pimpl = NULL;
}

