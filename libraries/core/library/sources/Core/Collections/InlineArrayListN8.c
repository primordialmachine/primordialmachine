#include "Core/Collections/InlineArrayListN8.h"

#include "Core/Memory.h"

Core_Result Core_InlineArrayListN8_initialize(Core_InlineArrayListN8* SELF, Core_Size initialCapacity, Core_InlineArrayListN8_Configuration const* configuration) {
  if (!SELF || !configuration) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->elements = NULL;
  if (Core_Memory_allocate((void**)&SELF->elements, 0)) {
    return Core_Failure;
  }
  SELF->size = 0;
  SELF->capacity = 0;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_steal(Core_InlineArrayListN8* SELF, char **bytes, Core_Size *number_of_bytes) {
  if (SELF->size < SELF->capacity) {
    if (Core_Memory_reallocate((void**)&SELF->elements, SELF->elements, SELF->size)) {
      return Core_Failure;
    }
    SELF->capacity = SELF->size;
  }
  char* new_elements = NULL;
  if (Core_Memory_allocate((void**)&new_elements, 0)) {
    return Core_Failure;
  }
  *bytes = SELF->elements;
  *number_of_bytes = SELF->size;
  SELF->size = 0;
  SELF->capacity = 0;
  SELF->elements = new_elements;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_set(Core_InlineArrayListN8* SELF, char const* bytes, Core_Size number_of_bytes) {
  if (!SELF || !bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->size = 0;
  if (Core_InlineArrayListN8_ensureFreeCapacity(SELF, number_of_bytes)) {
    return Core_Failure;
  }
  Core_Memory_copy(SELF->elements, bytes, number_of_bytes);
  SELF->size = number_of_bytes;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_getElements(void** RETURN, Core_InlineArrayListN8* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->elements;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_append(Core_InlineArrayListN8* SELF, Core_Natural8 element) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_insertMany(SELF, SELF->size, &element, 1);
}

Core_Result Core_InlineArrayListN8_appendMany(Core_InlineArrayListN8* SELF, Core_Natural8 const* elements, Core_Size numberOfElements) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_insertMany(SELF, SELF->size, elements, numberOfElements);
}

Core_Result Core_InlineArrayListN8_clear(Core_InlineArrayListN8* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->size = 0;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_ensureFreeCapacity(Core_InlineArrayListN8* SELF, Core_Size required) {
  Core_Size available = SELF->capacity - SELF->size;
  if (available >= required) {
    return Core_Success;
  }
  Core_Size additional = required - available;
  return Core_InlineArrayListN8_increaseCapacity(SELF, additional);
}

Core_Result Core_InlineArrayListN8_get(Core_Natural8* RETURN, Core_InlineArrayListN8* SELF, Core_Size index) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (index >= SELF->size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->elements[index];
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_getCapacity(Core_Size* RETURN, Core_InlineArrayListN8 const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayListN8 const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity - SELF->size;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_getSize(Core_Size* RETURN, Core_InlineArrayListN8 const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->size;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_increaseCapacity(Core_InlineArrayListN8* SELF, Core_Size additional) {
  Core_Size old = SELF->capacity;
  //Core_Size additional = required - available;
  if (SIZE_MAX - old < additional) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  Core_Size new = old + additional;
  // optimize the allocations
  Core_Size best_new;
  if (SIZE_MAX / 2 >= old) {
    // To understand the above test, simply consider uint8_t instead of Core_Size as the types.
    // Then the expression "UINT8_MAX / 2"  is evaluated to floor(255 / 2) = floor(255/2) = floor(127.5) = 127.
    // If the old capacity is smaller than or equal to 127, it can be safely double as 127 * 2 = 254.
    best_new = old > 0 ? 2 * old : 8;
  } else {
    best_new = SIZE_MAX;
  }
  if (new < best_new) {
    new = best_new;
  }
  if (Core_Memory_reallocate((void**)&SELF->elements, SELF->elements, new)) {
    return Core_Failure;
  }
  SELF->capacity = new;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_insert(Core_InlineArrayListN8* SELF, Core_Size index, Core_Natural8 element) {
  return Core_InlineArrayListN8_insertMany(SELF, index, &element, 1);
}

Core_Result Core_InlineArrayListN8_insertMany(Core_InlineArrayListN8* SELF, Core_Size index, Core_Natural8 const* elements, Core_Size numberOfElements) {
  if (!SELF || !elements) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_InlineArrayListN8_ensureFreeCapacity(SELF, numberOfElements)) {
    return Core_Failure;
  }
  if (index < SELF->size) {
    Core_Memory_move(SELF->elements + index + numberOfElements, SELF->elements + index, SELF->size - index);
  }
  Core_Memory_copy(SELF->elements + index, elements, numberOfElements);
  SELF->size += numberOfElements;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_prepend(Core_InlineArrayListN8* SELF, Core_Natural8 element) {
  return Core_InlineArrayListN8_insertMany(SELF, 0, &element, 1);
}

Core_Result Core_InlineArrayListN8_prependMany(Core_InlineArrayListN8* SELF, Core_Natural8 const* elements, Core_Size numberOfElements) {
  return Core_InlineArrayListN8_insertMany(SELF, 0, elements, numberOfElements);
}

Core_Result Core_InlineArrayListN8_removeMany(Core_InlineArrayListN8* SELF, Core_Size start, Core_Size length) {
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
  // [a,b,c,d] 0,4 =>  index
  // l = 4 - 0 - 4 = 0
  // [a,b,c,d] 1,3
  // l = 4 - 3 - 1 = 0
  // [a,b,c,d] 1,2
  // l = 4 - 2 - 1 = 1
  Core_Memory_copy(SELF->elements + start,
                   SELF->elements + start + length,
                   SELF->size - length - start);
  SELF->size -= length;
  return Core_Success;
}

Core_Result Core_InlineArrayListN8_removeManyBack(Core_InlineArrayListN8* SELF, Core_Size length) {
  return Core_InlineArrayListN8_removeMany(SELF, SELF->size - length, length);
}

Core_Result Core_InlineArrayListN8_removeManyFront(Core_InlineArrayListN8* SELF, Core_Size length) {
  return Core_InlineArrayListN8_removeMany(SELF, 0, length);
}

void Core_InlineArrayListN8_uninitialize(Core_InlineArrayListN8* SELF) {
  Core_Memory_deallocate(SELF->elements);
  SELF->elements = NULL;
}
