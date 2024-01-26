#include "dx/core/string_buffer.h"

#include "dx/core/string.h"
#include "dx/core/_is_utf8_sequence.h"
#include "dx/core/_string_format.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.string_buffer_iterator_impl",
                       dx_string_buffer_iterator_impl,
                       dx_string_iterator);

static inline dx_string_buffer_iterator_impl* DX_STRING_BUFFER_ITERATOR_IMPL(void* p) {
  return (dx_string_buffer_iterator_impl*)p;
}

struct dx_string_buffer_iterator_impl {
  dx_string_iterator _parent;
  dx_string_buffer* string_buffer;
  Core_Size index;
};

static inline dx_string_buffer_iterator_impl_Dispatch* DX_STRING_BUFFER_ITERATOR_IMPL_DISPATCH(void* p) {
  return (dx_string_buffer_iterator_impl_Dispatch*)p;
}

struct dx_string_buffer_iterator_impl_Dispatch {
  dx_string_iterator_Dispatch _parent;
};

Core_Result dx_string_buffer_iterator_impl_construct(dx_string_buffer_iterator_impl* SELF, dx_string_buffer* string_buffer);

Core_Result dx_string_buffer_iterator_impl_create(dx_string_buffer_iterator_impl** RETURN, dx_string_buffer* string_buffer);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.string_buffer",
                      dx_string_buffer,
                      Core_Object);

static void dx_string_buffer_destruct(dx_string_buffer* SELF) {
  Core_InlineArrayListN8_uninitialize(&SELF->backend);
}

static void dx_string_buffer_constructDispatch(dx_string_buffer_Dispatch* SELF)
{/*Intentionally emtpy.*/}

Core_Result dx_string_buffer_construct(dx_string_buffer* SELF) {
  DX_CONSTRUCT_PREFIX(dx_string_buffer);
  Core_InlineArrayListN8_Configuration configuration = {
    .addedCallback = NULL,
    .removedCallback = NULL,
  };
  if (Core_InlineArrayListN8_initialize(&SELF->backend, 0, &configuration)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_string_buffer_create(dx_string_buffer** RETURN) {
  DX_CREATE_PREFIX(dx_string_buffer);
  if (dx_string_buffer_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_string_buffer_clone(dx_string_buffer** RETURN, dx_string_buffer const* SELF) {
  dx_string_buffer* clone = NULL;
  if (dx_string_buffer_create(&clone)) {
    return Core_Failure;
  }
  if (dx_string_buffer_append_bytes(clone, SELF->backend.elements, SELF->backend.size)) {
    CORE_UNREFERENCE(clone);
    clone = NULL;
    return Core_Failure;
  }
  *RETURN = clone;
  return Core_Success;
}

Core_Result dx_string_buffer_get_byte(Core_Natural8* RETURN, dx_string_buffer* SELF, Core_Size index) {
  return Core_InlineArrayListN8_get(RETURN, &SELF->backend, index);
}

Core_Result dx_string_buffer_get_number_of_bytes(Core_Size* RETURN, dx_string_buffer* SELF) {
  return Core_InlineArrayListN8_getSize(RETURN, &SELF->backend);
}

Core_Result dx_string_buffer_get_bytes(void** RETURN, dx_string_buffer* SELF) {
  return Core_InlineArrayListN8_getElements(RETURN, &SELF->backend);
}

Core_Result dx_string_buffer_get_string(Core_String** RETURN, dx_string_buffer* SELF) {
  return Core_String_createFromArray(RETURN, &SELF->backend);
}

Core_Result dx_string_buffer_get_substring(Core_String** RETURN, dx_string_buffer* SELF, Core_Size start, Core_Size length) {
  return Core_String_createFromSubArray(RETURN, &SELF->backend, start, length);
}

Core_Result dx_string_buffer_create_iterator(dx_string_iterator** RETURN, dx_string_buffer* SELF) {
  return dx_string_buffer_iterator_impl_create((dx_string_buffer_iterator_impl**)RETURN, SELF);
}

Core_Result dx_string_buffer_set_bytes(dx_string_buffer* SELF, Core_Natural8 const* p, Core_Size n) {
  Core_Boolean t;
  if (_utf8_is_utf8_byte_sequence(&t, p, n)) {
    return Core_Failure;
  }
  if (!t) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_set(&SELF->backend, p, n);
}

Core_Result dx_string_buffer_set_string(dx_string_buffer* SELF, Core_String* string) {
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, string)) {
    return Core_Failure;
  }
  void const* bytes = NULL;
  if (Core_String_getBytes(&bytes, string)) {
    return Core_Failure;
  }
  return Core_InlineArrayListN8_set(&SELF->backend, bytes, number_of_bytes);
}

Core_Result dx_string_buffer_prepend_bytes(dx_string_buffer* SELF, Core_Natural8 const* p, Core_Size n) {
  Core_Boolean t;
  if (_utf8_is_utf8_byte_sequence(&t, p, n)) {
    return Core_Failure;
  }
  if (!t) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_prependMany(&SELF->backend, p, n);
}

Core_Result dx_string_buffer_prepend_string(dx_string_buffer* SELF, Core_String* string) {
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, string)) {
    return Core_Failure;
  }
  void const* bytes = NULL;
  if (Core_String_getBytes(&bytes, string)) {
    return Core_Failure;
  }
  return Core_InlineArrayListN8_prependMany(&SELF->backend, bytes, number_of_bytes);
}

Core_Result dx_string_buffer_append_bytes(dx_string_buffer* SELF, char const* p, Core_Size n) {
  Core_Boolean t;
  if (_utf8_is_utf8_byte_sequence(&t, p, n)) {
    return Core_Failure;
  }
  if (!t) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_appendMany(&SELF->backend, p, n);
}

Core_Result dx_string_buffer_append_string(dx_string_buffer* SELF, Core_String* string) {
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, string)) {
    return Core_Failure;
  }
  void const* bytes = NULL;
  if (Core_String_getBytes(&bytes, string)) {
    return Core_Failure;
  }
  return Core_InlineArrayListN8_appendMany(&SELF->backend, bytes, number_of_bytes);
}

Core_Result dx_string_buffer_append_fv(dx_string_buffer* SELF, Core_String* format, va_list arguments) {
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, format)) {
    return Core_Failure;
  }
  void const* bytes = NULL;
  if (Core_String_getBytes(&bytes, format)) {
    return Core_Failure;
  }
  return dx__format_v(&SELF->backend, bytes, ((char const*)bytes) + number_of_bytes, arguments);
}

Core_Result dx_string_buffer_append_f(dx_string_buffer* SELF, Core_String* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  Core_Result result = dx_string_buffer_append_fv(SELF, format, arguments);
  va_end(arguments);
  return result;
}

Core_Result dx_string_buffer_remove_last(dx_string_buffer* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size size;
  if (Core_InlineArrayListN8_getSize(&size, &SELF->backend)) {
    return Core_Failure;
  }
  if (!size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Natural8 value;
  Core_Size received = 1;
  Core_Size index = size - 1;
  Core_InlineArrayListN8_get(&value, &SELF->backend, index);
  while ((value & 0xC0) == 0x80) {
    if (index == 0) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
    index--;
    received++;
  }
  Core_Size expected;
  if (_utf8_classify(&expected, value)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_removeMany(&SELF->backend, size - received, received);
}

Core_Result dx_string_buffer_clear(dx_string_buffer* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_clear(&SELF->backend);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.string_buffer_iterator_impl",
                      dx_string_buffer_iterator_impl,
                      dx_string_iterator);

static Core_Result dx_string_buffer_iterator_impl_has_value(Core_Boolean* RETURN, dx_string_buffer_iterator_impl* SELF);

static Core_Result dx_string_buffer_iterator_impl_get_value(uint32_t* RETURN, dx_string_buffer_iterator_impl* SELF);

static Core_Result dx_string_buffer_iterator_impl_next(dx_string_buffer_iterator_impl* SELF);

static Core_Result dx_string_buffer_iterator_impl_next_n(dx_string_buffer_iterator_impl* SELF, Core_Size n);

static Core_Result _string_buffer_iterator_increment_n(dx_string_buffer_iterator_impl* SELF, Core_Size n);

static void dx_string_buffer_iterator_impl_destruct(dx_string_buffer_iterator_impl* SELF) {
  CORE_UNREFERENCE(SELF->string_buffer);
  SELF->string_buffer = NULL;
}

static void dx_string_buffer_iterator_impl_constructDispatch(dx_string_buffer_iterator_impl_Dispatch* SELF) {
  DX_STRING_ITERATOR_DISPATCH(SELF)->has_value = (Core_Result(*)(Core_Boolean*, dx_string_iterator*))&dx_string_buffer_iterator_impl_has_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->get_value = (Core_Result(*)(uint32_t*, dx_string_iterator*))&dx_string_buffer_iterator_impl_get_value;
  DX_STRING_ITERATOR_DISPATCH(SELF)->next = (Core_Result(*)(dx_string_iterator*))&dx_string_buffer_iterator_impl_next;
  DX_STRING_ITERATOR_DISPATCH(SELF)->next_n = (Core_Result(*)(dx_string_iterator*, Core_Size))dx_string_buffer_iterator_impl_next_n;
}

Core_Result dx_string_buffer_iterator_impl_construct(dx_string_buffer_iterator_impl* SELF, dx_string_buffer* string_buffer) {
  DX_CONSTRUCT_PREFIX(dx_string_buffer_iterator_impl);
  if (dx_string_iterator_construct(DX_STRING_ITERATOR(SELF))) {
    return Core_Failure;
  }
  if (!string_buffer) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->string_buffer = string_buffer;
  CORE_REFERENCE(SELF->string_buffer);
  SELF->index = 0;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_string_buffer_iterator_impl_create(dx_string_buffer_iterator_impl** RETURN, dx_string_buffer* string_buffer) {
  DX_CREATE_PREFIX(dx_string_buffer_iterator_impl);
  if (dx_string_buffer_iterator_impl_construct(SELF, string_buffer)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result dx_string_buffer_iterator_impl_has_value(Core_Boolean* RETURN, dx_string_buffer_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  *RETURN = SELF->index < number_of_bytes;
  return Core_Success;
}

static Core_Result _string_buffer_iterator_increment_n(dx_string_buffer_iterator_impl* SELF, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  void* bytes;
  if (dx_string_buffer_get_bytes(&bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  Core_Natural8 const* end = ((Core_Natural8 const*)bytes) + number_of_bytes;
  Core_Size old_index = SELF->index;
  while (n > 0) {
    Core_Natural8 const* current = ((Core_Natural8 const*)bytes) + SELF->index;
    Core_Size j;
    if (_utf8_classify(&j, *current)) {
      SELF->index = old_index;
      return Core_Failure;
    }
    if (number_of_bytes - SELF->index < j) {
      Core_setError(Core_Error_DecodingFailed);
      SELF->index = old_index;
      return Core_Failure;
    }
    SELF->index += j;
    // Decrement n.
    n--;
  }
  return Core_Success;
}

static Core_Result dx_string_buffer_iterator_impl_get_value(uint32_t* RETURN, dx_string_buffer_iterator_impl* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  void* bytes;
  if (dx_string_buffer_get_bytes(&bytes, SELF->string_buffer)) {
    return Core_Failure;
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

static Core_Result dx_string_buffer_iterator_impl_next(dx_string_buffer_iterator_impl* SELF) {
  return _string_buffer_iterator_increment_n(SELF, 1);
}

static Core_Result dx_string_buffer_iterator_impl_next_n(dx_string_buffer_iterator_impl* SELF, Core_Size n) {
  return _string_buffer_iterator_increment_n(SELF, n);
}

static Core_Result dx_string_buffer_iterator_impl_next_many(dx_string_buffer_iterator_impl* SELF, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  void* bytes;
  if (dx_string_buffer_get_bytes(&bytes, SELF->string_buffer)) {
    return Core_Failure;
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
