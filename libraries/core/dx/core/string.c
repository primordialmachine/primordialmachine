#include "dx/core/string.h"

#include "dx/core/_string_format.h"

#include "dx/core/byte_array.h"
#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/memory.h"
#include "dx/core/_is_utf8_sequence.h"

DX_DEFINE_OBJECT_TYPE("dx.string",
                      dx_string,
                      dx_object);

dx_string* dx_string_printfv(dx_string* format, va_list arguments) {
  dx_inline_byte_array buffer;
  if (dx_inline_byte_array_initialize(&buffer)) {
    return NULL;
  }
  if (dx__format_v(&buffer, format->bytes, format->bytes + format->number_of_bytes, arguments)) {
    dx_inline_byte_array_uninitialize(&buffer);
    return NULL;
  }
  dx_string* string = NULL;
  if (dx_string_create(&string, buffer.elements, buffer.size)) {
    dx_inline_byte_array_uninitialize(&buffer);
    return NULL;
  }
  dx_inline_byte_array_uninitialize(&buffer);
  return string;
}

dx_string* dx_string_printf(dx_string* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  dx_string* string = dx_string_printfv(format, arguments);
  va_end(arguments);
  return string;
}

static void dx_string_destruct(dx_string* self) {
  dx_memory_deallocate(self->bytes);
  self->bytes = NULL;
}

static void dx_string_dispatch_construct(dx_string_dispatch* self)
{/*Intentionally emtpy.*/}

int dx_string_construct(dx_string* self, char const* bytes, dx_size number_of_bytes) {
  if (DX_SIZE_GREATEST < number_of_bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_string_get_type();
  if (!_type) {
    return 1;
  }
  if (!bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (!dx__is_utf8_sequence(bytes, number_of_bytes)) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  self->bytes = NULL;
  if (dx_memory_allocate(&self->bytes, number_of_bytes)) {
    return 1;
  }
  dx_memory_copy(self->bytes, bytes, number_of_bytes);
  self->number_of_bytes = number_of_bytes;

  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_result dx_string_create(dx_string** RETURN, char const* bytes, dx_size number_of_bytes) {
  DX_CREATE_PREFIX(dx_string)
  if (dx_string_construct(SELF, bytes, number_of_bytes)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_string_get_bytes(void const** RETURN, dx_string* SELF) {
  *RETURN = SELF->bytes;
  return DX_SUCCESS;
}

dx_result dx_string_get_number_of_bytes(dx_size* RETURN, dx_string* SELF) {
  *RETURN = SELF->number_of_bytes;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Classify the first Byte of an UTF8 sequence to determine the length of the sequence.
/// @param x
/// The Byte.
/// @return
/// The length of the sequence.
/// The zero value on failure.
/// The function fails if there is an encoding error.
/// @failure This function has set the error variable to #DX_DECODING_FAILED.
static dx_size classify(uint8_t x) {
  if ((x & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    return 1;
  }
  else if ((x & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    return 2;
  }
  else if ((x & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    return 3;
  }
  else if ((x & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    return 4;
  }
  else {
    dx_set_error(DX_ERROR_DECODING_FAILED);
    return 0;
  }
}

typedef struct y_utf8_it {
  uint8_t const* current;
  uint8_t const* end;
  bool error;
} y_utf8_it;

int y_utf8_decode(y_utf8_it* it, dx_size* n, uint32_t* p) {
  if (it->error || it->current == it->end) {
    return 1;
  }
  dx_size l = classify(*it->current);
  if (!l) {
    it->error = true;
    return 1;
  }
  if (it->current + l > it->end) {
    it->error = true;
    return 1;
  }
  switch (l) {
  case 1: {
    *p = it->current[0];
  } break;
  case 2: {
    *p = (it->current[0] << 8)
      | (it->current[1] << 0);
  } break;
  case 3: {
    *p = (it->current[0] << 16)
      | (it->current[1] << 8)
      | (it->current[2] << 0);
  } break;
  case 4: {
    *p = (it->current[0] << 24)
      | (it->current[1] << 16)
      | (it->current[2] << 8)
      | (it->current[3] << 0);
  } break;
  };
  *n = l;
  it->current += l;
  return 0;
}

bool dx_string_contains_symbol(dx_string const* SELF, uint32_t symbol) {
  y_utf8_it it = { .current = (uint8_t*)SELF->bytes,
                   .end = (uint8_t*)SELF->bytes + SELF->number_of_bytes,
                   .error = false };
  uint32_t p;
  dx_size n;
  while (!y_utf8_decode(&it, &n, &p)) {
    if (p == symbol) {
      return true;
    }
  }
  return false;
}

bool dx_string_is_equal_to(dx_string const* SELF, dx_string const* other) {
  if (!SELF || !other) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return false;
  }
  if (SELF->number_of_bytes != other->number_of_bytes) {
    return false;
  }
  return !dx_memory_compare(SELF->bytes, other->bytes, SELF->number_of_bytes);
}

dx_size dx_string_get_hash_value(dx_string const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 0;
  }
  return dx_hash_bytes(SELF->bytes, SELF->number_of_bytes);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.string_iterator",
                       dx_string_iterator,
                       dx_object);

static void dx_string_iterator_destruct(dx_string_iterator* SELF)
{/*Intentionally empty.*/}

static void dx_string_iterator_dispatch_construct(dx_string_iterator_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_string_iterator_construct(dx_string_iterator* SELF) {
  dx_rti_type* TYPE = dx_string_iterator_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.string_iterator_impl",
                       dx_string_iterator_impl,
                       dx_string_iterator);

static dx_result dx_string_iterator_impl_has_value(dx_bool* RETURN, dx_string_iterator_impl* SELF);

static dx_result dx_string_iterator_impl_get_value(uint32_t* RETURN, dx_string_iterator_impl* SELF);

static dx_result dx_string_iterator_impl_next(dx_string_iterator_impl* SELF);

static void dx_string_iterator_impl_destruct(dx_string_iterator_impl* SELF) {
  DX_UNREFERENCE(SELF->string);
  SELF->string = NULL;
}

static void dx_string_iterator_impl_dispatch_construct(dx_string_iterator_impl_dispatch* SELF) {
  DX_STRING_ITERATOR_DISPATCH(SELF)->has_value = (dx_result(*)(dx_bool*,dx_string_iterator*))dx_string_iterator_impl_has_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->get_value = (dx_result(*)(uint32_t*,dx_string_iterator*))dx_string_iterator_impl_get_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->next = (dx_result(*)(dx_string_iterator*))dx_string_iterator_impl_next;
}

dx_result dx_string_iterator_impl_construct(dx_string_iterator_impl* SELF, dx_string* string) {
  dx_rti_type* TYPE = dx_string_iterator_impl_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_string_iterator_construct(DX_STRING_ITERATOR(SELF))) {
    return DX_FAILURE;
  }
  if (!string) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->string = string;
  DX_REFERENCE(SELF->string);
  SELF->index = 0;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_string_iterator_impl_create(dx_string_iterator_impl** RETURN, dx_string* string) {
  DX_CREATE_PREFIX(dx_string_iterator_impl)
  if (dx_string_iterator_impl_construct(SELF, string)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS; 
}

static dx_result dx_string_iterator_impl_has_value(dx_bool* RETURN, dx_string_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size number_of_bytes;
  if (dx_string_get_number_of_bytes(&number_of_bytes, SELF->string)) {
    return DX_FAILURE;
  }
  *RETURN = SELF->index < number_of_bytes;
  return DX_SUCCESS;
}

static dx_result dx_string_iterator_impl_get_value(uint32_t* RETURN, dx_string_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, SELF->string)) {
    return DX_FAILURE;
  }
  if (SELF->index >= number_of_bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  char const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, SELF->string)) {
    return DX_FAILURE;
  }
  uint32_t code_point = 0;
  char const* start = bytes;
  char const* current = bytes + SELF->index;
  char const* end = bytes + number_of_bytes;
  char const x = *current;
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

static dx_result dx_string_iterator_impl_next(dx_string_iterator_impl* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, SELF->string)) {
    return DX_FAILURE;
  }
  if (SELF->index >= number_of_bytes) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  char const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, SELF->string)) {
    return DX_FAILURE;
  }
  uint32_t code_point = 0;
  char const* start = bytes;
  char const* current = bytes + SELF->index;
  char const* end = bytes + number_of_bytes;
  char const x = *current;
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

int dx_str_to_i64(char const* p, dx_size l, int64_t* v) {
  if (!p || !v) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  char const* q = p + l;
  bool negative = false;
  if (p != q && *p == '-') {
    negative = true;
    p++;
    l -= 1;
  }
  uint64_t u;
  if (dx_str_to_u64(p, l, &u)) {
    return 1;
  }
  // neither +u nor -u can be represented by an int64_t value.
  if (u > INT64_MAX) {
    dx_set_error(DX_ERROR_CONVERSION_FAILED);
    return 1;
  }
  // u is within the range of [0,INT64_MAX].
  int64_t w = negative ? -(int64_t)u : +(int64_t)u;
  *v = w;
  return 0;
}

int dx_str_to_u64(char const* p, dx_size l, uint64_t* v) {
  if (!p || !v) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  char const* q = p + l;
  bool negative = false;
  if (p != q && *p == '+') {
    p++;
  } else if (p != q && *p == '-') {
    dx_set_error(DX_ERROR_CONVERSION_FAILED);
    return 1;
  }
  uint64_t u = 0;
  // there must be at least one digit
  if (!(p != q && '0' <= *p && *p <= '9')) {
    dx_set_error(DX_ERROR_CONVERSION_FAILED);
    return 1;
  }
  do {
    uint64_t overflow;
    // safe multiply
    u = dx_mul_u64(u, 10, &overflow);
    if (overflow) {
      dx_set_error(DX_ERROR_CONVERSION_FAILED);
      return 1;
    }
    uint64_t w = (uint64_t)(*p) - (uint64_t)'0';
    // safe add
    u = dx_add_u64(u, w, &overflow);
    if (overflow) {
      dx_set_error(DX_ERROR_CONVERSION_FAILED);
      return 1;
    }
    p++;
  } while (p != q && '0' <= *p && *p <= '9');
  if (p != q) {
    dx_set_error(DX_ERROR_CONVERSION_FAILED);
    return 1;
  }
  *v = u;
  return 0;
}
