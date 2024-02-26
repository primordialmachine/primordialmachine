#include "dx/core/string_buffer.h"

#include "Core/String.h"
#include "dx/core/_is_utf8_sequence.h"
#include "dx/core/_string_format.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("Core.StringBufferIterator",
                       Core_StringBufferIterator,
                       dx_string_iterator);

static inline Core_StringBufferIterator* CORE_STRINGBUFFERITERATOR(void* p) {
  return (Core_StringBufferIterator*)p;
}

struct Core_StringBufferIterator {
  Core_StringIterator _parent;
  Core_StringBuffer* string_buffer;
  Core_Size index;
};

static inline Core_StringBufferIterator_Dispatch* CORE_STRINGBUFFERITERATOR_DISPATCH(void* p) {
  return (Core_StringBufferIterator_Dispatch*)p;
}

struct Core_StringBufferIterator_Dispatch {
  Core_StringIterator_Dispatch _parent;
};

Core_Result Core_StringBufferIterator_construct(Core_StringBufferIterator* SELF, Core_StringBuffer* string_buffer);

Core_Result Core_StringBufferIterator_create(Core_StringBufferIterator** RETURN, Core_StringBuffer* string_buffer);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("Core.StringBuffer",
                      Core_StringBuffer,
                      Core_Object);

static void Core_StringBuffer_destruct(Core_StringBuffer* SELF) {
  Core_InlineArrayListN8_uninitialize(&SELF->backend);
}

static void Core_StringBuffer_constructDispatch(Core_StringBuffer_Dispatch* SELF)
{/*Intentionally emtpy.*/}

Core_Result Core_StringBuffer_construct(Core_StringBuffer* SELF) {
  Core_BeginConstructor(Core_StringBuffer);
  Core_InlineArrayListN8_Configuration configuration = {
    .addedCallback = NULL,
    .removedCallback = NULL,
  };
  if (Core_InlineArrayListN8_initialize(&SELF->backend, 0, &configuration)) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_StringBuffer);
}

Core_Result Core_StringBuffer_create(Core_StringBuffer** RETURN) {
  DX_CREATE_PREFIX(Core_StringBuffer);
  if (Core_StringBuffer_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_StringBuffer_clone(Core_StringBuffer** RETURN, Core_StringBuffer const* SELF) {
  Core_StringBuffer* clone = NULL;
  if (Core_StringBuffer_create(&clone)) {
    return Core_Failure;
  }
  if (Core_StringBuffer_appendBytes(clone, SELF->backend.elements, SELF->backend.size)) {
    CORE_UNREFERENCE(clone);
    clone = NULL;
    return Core_Failure;
  }
  *RETURN = clone;
  return Core_Success;
}

Core_Result Core_StringBuffer_get_byte(Core_Natural8* RETURN, Core_StringBuffer* SELF, Core_Size index) {
  return Core_InlineArrayListN8_get(RETURN, &SELF->backend, index);
}

Core_Result Core_StringBuffer_get_bytes(void** RETURN, Core_StringBuffer* SELF) {
  return Core_InlineArrayListN8_getElements(RETURN, &SELF->backend);
}

Core_Result Core_StringBuffer_create_iterator(Core_StringIterator** RETURN, Core_StringBuffer* SELF) {
  return Core_StringBufferIterator_create((Core_StringBufferIterator**)RETURN, SELF);
}

Core_Result Core_StringBuffer_appendBytes(Core_StringBuffer* SELF, char const* p, Core_Size n) {
  Core_Boolean t;
  if (_utf8_is_utf8_byte_sequence(&t, p, n, NULL)) {
    return Core_Failure;
  }
  if (!t) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_appendMany(&SELF->backend, p, n);
}

Core_Result Core_StringBuffer_appendString(Core_StringBuffer* SELF, Core_String* string) {
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

Core_Result Core_StringBuffer_clear(Core_StringBuffer* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_clear(&SELF->backend);
}

Core_Result Core_StringBuffer_getNumberOfBytes(Core_Size* RETURN, Core_StringBuffer* SELF) {
  return Core_InlineArrayListN8_getSize(RETURN, &SELF->backend);
}

Core_Result Core_StringBuffer_getNumberOfSymbols(Core_Size* RETURN, Core_StringBuffer* SELF) {
  return Core_InlineArrayListN8_getSize(RETURN, &SELF->backend);
}

Core_Result Core_StringBuffer_getString(Core_String** RETURN, Core_StringBuffer* SELF) {
  return Core_String_createFromArray(RETURN, &SELF->backend);
}

Core_Result Core_StringBuffer_getSubstring(Core_String** RETURN, Core_StringBuffer* SELF, Core_Size start, Core_Size length) {
  _utf8_symbol_range source = { .start = start, .length = length };
  _utf8_byte_range target = { .start = 0, .length = 0 };
  if (_utf8_symbol_range_to_byte_range(SELF->backend.elements, SELF->backend.size, &source, &target)) {
    return Core_Failure;
  }
  return Core_String_createFromSubArray(RETURN, &SELF->backend, target.start, target.length);
}

Core_Result Core_StringBuffer_prependBytes(Core_StringBuffer* SELF, Core_Natural8 const* p, Core_Size n) {
  Core_Boolean t;
  if (_utf8_is_utf8_byte_sequence(&t, p, n, NULL)) {
    return Core_Failure;
  }
  if (!t) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_prependMany(&SELF->backend, p, n);
}

Core_Result Core_StringBuffer_prependString(Core_StringBuffer* SELF, Core_String* string) {
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

Core_Result Core_StringBuffer_setBytes(Core_StringBuffer* SELF, Core_Natural8 const* p, Core_Size n) {
  Core_Boolean t;
  if (_utf8_is_utf8_byte_sequence(&t, p, n, NULL)) {
    return Core_Failure;
  }
  if (!t) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListN8_set(&SELF->backend, p, n);
}

Core_Result Core_StringBuffer_setString(Core_StringBuffer* SELF, Core_String* string) {
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

Core_Result Core_StringBuffer_appendFV(Core_StringBuffer* SELF, Core_String* format, va_list arguments) {
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

Core_Result Core_StringBuffer_appendF(Core_StringBuffer* SELF, Core_String* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  Core_Result result = Core_StringBuffer_appendFV(SELF, format, arguments);
  va_end(arguments);
  return result;
}

Core_Result Core_StringBuffer_remove_last(Core_StringBuffer* SELF) {
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("Core.StringBufferIterator",
                      Core_StringBufferIterator,
                      Core_StringIterator);

static Core_Result Core_StringBufferIterator_has_value(Core_Boolean* RETURN, Core_StringBufferIterator* SELF);

static Core_Result Core_StringBufferIterator_get_value(uint32_t* RETURN, Core_StringBufferIterator* SELF);

static Core_Result Core_StringBufferIterator_next(Core_StringBufferIterator* SELF);

static Core_Result Core_StringBufferIterator_next_n(Core_StringBufferIterator* SELF, Core_Size n);

static Core_Result _string_buffer_iterator_increment_n(Core_StringBufferIterator* SELF, Core_Size n);

static void Core_StringBufferIterator_destruct(Core_StringBufferIterator* SELF) {
  CORE_UNREFERENCE(SELF->string_buffer);
  SELF->string_buffer = NULL;
}

static void Core_StringBufferIterator_constructDispatch(Core_StringBufferIterator_Dispatch* SELF) {
  CORE_STRINGITERATOR_DISPATCH(SELF)->hasValue = (Core_Result(*)(Core_Boolean*, Core_StringIterator*))&Core_StringBufferIterator_has_value;
  CORE_STRINGITERATOR_DISPATCH(SELF)->getValue = (Core_Result(*)(uint32_t*, Core_StringIterator*))&Core_StringBufferIterator_get_value;
  CORE_STRINGITERATOR_DISPATCH(SELF)->next = (Core_Result(*)(Core_StringIterator*))&Core_StringBufferIterator_next;
  CORE_STRINGITERATOR_DISPATCH(SELF)->next_n = (Core_Result(*)(Core_StringIterator*, Core_Size))Core_StringBufferIterator_next_n;
}

Core_Result Core_StringBufferIterator_construct(Core_StringBufferIterator* SELF, Core_StringBuffer* string_buffer) {
  Core_BeginConstructor(Core_StringBufferIterator);
  
  if (Core_StringIterator_construct(CORE_STRINGITERATOR(SELF))) {
    return Core_Failure;
  }
  if (!string_buffer) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->string_buffer = string_buffer;
  CORE_REFERENCE(SELF->string_buffer);
  SELF->index = 0;

  Core_EndConstructor(Core_StringBufferIterator);
}

Core_Result Core_StringBufferIterator_create(Core_StringBufferIterator** RETURN, Core_StringBuffer* string_buffer) {
  DX_CREATE_PREFIX(Core_StringBufferIterator);
  if (Core_StringBufferIterator_construct(SELF, string_buffer)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result Core_StringBufferIterator_has_value(Core_Boolean* RETURN, Core_StringBufferIterator* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (Core_StringBuffer_getNumberOfBytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  *RETURN = SELF->index < number_of_bytes;
  return Core_Success;
}

static Core_Result _string_buffer_iterator_increment_n(Core_StringBufferIterator* SELF, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  if (Core_StringBuffer_getNumberOfBytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  void* bytes;
  if (Core_StringBuffer_get_bytes(&bytes, SELF->string_buffer)) {
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

static Core_Result Core_StringBufferIterator_get_value(uint32_t* RETURN, Core_StringBufferIterator* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (Core_StringBuffer_getNumberOfBytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  void* bytes;
  if (Core_StringBuffer_get_bytes(&bytes, SELF->string_buffer)) {
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

static Core_Result Core_StringBufferIterator_next(Core_StringBufferIterator* SELF) {
  return _string_buffer_iterator_increment_n(SELF, 1);
}

static Core_Result Core_StringBufferIterator_next_n(Core_StringBufferIterator* SELF, Core_Size n) {
  return _string_buffer_iterator_increment_n(SELF, n);
}

static Core_Result Core_StringBufferIterator_next_many(Core_StringBufferIterator* SELF, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  if (Core_StringBuffer_getNumberOfBytes(&number_of_bytes, SELF->string_buffer)) {
    return Core_Failure;
  }
  if (SELF->index >= number_of_bytes) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  void* bytes;
  if (Core_StringBuffer_get_bytes(&bytes, SELF->string_buffer)) {
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
