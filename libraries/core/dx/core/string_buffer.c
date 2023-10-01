#include "dx/core/string_buffer.h"

#include "dx/core/string.h"
#include "dx/core/_is_utf8_sequence.h"
#include "dx/core/_string_format.h"

DX_DEFINE_OBJECT_TYPE("dx.string_buffer",
                      dx_string_buffer,
                      dx_object);

static void dx_string_buffer_destruct(dx_string_buffer* SELF) {
  dx_inline_byte_array_uninitialize(&SELF->backend);
}

static void dx_string_buffer_dispatch_construct(dx_string_buffer_dispatch* SELF)
{/*Intentionally emtpy.*/}

dx_result dx_string_buffer_construct(dx_string_buffer* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_string_buffer_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_inline_byte_array_initialize(&SELF->backend)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_string_buffer_create(dx_string_buffer** RETURN) {
  DX_CREATE_PREFIX(dx_string_buffer)
  if (dx_string_buffer_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_string_buffer_clone(dx_string_buffer** RETURN, dx_string_buffer const* SELF) {
  dx_string_buffer* clone = NULL;
  if (dx_string_buffer_create(&clone)) {
    return DX_FAILURE;
  }
  if (dx_string_buffer_append_bytes(clone, SELF->backend.elements, SELF->backend.size)) {
    DX_UNREFERENCE(clone);
    clone = NULL;
    return DX_FAILURE;
  }
  *RETURN = clone;
  return DX_SUCCESS;
}

dx_result dx_string_buffer_get_bytes(void** RETURN, dx_string_buffer* SELF) {
  return dx_inline_byte_array_get_bytes(RETURN, &SELF->backend);
}

dx_result dx_string_buffer_get_number_of_bytes(dx_size* RETURN, dx_string_buffer* SELF) {
  return dx_inline_byte_array_get_size(RETURN, &SELF->backend);
}

dx_result dx_string_buffer_to_string(dx_string** RETURN, dx_string_buffer* SELF) {
  dx_size size;
  if (dx_inline_byte_array_get_size(&size, &SELF->backend)) {
    return DX_FAILURE;
  }
  void* bytes;
  if (dx_inline_byte_array_get_bytes(&bytes, &SELF->backend)) {
    return DX_FAILURE;
  }
  // @todo Add a constructor dx_string_from_byte_buffer in order to avoid the UTF-8 validity recheck that dx_string_create(char const*, dx_size) performs.
  return dx_string_create(RETURN, bytes, size);
}

dx_result dx_string_buffer_set_string(dx_string_buffer* SELF, dx_string* string) {
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, string)) {
    return DX_FAILURE;
  }
  void const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, string)) {
    return DX_FAILURE;
  }
  return dx_inline_byte_array_set(&SELF->backend, bytes, number_of_bytes);
}

dx_result dx_string_buffer_append_string(dx_string_buffer* SELF, dx_string* string) {
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, string)) {
    return DX_FAILURE;
  }
  void const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, string)) {
    return DX_FAILURE;
  }
  return dx_inline_byte_array_append(&SELF->backend, bytes, number_of_bytes);
}

dx_result dx_string_buffer_set_bytes(dx_string_buffer* SELF, char const* p, dx_size n) {
  if (!dx__is_utf8_sequence(p, n)) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_byte_array_set(&SELF->backend, p, n);
}

dx_result dx_string_buffer_append_bytes(dx_string_buffer* SELF, char const* p, dx_size n) {
  if (!dx__is_utf8_sequence(p, n)) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_byte_array_append(&SELF->backend, p, n);
}

dx_result dx_string_buffer_prepend_bytes(dx_string_buffer* SELF, char const* p, dx_size n) {
  if (!dx__is_utf8_sequence(p, n)) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_byte_array_prepend(&SELF->backend, p, n);
}

dx_result dx_string_buffer_append_fv(dx_string_buffer* SELF, dx_string* format, va_list arguments) {
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, format)) {
    return DX_FAILURE;
  }
  void const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, format)) {
    return DX_FAILURE;
  }
  return dx__format_v(&SELF->backend, bytes, ((char const*)bytes) + number_of_bytes, arguments);
}

dx_result dx_string_buffer_append_f(dx_string_buffer* SELF, dx_string* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  dx_result result = dx_string_buffer_append_fv(SELF, format, arguments);
  va_end(arguments);
  return result;
}

dx_result dx_string_buffer_remove_last(dx_string_buffer* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size size;
  if (dx_inline_byte_array_get_size(&size, &SELF->backend)) {
    return DX_FAILURE;
  }
  if (!size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  void* bytes;
  if (dx_inline_byte_array_get_bytes(&bytes, &SELF->backend)) {
    return DX_FAILURE;
  }
  char const* start = (char const*)bytes;
  char const* end = start + size;
  char const* current = end - 1;
  dx_size received = 1;
  while (((*current) & 0xC0) == 0x80) {
    if (current == start) {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    }
    current--;
    received++;
  }
  dx_size expected = _utf8_classify(*current);
  if (!expected) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_byte_array_remove(&SELF->backend, size - received, received);
}

dx_result dx_string_buffer_clear(dx_string_buffer* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_byte_array_clear(&SELF->backend);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.string_buffer_iterator_impl",
                      dx_string_buffer_iterator_impl,
                      dx_string_buffer);

static dx_result dx_string_buffer_iterator_impl_has_value(dx_bool* RETURN, dx_string_buffer_iterator_impl* SELF);

static dx_result dx_string_buffer_iterator_impl_get_value(uint32_t* RETURN, dx_string_buffer_iterator_impl* SELF);

static dx_result dx_string_buffer_iterator_impl_next(dx_string_buffer_iterator_impl* SELF);

static void dx_string_buffer_iterator_impl_destruct(dx_string_buffer_iterator_impl* SELF) {
  DX_UNREFERENCE(SELF->string_buffer);
  SELF->string_buffer = NULL;
}

static void dx_string_buffer_iterator_impl_dispatch_construct(dx_string_buffer_iterator_impl_dispatch* SELF) {
  DX_STRING_ITERATOR_DISPATCH(SELF)->has_value = (dx_result(*)(dx_bool*,dx_string_iterator*))dx_string_buffer_iterator_impl_has_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->get_value = (dx_result(*)(uint32_t*,dx_string_iterator*))dx_string_buffer_iterator_impl_get_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->next = (dx_result(*)(dx_string_iterator*))dx_string_buffer_iterator_impl_next;
}

dx_result dx_string_buffer_iterator_impl_construct(dx_string_buffer_iterator_impl* SELF, dx_string_buffer* string_buffer) {
  dx_rti_type* TYPE = dx_string_buffer_iterator_impl_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_string_iterator_construct(DX_STRING_ITERATOR(SELF))) {
    return DX_FAILURE;
  }
  if (!string_buffer) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->string_buffer = string_buffer;
  DX_REFERENCE(SELF->string_buffer);
  SELF->index = 0;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_string_buffer_iterator_impl_create(dx_string_buffer_iterator_impl** RETURN, dx_string_buffer* string_buffer) {
  DX_CREATE_PREFIX(dx_string_buffer_iterator_impl)
  if (dx_string_buffer_iterator_impl_construct(SELF, string_buffer)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result dx_string_buffer_iterator_impl_has_value(dx_bool* RETURN, dx_string_buffer_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size number_of_bytes = 0;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return DX_FAILURE;
  }
  *RETURN = SELF->index < number_of_bytes;
  return DX_SUCCESS;
}

static dx_result dx_string_buffer_iterator_impl_get_value(uint32_t* RETURN, dx_string_buffer_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size number_of_bytes = 0;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return DX_FAILURE;
  }
  if (SELF->index >= number_of_bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  void* bytes;
  if (dx_string_buffer_get_bytes(&bytes, SELF->string_buffer)) {
    return DX_FAILURE;
  }
  char const* current = ((char const*)bytes) + SELF->index;
  char const* end = current + number_of_bytes;
  char const x = *current;
  uint32_t code_point = 0;
  {
    // mask first byte with 1000.0000 (0x80)
    // if the result is 0000.0000 (0x00) then we have a one byte sequence.
    if (((*current) & 0x80) == 0x0) {
      code_point |= *current;
      *RETURN = code_point;
      return DX_SUCCESS;
    }
    // mask first byte with 1110.0000 (0xE0)
    // if the result is 1100.0000 (0xC0) then we have a two byte sequence.
    if (((*current) & 0xE0) == 0xC0) {
      code_point |= *current;
      // mask second byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (dx_size i = 0; i < 1; ++i) {
        code_point <<= 8;
        code_point |= *(++current);
        if (current == end) return DX_FAILURE;
        if ((*current & 0xC0) != 0x80) return DX_FAILURE;
      }
      *RETURN = code_point;
      return DX_SUCCESS;
    }
    // mask first byte with 1111.0000 (0xF0)
    // if the result is 1110.0000 (0xE0) then we have a three byte sequence.
    if ((x& 0xF0) == 0xE0) {
      // mask second to third byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (dx_size i = 0; i < 2; ++i) {
        code_point <<= 8;
        code_point |= *(++current);
        if (current == end) return DX_FAILURE;
        if ((*current & 0xC0) != 0x80) return DX_FAILURE;
      }
      *RETURN = code_point;
      return DX_SUCCESS;
    }
    // mask first byte with 1111.1000 (0xF8)
    // if the result is 1111.0000 (0xF0) then we have a four byte sequence.
    if ((x & 0xF8) == 0xF0) {
      // mask second to fourth byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (dx_size i = 0; i < 3; ++i) {
        code_point <<= 8;
        code_point |= *(++current);
        if (current == end) return DX_FAILURE;
        if ((*current & 0xC0) != 0x80) return DX_FAILURE;
      }
      *RETURN = code_point;
      return DX_SUCCESS;
    }
  }
  return DX_FAILURE;
}

static dx_result dx_string_buffer_iterator_impl_next(dx_string_buffer_iterator_impl* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size number_of_bytes;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return DX_FAILURE;
  }
  if (SELF->index >= number_of_bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  void* bytes;
  if (dx_string_buffer_get_bytes(&bytes, SELF->string_buffer)) {
    return DX_FAILURE;
  }
  char const* current = ((char const*)bytes) + SELF->index;
  char const* end = current + number_of_bytes;
  char const x = *current;
  uint32_t code_point = 0;
  {
    // mask first byte with 1000.0000 (0x80)
    // if the result is 0000.0000 (0x00) then we have a one byte sequence.
    if (((*current) & 0x80) == 0x0) {
      SELF->index += 1;
      return DX_SUCCESS;
    }
    // mask first byte with 1110.0000 (0xE0)
    // if the result is 1100.0000 (0xC0) then we have a two byte sequence.
    if (((*current) & 0xE0) == 0xC0) {
      // mask second byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (dx_size i = 0; i < 1; ++i) {
        if (current == end) return DX_FAILURE;
        if ((*current & 0xC0) != 0x80) return DX_FAILURE;
      }
      SELF->index += 2;
      return DX_SUCCESS;
    }
    // mask first byte with 1111.0000 (0xF0)
    // if the result is 1110.0000 (0xE0) then we have a three byte sequence.
    if ((x & 0xF0) == 0xE0) {
      // mask second to third byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (dx_size i = 0; i < 2; ++i) {
        if (current == end) return DX_FAILURE;
        if ((*current & 0xC0) != 0x80) return DX_FAILURE;
      }
      SELF->index += 3;
      return DX_SUCCESS;
    }
    // mask first byte with 1111.1000 (0xF8)
    // if the result is 1111.0000 (0xF0) then we have a four byte sequence.
    if ((x & 0xF8) == 0xF0) {
      // mask second to fourth byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (dx_size i = 0; i < 3; ++i) {
        if (current == end) return DX_FAILURE;
        if ((*current & 0xC0) != 0x80) return DX_FAILURE;
      }
      SELF->index += 4;
      return DX_SUCCESS;
    }
  }
  return DX_FAILURE;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
