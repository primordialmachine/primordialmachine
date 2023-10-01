#include "dx/core/byte_array.h"

#include "dx/core/memory.h"

dx_result dx_inline_byte_array_initialize(dx_inline_byte_array* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->elements = NULL;
  if (dx_memory_allocate(&SELF->elements, 0)) {
    return DX_FAILURE;
  }
  SELF->size = 0;
  SELF->capacity = 0;
  return DX_SUCCESS;
}

void dx_inline_byte_array_uninitialize(dx_inline_byte_array* SELF) {
  dx_memory_deallocate(SELF->elements);
  SELF->elements = NULL;
}

dx_result dx_inline_byte_array_steal(dx_inline_byte_array* SELF, char **bytes, dx_size *number_of_bytes) {
  if (SELF->size < SELF->capacity) {
    char *new_elements = dx_memory_reallocate(SELF->elements, SELF->size);
    if (!new_elements) {
      return DX_FAILURE;
    }
    SELF->elements = new_elements;
    SELF->capacity = SELF->size;
  }
  char* new_elements = NULL;
  if (dx_memory_allocate(&new_elements, 0)) {
    return DX_FAILURE;
  }
  *bytes = SELF->elements;
  *number_of_bytes = SELF->size;
  SELF->size = 0;
  SELF->capacity = 0;
  SELF->elements = new_elements;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_increase_capacity(dx_inline_byte_array* SELF, dx_size additional) {
  dx_size old = SELF->capacity;
  //dx_size additional = required - available;
  if (SIZE_MAX - old < additional) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  dx_size new = old + additional;
  // optimize the allocations
  dx_size best_new;
  if (SIZE_MAX / 2 >= old) {
    // To understand the above test, simply consider uint8_t instead of dx_size as the types.
    // Then the expression "UINT8_MAX / 2"  is evaluated to floor(255 / 2) = floor(255/2) = floor(127.5) = 127.
    // If the old capacity is smaller than or equal to 127, it can be safely double as 127 * 2 = 254.
    best_new = old > 0 ? 2 * old : 8;
  } else {
    best_new = SIZE_MAX;
  }
  if (new < best_new) {
    new = best_new;
  }
  char* new_elements = dx_memory_reallocate(SELF->elements, new);
  if (!new_elements) {
    return DX_FAILURE;
  }
  SELF->elements = new_elements;
  SELF->capacity = new;
  return DX_SUCCESS; 
}

dx_result dx_inline_byte_array_ensure_free_capacity(dx_inline_byte_array* SELF, dx_size required) {
  dx_size available = SELF->capacity - SELF->size;
  if (available >= required) {
    return DX_SUCCESS;
  }
  dx_size additional = required - available;
  return dx_inline_byte_array_increase_capacity(SELF, additional);
}

dx_result dx_inline_byte_array_remove(dx_inline_byte_array* SELF, dx_size index, dx_size length) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  // if index 0 we remove 5 elements and the
  if (index + length > SELF->size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (length == 0) {
    return DX_SUCCESS;
  }
  // [a,b,c,d] 0,4 =>  index
  // l = 4 - 0 - 4 = 0
  // [a,b,c,d] 1,3
  // l = 4 - 3 - 1 = 0
  // [a,b,c,d] 1,2
  // l = 4 - 2 - 1 = 1
  dx_memory_copy(SELF->elements + index,
                 SELF->elements + index + length,
                 SELF->size - length - index);
  SELF->size -= length;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_set(dx_inline_byte_array* SELF, char const* bytes, dx_size number_of_bytes) {
  if (!SELF || !bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->size = 0;
  if (dx_inline_byte_array_ensure_free_capacity(SELF, number_of_bytes)) {
    return DX_FAILURE;
  }
  dx_memory_copy(SELF->elements, bytes, number_of_bytes);
  SELF->size = number_of_bytes;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_prepend(dx_inline_byte_array* SELF, char const* bytes, dx_size number_of_bytes) {
  return dx_inline_byte_array_insert(SELF, 0, bytes, number_of_bytes);
}

dx_result dx_inline_byte_array_append(dx_inline_byte_array* SELF, char const* bytes, dx_size number_of_bytes) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_byte_array_insert(SELF, SELF->size, bytes, number_of_bytes);
}
 
dx_result dx_inline_byte_array_insert(dx_inline_byte_array* SELF, dx_size index, char const* bytes, dx_size number_of_bytes) {
  if (!SELF || !bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (dx_inline_byte_array_ensure_free_capacity(SELF, number_of_bytes)) {
    return DX_FAILURE;
  }
  if (index < SELF->size) {
    dx_memory_move(SELF->elements + index + number_of_bytes, SELF->elements + index, SELF->size - index);
  }
  dx_memory_copy(SELF->elements + index, bytes, number_of_bytes);
  SELF->size += number_of_bytes;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_get_size(dx_size* RETURN, dx_inline_byte_array const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->size;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_get_bytes(void** RETURN, dx_inline_byte_array* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->elements;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_get_capacity(dx_size* RETURN, dx_inline_byte_array const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_get_free_capacity(dx_size* RETURN, dx_inline_byte_array const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity - SELF->size;
  return DX_SUCCESS;
}

dx_result dx_inline_byte_array_clear(dx_inline_byte_array* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->size = 0;
  return DX_SUCCESS;
}
