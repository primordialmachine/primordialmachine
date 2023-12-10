#include "dx/core/byte_array.h"

#include "Core/Memory.h"

Core_Result dx_inline_byte_array_initialize(dx_inline_byte_array* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->elements = NULL;
  if (Core_Memory_allocate(&SELF->elements, 0)) {
    return Core_Failure;
  }
  SELF->size = 0;
  SELF->capacity = 0;
  return Core_Success;
}

void dx_inline_byte_array_uninitialize(dx_inline_byte_array* SELF) {
  Core_Memory_deallocate(SELF->elements);
  SELF->elements = NULL;
}

Core_Result dx_inline_byte_array_steal(dx_inline_byte_array* SELF, char **bytes, Core_Size *number_of_bytes) {
  if (SELF->size < SELF->capacity) {
    if (Core_Memory_reallocate(&SELF->elements, SELF->elements, SELF->size)) {
      return Core_Failure;
    }
    SELF->capacity = SELF->size;
  }
  char* new_elements = NULL;
  if (Core_Memory_allocate(&new_elements, 0)) {
    return Core_Failure;
  }
  *bytes = SELF->elements;
  *number_of_bytes = SELF->size;
  SELF->size = 0;
  SELF->capacity = 0;
  SELF->elements = new_elements;
  return Core_Success;
}

Core_Result dx_inline_byte_array_increase_capacity(dx_inline_byte_array* SELF, Core_Size additional) {
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
  if (Core_Memory_reallocate(&SELF->elements, SELF->elements, new)) {
    return Core_Failure;
  }
  SELF->capacity = new;
  return Core_Success; 
}

Core_Result dx_inline_byte_array_ensure_free_capacity(dx_inline_byte_array* SELF, Core_Size required) {
  Core_Size available = SELF->capacity - SELF->size;
  if (available >= required) {
    return Core_Success;
  }
  Core_Size additional = required - available;
  return dx_inline_byte_array_increase_capacity(SELF, additional);
}

Core_Result dx_inline_byte_array_remove(dx_inline_byte_array* SELF, Core_Size index, Core_Size length) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // if index 0 we remove 5 elements and the
  if (index + length > SELF->size) {
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
  Core_Memory_copy(SELF->elements + index,
                   SELF->elements + index + length,
                   SELF->size - length - index);
  SELF->size -= length;
  return Core_Success;
}

Core_Result dx_inline_byte_array_set(dx_inline_byte_array* SELF, char const* bytes, Core_Size number_of_bytes) {
  if (!SELF || !bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->size = 0;
  if (dx_inline_byte_array_ensure_free_capacity(SELF, number_of_bytes)) {
    return Core_Failure;
  }
  Core_Memory_copy(SELF->elements, bytes, number_of_bytes);
  SELF->size = number_of_bytes;
  return Core_Success;
}

Core_Result dx_inline_byte_array_prepend(dx_inline_byte_array* SELF, char const* bytes, Core_Size number_of_bytes) {
  return dx_inline_byte_array_insert(SELF, 0, bytes, number_of_bytes);
}

Core_Result dx_inline_byte_array_append(dx_inline_byte_array* SELF, char const* bytes, Core_Size number_of_bytes) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_inline_byte_array_insert(SELF, SELF->size, bytes, number_of_bytes);
}
 
Core_Result dx_inline_byte_array_insert(dx_inline_byte_array* SELF, Core_Size index, char const* bytes, Core_Size number_of_bytes) {
  if (!SELF || !bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (dx_inline_byte_array_ensure_free_capacity(SELF, number_of_bytes)) {
    return Core_Failure;
  }
  if (index < SELF->size) {
    Core_Memory_move(SELF->elements + index + number_of_bytes, SELF->elements + index, SELF->size - index);
  }
  Core_Memory_copy(SELF->elements + index, bytes, number_of_bytes);
  SELF->size += number_of_bytes;
  return Core_Success;
}

Core_Result dx_inline_byte_array_get_size(Core_Size* RETURN, dx_inline_byte_array const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->size;
  return Core_Success;
}

Core_Result dx_inline_byte_array_get_bytes(void** RETURN, dx_inline_byte_array* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->elements;
  return Core_Success;
}

Core_Result dx_inline_byte_array_get_capacity(Core_Size* RETURN, dx_inline_byte_array const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity;
  return Core_Success;
}

Core_Result dx_inline_byte_array_get_free_capacity(Core_Size* RETURN, dx_inline_byte_array const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity - SELF->size;
  return Core_Success;
}

Core_Result dx_inline_byte_array_clear(dx_inline_byte_array* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->size = 0;
  return Core_Success;
}
