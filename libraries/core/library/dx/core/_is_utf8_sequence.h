#if !defined(DX_CORE__IS_UTF8_SEQUENCE)
#define DX_CORE__IS_UTF8_SEQUENCE

#include "dx/core/core.h"

static inline Core_Natural8 _utf8_classify(char const x) {
  // mask first byte with 1000.0000 (0x80)
  // if the result is 0000.0000 (0x00) then we have a one byte sequence.
  if ((x & 0x80) == 0x00) {
    return 1;
  }
  // mask first byte with 1110.0000 (0xE0)
  // if the result is 1100.0000 (0xC0) then we have a two byte sequence.
  if ((x & 0xE0) == 0xC0) {
    return 2;
  }
  // mask first byte with 1111.0000 (0xF0)
  // if the result is 1110.0000 (0xE0) then we have a three byte sequence.
  if ((x & 0xF0) == 0xE0) {
    return 3;
  }
  // mask first byte with 1111.1000 (0xF8)
  // if the result is 1111.0000 (0xF0) then we have a four byte sequence.
  if ((x & 0xF8) == 0xF0) {
    return 4;
  }
  return 0;
}

/// @brief
/// Get if the specified sequence of Bytes is a UTF-8 sequence.
/// @param start
/// A pointer to the first Byte.
/// @param length
/// The number of Bytes.
/// @return
/// @a true if the sequence of Bytes is a UTF-8 sequence.
/// @a false if the sequence of Bytes is not a UTF-8 sequence.
/// @a false is also returned on failure.
/// @default-failure
static inline bool dx__is_utf8_sequence(char const* p, Core_Size n) {
  if (!p) {
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
  char const* current = p;
  char const* end = p + n;
  while (current != end) {
    char const x = *current;
    Core_Size i = _utf8_classify(*current);
    if (i == 0) {
      return false;
    }
    current++;
    // mask second,third,fourth byte with 1100.0000 (0xC0)
    // if the result is 1000.0000 (0x80) then we have a valid sequence.
    for (Core_Size j = 0; j < i - 1; ++j) {
      if (current == end) {
        return false;
      }
      if ((*current & 0xC0) != 0x80) {
        return false;
      }
      current++;
      continue;
    }
  }
  return true;
}

#endif // DX_CORE__IS_UTF8_SEQUENCE
