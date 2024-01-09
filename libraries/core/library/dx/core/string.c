#include "dx/core/string.h"

#include "dx/core/_is_utf8_sequence.h"
#include "dx/core/_string_format.h"
#include "dx/core/inline_byte_array.h"
#include "Core/Hash.h"
#include "Core/Memory.h"
#include "Core/safeAddNx.h"
#include "Core/safeMulNx.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @internal 
Core_declareObjectType("dx.string_iterator_impl",
                       dx_string_iterator_impl,
                       dx_string_iterator);

/// @internal 
static inline dx_string_iterator_impl* DX_STRING_ITERATOR_IMPL(void* p) {
  return (dx_string_iterator_impl*)p;
}

/// @internal 
struct dx_string_iterator_impl {
  dx_string_iterator _parent;
  Core_String* string;
  Core_Size index;
};

/// @internal 
static inline dx_string_iterator_impl_Dispatch* DX_STRING_ITERATOR_IMPL_DISPATCH(void* p) {
  return (dx_string_iterator_impl_Dispatch*)p;
}

/// @internal 
struct dx_string_iterator_impl_Dispatch {
  dx_string_iterator_Dispatch _parent;
};

/// @internal 
Core_Result dx_string_iterator_impl_construct(dx_string_iterator_impl* SELF, Core_String* string);

/// @internal 
Core_Result dx_string_iterator_impl_create(dx_string_iterator_impl** RETURN, Core_String* string);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("Core.String",
                      Core_String,
                      Core_Object);

Core_Result Core_String_printfv(Core_String** RETURN, Core_String* format, va_list arguments) {
  Core_InlineArrayN8 buffer;
  if (Core_InlineArrayN8_initialize(&buffer)) {
    return Core_Failure;
  }
  if (dx__format_v(&buffer, format->bytes, format->bytes + format->number_of_bytes, arguments)) {
    Core_InlineArrayN8_uninitialize(&buffer);
    return Core_Failure;
  }
  Core_String* string = NULL;
  if (Core_String_create(&string, buffer.elements, buffer.size)) {
    Core_InlineArrayN8_uninitialize(&buffer);
    return Core_Failure;
  }
  Core_InlineArrayN8_uninitialize(&buffer);
  *RETURN = string;
  return Core_Success;
}

Core_Result Core_String_printf(Core_String** RETURN, Core_String* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  Core_String* string = NULL;
  if (Core_String_printfv(&string, format, arguments)) {
    va_end(arguments);
    return Core_Failure;
  }
  va_end(arguments);
  *RETURN = string;
  return Core_Success;
}

static void Core_String_destruct(Core_String* SELF) {
  Core_Memory_deallocate(SELF->bytes);
  SELF->bytes = NULL;
}

static void Core_String_constructDispatch(Core_String_Dispatch* SELF)
{/*Intentionally emtpy.*/}

Core_Result Core_String_construct(Core_String* SELF, char const* bytes, Core_Size number_of_bytes) {
  DX_CONSTRUCT_PREFIX(Core_String);
  if (Core_Size_Greatest < number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!dx__is_utf8_sequence(bytes, number_of_bytes)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->bytes = NULL;
  if (Core_Memory_allocate(&SELF->bytes, number_of_bytes)) {
    return Core_Failure;
  }
  Core_Memory_copy(SELF->bytes, bytes, number_of_bytes);
  SELF->number_of_bytes = number_of_bytes;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_String_create(Core_String** RETURN, char const* bytes, Core_Size number_of_bytes) {
  DX_CREATE_PREFIX(Core_String);
  if (Core_String_construct(SELF, bytes, number_of_bytes)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_String_getBytes(void const** RETURN, Core_String* SELF) {
  *RETURN = SELF->bytes;
  return Core_Success;
}

Core_Result Core_String_getNumberOfBytes(Core_Size* RETURN, Core_String* SELF) {
  *RETURN = SELF->number_of_bytes;
  return Core_Success;
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
static Core_Size classify(uint8_t x) {
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
    Core_setError(Core_Error_DecodingFailed);
    return 0;
  }
}

typedef struct y_utf8_it {
  uint8_t const* current;
  uint8_t const* end;
  bool error;
} y_utf8_it;

int y_utf8_decode(y_utf8_it* it, Core_Size* n, uint32_t* p) {
  if (it->error || it->current == it->end) {
    return 1;
  }
  Core_Size l = classify(*it->current);
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

Core_Boolean dx_string_contains_symbol(Core_String const* SELF, uint32_t symbol) {
  y_utf8_it it = { .current = (uint8_t*)SELF->bytes,
                   .end = (uint8_t*)SELF->bytes + SELF->number_of_bytes,
                   .error = false };
  uint32_t p;
  Core_Size n;
  while (!y_utf8_decode(&it, &n, &p)) {
    if (p == symbol) {
      return true;
    }
  }
  return false;
}

Core_Result Core_String_containsSymbol(Core_Boolean* RETURN, Core_String const* SELF, uint32_t symbol) {
  y_utf8_it it = { .current = (uint8_t*)SELF->bytes,
                   .end = (uint8_t*)SELF->bytes + SELF->number_of_bytes,
                   .error = false };
  uint32_t p;
  Core_Size n;
  while (!y_utf8_decode(&it, &n, &p)) {
    if (p == symbol) {
      *RETURN = Core_True;
      return Core_Success;
    }
  }
  *RETURN = Core_False;
  return Core_Success;
}

Core_Result Core_String_isEqualTo(Core_Boolean* RETURN, Core_String const* SELF, Core_String const* other) {
  if (!RETURN || !SELF || !other) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->number_of_bytes != other->number_of_bytes) {
    *RETURN = false;
  } else {
    Core_Integer8 temporary;
    Core_Memory_compare(&temporary, SELF->bytes, other->bytes, SELF->number_of_bytes); // must succeed
    *RETURN = !temporary;
  }
  return Core_Success;
}

Core_Result Core_String_getHashValue(Core_Size* RETURN, Core_String const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_hashBytes(RETURN, SELF->bytes, SELF->number_of_bytes);
}

Core_Result Core_String_create_iterator(dx_string_iterator** RETURN, Core_String* SELF) {
  return dx_string_iterator_impl_create((dx_string_iterator_impl**)RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.string_iterator_impl",
                       dx_string_iterator_impl,
                       dx_string_iterator);

static Core_Result dx_string_iterator_impl_has_value(Core_Boolean* RETURN, dx_string_iterator_impl* SELF);

static Core_Result dx_string_iterator_impl_get_value(uint32_t* RETURN, dx_string_iterator_impl* SELF);

static Core_Result dx_string_iterator_impl_next(dx_string_iterator_impl* SELF);

static void dx_string_iterator_impl_destruct(dx_string_iterator_impl* SELF) {
  CORE_UNREFERENCE(SELF->string);
  SELF->string = NULL;
}

static void dx_string_iterator_impl_constructDispatch(dx_string_iterator_impl_Dispatch* SELF) {
  DX_STRING_ITERATOR_DISPATCH(SELF)->has_value = (Core_Result(*)(Core_Boolean*,dx_string_iterator*))dx_string_iterator_impl_has_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->get_value = (Core_Result(*)(uint32_t*,dx_string_iterator*))dx_string_iterator_impl_get_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->next = (Core_Result(*)(dx_string_iterator*))dx_string_iterator_impl_next;
}

Core_Result dx_string_iterator_impl_construct(dx_string_iterator_impl* SELF, Core_String* string) {
  DX_CONSTRUCT_PREFIX(dx_string_iterator_impl);
  if (dx_string_iterator_construct(DX_STRING_ITERATOR(SELF))) {
    return Core_Failure;
  }
  if (!string) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->string = string;
  CORE_REFERENCE(SELF->string);
  SELF->index = 0;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_string_iterator_impl_create(dx_string_iterator_impl** RETURN, Core_String* string) {
  DX_CREATE_PREFIX(dx_string_iterator_impl);
  if (dx_string_iterator_impl_construct(SELF, string)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success; 
}

static Core_Result dx_string_iterator_impl_has_value(Core_Boolean* RETURN, dx_string_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  if (Core_String_getNumberOfBytes(&number_of_bytes, SELF->string)) {
    return Core_Failure;
  }
  *RETURN = SELF->index < number_of_bytes;
  return Core_Success;
}

static Core_Result dx_string_iterator_impl_get_value(uint32_t* RETURN, dx_string_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, SELF->string)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* bytes = NULL;
  if (Core_String_getBytes(&bytes, SELF->string)) {
    return Core_Failure;
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
      return Core_Success;
    }
    // mask first byte with 1110.0000 (0xE0)
    // if the result is 1100.0000 (0xC0) then we have a two byte sequence.
    if (((*current) & 0xE0) == 0xC0) {
      code_point |= *current;
      // mask second byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (Core_Size i = 0; i < 1; ++i) {
        code_point <<= 8;
        code_point |= *(++current);
        if (current == end) return Core_Failure;
        if ((*current & 0xC0) != 0x80) return Core_Failure;
      }
      *RETURN = code_point;
      return Core_Success;
    }
    // mask first byte with 1111.0000 (0xF0)
    // if the result is 1110.0000 (0xE0) then we have a three byte sequence.
    if ((x& 0xF0) == 0xE0) {
      // mask second to third byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (Core_Size i = 0; i < 2; ++i) {
        code_point <<= 8;
        code_point |= *(++current);
        if (current == end) return Core_Failure;
        if ((*current & 0xC0) != 0x80) return Core_Failure;
      }
      *RETURN = code_point;
      return Core_Success;
    }
    // mask first byte with 1111.1000 (0xF8)
    // if the result is 1111.0000 (0xF0) then we have a four byte sequence.
    if ((x & 0xF8) == 0xF0) {
      // mask second to fourth byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (Core_Size i = 0; i < 3; ++i) {
        code_point <<= 8;
        code_point |= *(++current);
        if (current == end) return Core_Failure;
        if ((*current & 0xC0) != 0x80) return Core_Failure;
      }
      *RETURN = code_point;
      return Core_Success;
    }
  }
  return Core_Failure;
}

static Core_Result dx_string_iterator_impl_next(dx_string_iterator_impl* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, SELF->string)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* bytes = NULL;
  if (Core_String_getBytes(&bytes, SELF->string)) {
    return Core_Failure;
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
      return Core_Success;
    }
    // mask first byte with 1110.0000 (0xE0)
    // if the result is 1100.0000 (0xC0) then we have a two byte sequence.
    if (((*current) & 0xE0) == 0xC0) {
      // mask second byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (Core_Size i = 0; i < 1; ++i) {
        if (current == end) return Core_Failure;
        if ((*current & 0xC0) != 0x80) return Core_Failure;
      }
      SELF->index += 2;
      return Core_Success;
    }
    // mask first byte with 1111.0000 (0xF0)
    // if the result is 1110.0000 (0xE0) then we have a three byte sequence.
    if ((x & 0xF0) == 0xE0) {
      // mask second to third byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (Core_Size i = 0; i < 2; ++i) {
        if (current == end) return Core_Failure;
        if ((*current & 0xC0) != 0x80) return Core_Failure;
      }
      SELF->index += 3;
      return Core_Success;
    }
    // mask first byte with 1111.1000 (0xF8)
    // if the result is 1111.0000 (0xF0) then we have a four byte sequence.
    if ((x & 0xF8) == 0xF0) {
      // mask second to fourth byte with 1100.0000 (0xC0)
      // if the result is 1000.0000 (0x80) then we have a valid sequence.
      for (Core_Size i = 0; i < 3; ++i) {
        if (current == end) return Core_Failure;
        if ((*current & 0xC0) != 0x80) return Core_Failure;
      }
      SELF->index += 4;
      return Core_Success;
    }
  }
  return Core_Failure;
}
