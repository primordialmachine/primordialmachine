#if !defined(DX_CORE__IS_UTF8_SEQUENCE)
#define DX_CORE__IS_UTF8_SEQUENCE

#include "dx/core/core.h"

/// @brief Classify the first Byte of an UTF8 sequence to determine the length of the sequence.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param x The Byte.
/// @success <code>*RETURN</code> was assigned the length of the sequence.
/// @error Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error Core_Error_DecodingFailed @a x can not be classified.
static Core_Result _utf8_classify(Core_Size* RETURN, Core_Natural8 x) {
  if ((x & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    *RETURN = 1;
    return Core_Success;
  } else if ((x & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    *RETURN = 2;
    return Core_Success;
  } else if ((x & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    *RETURN = 3;
    return Core_Success;
  } else if ((x & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    *RETURN = 3;
    return Core_Success;
  } else {
    Core_setError(Core_Error_DecodingFailed);
    return Core_Failure;
  }
}

typedef struct _utf8_byte_range {
  Core_Size start;
  Core_Size length;
} _utf8_byte_range;

typedef struct _utf8_symbol_range {
  Core_Size start;
  Core_Size length;
} _utf8_symbol_range;

/// @brief Convert a range of symbols (specified by its starting index and its length) into a range of Bytes (specified by its starting index and its length).
/// @param context An opaque context pointer.
/// @param get A pointer to a function to read a Byte from the contet given its index.
/// @param symbols The range of symbols.
/// @param target A pointer to a <code>_utf8_byte_range</code> object.
/// @success <code>*target</code> was assigned the range of Bytes.
/// @return #Core_Success on success. #Core_Failure.
/// @success
/// <code>*RETURN</code> was assigned @a true if the sequence of Bytes is an UTF-8 Byte sequence.
/// Otherwise it was assigned @a false.
/// @error Core_Error_ArgumentInvalid @a get is a null pointer.
/// @error Core_Error_ArgumentInvalid @a source is a null pointer.
/// @error Core_Error_ArgumentInvalid @a target is a null pointer.
static Core_Result _utf8_symbol_range_to_byte_range(void *context, Core_Result (*get)(Core_Natural8*, void *context, Core_Size), _utf8_symbol_range* source, _utf8_byte_range* target) {
  _utf8_symbol_range currentSymbolRange = {
    .start = 0,
    .length = 0
  };
  _utf8_byte_range currentByteRange = {
    .start = 0,
    .length = 0
  };
  while (currentSymbolRange.start < source->start) {
    Core_Natural8 value;
    if (get(&value, context, currentByteRange.start + currentByteRange.length)) {
      return Core_Failure;
    }
    Core_Size j;
    if (_utf8_classify(&j, value)) {
      return Core_Failure;
    }
    currentByteRange.start += j;
    currentSymbolRange.start++;
  }
  while (currentSymbolRange.start + currentSymbolRange.length < source->start + source->length) {
    Core_Natural8 value;
    if (get(&value, context, currentByteRange.start + currentByteRange.length)) {
      return Core_Failure;
    }
    Core_Size j;
    if (_utf8_classify(&j, value)) {
      return Core_Failure;
    }
    currentByteRange.length += j;
    currentSymbolRange.length++;
  }
  *target = currentByteRange;
  return Core_Success;
}

/// @brief Get if the specified sequence of Bytes is a UTF-8 Byte sequence.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The number of Bytes in the array pointed to by @a p.
/// @return #Core_Success on success. #Core_Failure.
/// @success
/// <code>*RETURN</code> was assigned @a true if the sequence of Bytes is an UTF-8 Byte sequence.
/// Otherwise it was assigned @a false.
/// @error Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error Core_Error_ArgumentInvalid @a p is a null pointer.
static inline Core_Result _utf8_is_utf8_byte_sequence(Core_Boolean* RETURN, Core_Natural8 const* p, Core_Size n) {
  if (!RETURN || !p) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Natural8 const* current = p;
  Core_Natural8 const* end = p + n;
  while (current != end) {
    Core_Natural8 const x = *current;
    Core_Size i;
    if (_utf8_classify(&i, *current)) {
      Core_setError(Core_Error_NoError);
      *RETURN = false;
      return Core_Success;
    }
    current++;
    // mask second,third,fourth byte with 1100.0000 (0xC0)
    // if the result is 1000.0000 (0x80) then we have a valid sequence.
    for (Core_Size j = 0; j < i - 1; ++j) {
      if (current == end) {
        *RETURN = false;
        return Core_Success;
      }
      if ((*current & 0xC0) != 0x80) {
        *RETURN = false;
        return Core_Success;
      }
      current++;
      continue;
    }
  }
  *RETURN = true;
  return Core_Success;
}

#endif // DX_CORE__IS_UTF8_SEQUENCE
