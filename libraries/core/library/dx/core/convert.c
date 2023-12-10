#include "dx/core/convert.h"

// atod, atof
#include <stdlib.h>
// memcpy
#include <memory.h>

/// @brief Get if the current symbol is a digit.
/// @param current, end The parser.
/// @return @a true if the current symbol is a digit. @a false otherwise. 
static inline bool is_digit(char const* current, char const* end);

/// @brief Parse a floating point literal.
/// @param p A pointer to the bytes. 
/// @param n The number of Bytes.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
static int dx_parse_flit(char const* p, Core_Size n);

static inline bool is_digit(char const* current, char const* end) {
  return (current != end)
      && ('0' <= *current && *current <= '9');
}

static int dx_parse_flit(char const* p, Core_Size n) {
  char const* start = p;
  char const* end = p + n;
  char const* current = p;

  if (current != end) {
    if (*current == '-') {
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (is_digit(current, end)) {
    do {
      current++;
    } while (is_digit(current, end));
    if (Core_getError()) {
      return Core_Failure;
    }
    if (current != end && *current == '.') {
      current++;
      while (is_digit(current, end)) {
        current++;
      }
    }
  } else if (current != end && *current == '.') {
    current++;
    if (!is_digit(current, end)) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    do {
      current++;
    } while (is_digit(current, end));
    if (Core_getError()) {
      return Core_Failure;
    }
  } else {
    return Core_Failure;
  }
  if (current != end && (*current == 'e' || *current == 'E')) {
    current++;
    if (current != end && (*current == '+' || *current == '-')) {
      current++;
    }
    if (!is_digit(current, end)) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    do {
      current++;
    } while (is_digit(current, end));
    if (Core_getError()) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_i8(char const* p, Core_Size n, Core_Integer8* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Integer8 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  if (negative) {
    do {
      //
      static const Core_Integer8 BASE = 10;
      static const Core_Integer8 MIN1 = Core_Integer8_Least + 8; // -128 / 10 = -12.8 = -12.
                                                                 // -12 * 10 = -120  is no overflow.
      if (value < MIN1) {
        // A multiplication of a value smaller than MIN1 by 10 would cause an overflow.
        // Hence, the conversion must fail.
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static const Core_Integer8 MIN2 = Core_Integer8_Least;
      Core_Integer8 x = *current - '0';
      if (value < MIN2 + x) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const Core_Integer8 BASE = 10;
      static const Core_Integer8 MAX1 = Core_Integer8_Greatest - 7; // 127 / 10 = 12.7 = 12.
                                                                    // 12 * 10 = 120 is no overflow.
      if (value > MAX1) {
        // A multiplication of a value greater than MAX1 by 10 would cause an overflow.
      // Hence the conversion must fail.
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static const Core_Integer8 MAX2 = Core_Integer8_Greatest;
      Core_Integer8 x = *current - '0';
      if (MAX2 - x < value) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_n8(char const* p, Core_Size n, Core_Natural8* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Natural8 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  do {
    //
    static const Core_Natural8 BASE = 10;
    static const Core_Natural8 MAX1 = Core_Natural8_Greatest - 5; // 255 / 10 = 25.5 = 25.
                                                                  // 25 * 10 = 250 is no overflow.
    if (value > MAX1) {
      // A multiplication of a value greater than MAX1 by 10 would cause an overflow.
      // Hence the conversion must fail.
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value = value * BASE;
    //
    static const Core_Natural8 MAX2 = Core_Natural8_Greatest;
    Core_Natural8 x = *current - '0';
    if (MAX2 - x < value) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_i16(char const* p, Core_Size n, Core_Integer16* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Integer16 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  if (negative) {
    do {
      //
      static const Core_Integer16 BASE = 10;
      static const Core_Integer16 MIN1 = Core_Integer16_Least + 8; // -32768 / 10 = 3276.8 = 3276.
                                                                   // -3276 * 10 = 32760 is no overflow.
      if (value < MIN1) {
        // A multiplication of a value smaller than MIN1 by 10 would cause an overflow.
        // Hence, the conversion must fail.
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static const Core_Integer16 MIN2 = Core_Integer16_Least;
      Core_Integer16 x = *current - '0';
      if (value < MIN2 + x) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const Core_Integer16 BASE = 10;
      static const Core_Integer16 MAX1 = Core_Integer16_Greatest - 7; // 32767 / 10 = 3276.7 = 3276.
                                                                      // 3276 * 10 = 32760 is no overflow.
      if (value > MAX1) {
        // A multiplication of a value greater than MAX1 by 10 would cause an overflow.
        // Hence the conversion must fail.
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static const Core_Integer16 MAX2 = Core_Integer16_Greatest;
      Core_Integer16 x = *current - '0';
      if (MAX2 - x < value) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_n16(char const* p, Core_Size n, Core_Natural16* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Natural16 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  do {
    //
    static const Core_Natural16 BASE = 10;
    static const Core_Natural16 MAX1 = Core_Natural16_Greatest - 5; // 65535 / 10 = 6553.5 = 6553.
                                                                    // 6553*10 = 65530 is no overflow.
    if (value > MAX1) {
      // A multiplication of a value greater than MAX1 by 10 would cause an overflow.
      // Hence the conversion must fail.
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value = value * BASE;
    //
    static const Core_Natural16 MAX2 = Core_Natural16_Greatest;
    Core_Natural16 x = *current - '0';
    if (MAX2 - x < value) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_i32(char const* p, Core_Size n, Core_Integer32* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Integer32 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  if (negative) {
    do {
      //
      static const Core_Integer32 BASE = 10;
      static const Core_Integer32 MIN1 = Core_Integer32_Least + 8; // -2147483648 / 10 = -214748364.8 = -214748364.
                                                                   // -214748364 * 10 = -2147483640 is no overflow.
      if (value < MIN1) {
        // A multiplication of a value smaller than MIN1 by 10 would cause an overflow.
        // Hence, the conversion must fail.
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static const Core_Integer32 MIN2 = Core_Integer32_Least;
      Core_Integer32 x = *current - '0';
      if (value < MIN2 + x) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const Core_Integer32 BASE = 10;
      static const Core_Integer32 MAX1 = Core_Integer32_Greatest - 7; // 2147483647 / 10 = 214748364.7 = 214748364.
                                                                      // 214748364 * 10 = 2147483640 is no overflow.
      if (value > MAX1) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static const Core_Integer32 MAX2 = Core_Integer32_Greatest;
      Core_Integer32 x = *current - '0';
      if (MAX2 - x < value) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_n32(char const* p, Core_Size n, Core_Natural32* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Natural32 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  do {
    //
    static const Core_Natural32 BASE = 10;
    static const Core_Natural32 MAX1 = Core_Natural32_Greatest - 5; // 4294967295 / 10 = 429496729.5 = 429496729.
                                                                    // 429496729 * 10 = 4294967290 is no overflow.
    if (value == MAX1) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value = value * BASE;
    //
    static const uint32_t MAX2 = Core_Natural32_Greatest;
    Core_Natural32 x = *current - '0';
    if (MAX2 - x < value) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_i64(char const* p, Core_Size n, Core_Integer64* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Integer64 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  if (negative) {
    do {
      //
      static Core_Integer64 const BASE = 10;
      static Core_Integer64 const MIN1 = Core_Integer64_Least + 8; // -9223372036854775808 / 10 = -922337203685477580.8 = -922337203685477580.
                                                                   // -922337203685477580 * 10 = -9223372036854775800 is no overflow.
      if (value == MIN1) {
        // A multiplication of MIN1 by 10 would cause an overflow.
        // Hence, the conversion must fail.
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static Core_Integer64 const MIN2 = Core_Integer64_Least;
      Core_Integer64 x = *current - '0';
      if (value < MIN2 + x) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static Core_Integer64 const BASE = 10;
      static Core_Integer64 const MAX1 = Core_Integer64_Greatest - 7; // 9223372036854775807 / 10 = 922337203685477580.7 = 922337203685477580.
                                                                      // 922337203685477580 * 10 = 9223372036854775800 is no overflow.
      if (value == MAX1) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value = value * BASE;
      //
      static Core_Integer64 const MAX2 = Core_Integer64_Greatest;
      Core_Integer64 x = *current - '0';
      if (MAX2 - x < value) {
        Core_setError(Core_Error_ConversionFailed);
        return Core_Failure;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_n64(char const* p, Core_Size n, Core_Natural64* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  Core_Natural64 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  do {
    //
    static Core_Natural64 const BASE = 10;
    static Core_Natural64 const MAX1 = Core_Natural64_Greatest - 5; // 18446744073709551615	/ 10 = 1844674407370955161.5 = 1844674407370955161.
                                                                    // 1844674407370955161 * 10 = 18446744073709551610 is no overflow.
    if (value == MAX1) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value = value * BASE;
    //
    static Core_Natural64 const MAX2 = Core_Natural64_Greatest;
    Core_Natural64 x = *current - '0';
    if (MAX2 - x < value) {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_sz(char const* p, Core_Size n, Core_Size* target) {
#if defined(_M_X64)
  return dx_convert_utf8bytes_to_n64(p, n, target);
#elif defined(_M_IX86)
  return dx_convert_utf8bytes_to_n32(p, n, target);
#else
  #error("environment not supported")
#endif
}


Core_Result dx_convert_utf8bytes_to_f32(char const* p, Core_Size n, Core_Real32* target) {
  if (dx_parse_flit(p, n)) {
    return Core_Failure;
  }
#define N_MAX 200
  if (n > N_MAX) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  errno = 0;
  char buffer[N_MAX + 1];
  memcpy(buffer, p, n);
  buffer[n] = '\0';
  char* end;
  double v = strtof(buffer, &end);
  if (end != &buffer[n]) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  if (errno) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
#undef N_MAX
  *target = v;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_f64(char const* p, Core_Size n, Core_Real64* target) {
  if (dx_parse_flit(p, n)) {
    return Core_Failure;
  }
#define N_MAX 200
  if (n > N_MAX) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  errno = 0;
  char buffer[N_MAX + 1];
  memcpy(buffer, p, n);
  buffer[n] = '\0';
  char* end;
  double v = strtod(buffer, &end);
  if (end != &buffer[n]) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
  if (errno) {
    Core_setError(Core_Error_ConversionFailed);
    return Core_Failure;
  }
#undef N_MAX
  *target = v;
  return Core_Success;
}

Core_Result dx_convert_utf8bytes_to_bool(char const* p, Core_Size n, Core_Boolean* target) {
  if (!p || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  static_assert(sizeof("true") != sizeof("false"), "environment not supported");
  switch (n) {
    case sizeof("true") - sizeof(char) : {
      *target = !memcmp(p, "true", n);
      return Core_Success;
    } break;
    case sizeof("false") - sizeof(char) : {
      *target = !memcmp(p, "false", n);
      return Core_Success;
    } break;
    default: {
      Core_setError(Core_Error_ConversionFailed);
      return Core_Failure;
    } break;
  };
}
