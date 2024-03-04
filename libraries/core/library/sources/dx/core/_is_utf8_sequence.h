#if !defined(DX_CORE__IS_UTF8_SEQUENCE)
#define DX_CORE__IS_UTF8_SEQUENCE

#include "Core/Utf8.h"

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
/// @param p A pointer to a UTF-8 Byte sequence.
/// @param n The length of the UTF-8 Byte sequence.
/// @param source A pointer to a <code>_utf8_symbol_range</code> object.
/// @param target A pointer to a <code>_utf8_byte_range</code> object.
/// @success <code>*target</code> was assigned the range of Bytes.
/// @return #Core_Success on success. #Core_Failure.
/// @success
/// <code>*RETURN</code> was assigned @a true if the sequence of Bytes is an UTF-8 Byte sequence.
/// Otherwise it was assigned @a false.
/// @error Core_Error_ArgumentInvalid @a p is a null pointer.
/// @error Core_Error_ArgumentInvalid @a source is a null pointer.
/// @error Core_Error_ArgumentInvalid @a target is a null pointer.
Core_Result _utf8_symbol_range_to_byte_range(Core_Natural8 const* p, Core_Size n, _utf8_symbol_range* source, _utf8_byte_range* target);

/// @brief Get if the specified sequence of Bytes is a UTF-8 symbol sequence.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The number of Bytes in the array pointed to by @a p.
/// @param numberOfSymbols A pointer to a <code>core_Size</code> variable or a null pointer.
/// @return #Core_Success on success. #Core_Failure.
/// @success
/// <code>*RETURN</code> was assigned @a true if the sequence of Bytes is an UTF-8 symbol sequence.
/// Otherwise it was assigned @a false.
/// If the Byte sequence is a UTF-8 symbol sequence and if <code>numberOfSymbols</code> is not null,
/// then <code>*numberOfSymbols</code> was assigned the number of UTF-8 symbol sequence.
/// @error Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error Core_Error_ArgumentInvalid @a p is a null pointer.
static inline Core_Result _utf8_is_utf8_byte_sequence(Core_Boolean* RETURN, Core_Natural8 const* p, Core_Size n, Core_Size* numberOfSymbols) {
  if (!RETURN || !p) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size numberOfSymbols_ = 0;
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
    numberOfSymbols_++;
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
  if (numberOfSymbols) {
    *numberOfSymbols = numberOfSymbols_;
  }
  *RETURN = true;
  return Core_Success;
}

#endif // DX_CORE__IS_UTF8_SEQUENCE
