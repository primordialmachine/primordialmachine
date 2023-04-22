#include "dx/core/_string_format.h"

// memcpy
#include <string.h>
// snprintf
#include <stdio.h>

#include "dx/core/string.h"

#define PRINT_PRECISION_8 (8)
#define PRINT_PRECISION_16 (16)
#define PRINT_PRECISION_32 (32)
#define PRINT_PRECISION_64 (64)

#define PRINT_ERROR (0)

#define PRINT_TYPE_I (1)
#define PRINT_TYPE_N (2)
#define PRINT_TYPE_S (3)
#define PRINT_TYPE_R (4)

#define PRINT_R32 (PRINT_TYPE_R | PRINT_PRECISION_32)
#define PRINT_R64 (PRINT_TYPE_R | PRINT_PRECISION_64)

#define PRINT_I16 (PRINT_TYPE_I | PRINT_PRECISION_16)
#define PRINT_I32 (PRINT_TYPE_I | PRINT_PRECISION_32)
#define PRINT_I64 (PRINT_TYPE_I | PRINT_PRECISION_64)
#define PRINT_I8  (PRINT_TYPE_I | PRINT_PRECISION_8)

#define PRINT_N16 (PRINT_TYPE_N | PRINT_PRECISION_16)
#define PRINT_N32 (PRINT_TYPE_N | PRINT_PRECISION_32)
#define PRINT_N64 (PRINT_TYPE_N | PRINT_PRECISION_64)
#define PRINT_N8  (PRINT_TYPE_N | PRINT_PRECISION_8)

#define PRINT_STRING (1024)
#define PRINT_DOLLAR (1025)

static bool _is_digit(char x) {
  return '0' <= x && x <= '9';
}

#define IS(SYMBOL) ((**start) == SYMBOL)

/// @param RETURN A pointer to a <code>int</code> variable.
/// @param start The start of this parse.
/// @param end The end of this parse.
/// @return DX_SUCCESS on success, DX_FAILURE on failure.
/// @success <code>*RETURN</code> was assigned the number of significands.
static dx_result parse_fractional_digits_specifier(uint8_t* RETURN, char const** start, char const* end);

/// @param RETURN A pointer to a <code>int</code> variable.
/// @param start The start of this parse.
/// @param end The end of this parse.
/// @return DX_SUCCESS on success, DX_FAILURE on failure.
/// @success <code>*RETURN</code> was assigned the precision.
static dx_result parse_type_specifier_bits(uint8_t* RETURN, char const** start, char const* end);

static dx_result parse_fractional_digits_specifier(uint8_t* RETURN, char const** start, char const* end) {
  // '.'
  if ((*start) == end) {
    dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
    return DX_FAILURE;
  }
  if (!IS('.')) {
    dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
    return DX_FAILURE;
  }
  (*start)++;

  // digit
  uint8_t temporary = 0;
  if ((*start) == end) {
    dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
    return DX_FAILURE;
  }
  if (!_is_digit(**start)) {
    dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
    return DX_FAILURE;
  }
  temporary += (uint8_t)(**start) - (uint8_t)'0';
  (*start)++;

  // digit*
  while ((*start) != end && _is_digit(**start)) {
    temporary *= 10;
    temporary += (uint8_t)(**start) - (uint8_t)'0';
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}


static dx_result parse_type_specifier_bits(uint8_t* RETURN, char const** start, char const* end) {
  if ((*start) == end) {
    dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
    return DX_FAILURE;
  }
  
  if (IS('8')) {
    *RETURN = PRINT_PRECISION_8;
    return DX_SUCCESS;
  } else if ((**start) == '1') {
    (*start)++;
    if ((*start) == end) {
      dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
      return DX_FAILURE;
    }
    if (!IS('6')) {
      dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
      return DX_FAILURE;
    }
    *RETURN = PRINT_PRECISION_16;
    return DX_SUCCESS;
  } else if (IS('3')) {
    (*start)++;
    if ((*start) == end) {
      dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
      return DX_FAILURE;
    }
    if (!IS('2')) {
      dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
      return DX_FAILURE;
    }
    *RETURN = PRINT_PRECISION_32;
    return DX_SUCCESS;
  } else if (IS('6')) {
    (*start)++;
    if ((*start) == end) {
      dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
      return DX_FAILURE;
    }
    if (!IS('4')) {
      dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
      return DX_FAILURE;
    }
    *RETURN = PRINT_PRECISION_64;
    return DX_SUCCESS;
  } else {
    dx_set_error(DX_ERROR_SYNTACTICAL_ERROR);
    return DX_FAILURE;
  }
}

typedef struct format_spec_t {
  // PRINT_TYPE_I|PRINT_TYPE_PRECISION_(8|16|32|64)
  // PRINT_TYPE_N|PRINT_TYPE_PRECISION_(8|16|32|64)
  // PRINT_TYPE_R|PRINT_TYPE_PRECISION_(32|64)
  // PRINT_ERROR
  // PRINT_DOLLAR
  int type;
  // number of fractional digitis
  // only valif for PRINT_TYPE_R(32|64)
  uint8_t fractional_digits;
} format_spec_t;

static int parse(format_spec_t* format_spec, char const** start, char const* end) {
  assert(IS('$'));
  
  (*start)++;
  if ((*start) == end) {
    format_spec->type = PRINT_ERROR;
    return 0;
  }
  
  if (IS('$')) {
    format_spec->type = PRINT_DOLLAR;
    return 0;
  }
  if (!IS('{')) {
    format_spec->type = PRINT_ERROR;
    return 0;
  }

  (*start)++;
  if ((*start) == end) {
    format_spec->type = PRINT_ERROR;
    return 0;
  }
  if (IS('i')) {
    (*start)++;
    int k;
    uint8_t type_specifier_bits;
    if (parse_type_specifier_bits(&type_specifier_bits, start, end)) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    k = PRINT_TYPE_I | type_specifier_bits;
    (*start)++;
    if ((*start) == end) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    if (!IS('}')) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    (*start)++;
    format_spec->type = k;
    return 0;
  } else if (IS('n')) {
    (*start)++;
    int k;
    uint8_t type_specifier_bits;
    if (parse_type_specifier_bits(&type_specifier_bits, start, end)) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    k = PRINT_TYPE_N | type_specifier_bits;
    (*start)++;
    if ((*start) == end) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    if (!IS('}')) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    (*start)++;
    format_spec->type = k;
    return 0;
  } else if (IS('r')) {
    (*start)++;
    int k;
    uint8_t type_specifier_bits;
    if (parse_type_specifier_bits(&type_specifier_bits, start, end)) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    if (type_specifier_bits != PRINT_PRECISION_32 && type_specifier_bits != PRINT_PRECISION_64) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    k = PRINT_TYPE_R | type_specifier_bits;
    (*start)++;
    //
    if ((*start) == end) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    format_spec->fractional_digits = 14; // default value
    if (IS('.')) {
      if (parse_fractional_digits_specifier(&format_spec->fractional_digits, start, end)) {
        format_spec->type = PRINT_ERROR;
        return 0;
      }
    }
    //
    if ((*start) == end) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    if (!IS('}')) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    (*start)++;

    format_spec->type = k;
    return 0;
  } else if (IS('s')) {
    (*start)++;
    if ((*start) == end) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    if (!IS('}')) {
      format_spec->type = PRINT_ERROR;
      return 0;
    }
    (*start)++;
    format_spec->type = PRINT_STRING;
    return 0;
  } else {
    format_spec->type = PRINT_ERROR;
    return 0;
  }
}

#undef IS

int dx__format_v(dx_inline_byte_array* buffer, char const* start, char const* end, va_list arguments) {
  char const* current = start;
  while (current != end) {
    if (*current == '$') {
      // We encountered a format symbol. Store all the bytes up to and excluding the format symbol in the buffer.
      if (dx_inline_byte_array_append(buffer, start, current - start)) {
        return 1;
      }
      format_spec_t format_spec;
      parse(&format_spec, &current, end);
      if (format_spec.type == PRINT_ERROR) {
        dx_set_error(DX_ERROR_INVALID_ARGUMENT);
        return 1;
      }
      switch (format_spec.type) {
      case PRINT_STRING: {
        dx_string* argument = va_arg(arguments, dx_string*);
        if (!argument) {
          dx_set_error(DX_ERROR_INVALID_ARGUMENT);
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, argument->bytes, argument->number_of_bytes)) {
          return 1;
        }
      } break;
      case PRINT_I8: {
        dx_i8 argument = va_arg(arguments, dx_i8);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi8, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_I16: {
        dx_i16 argument = va_arg(arguments, dx_i16);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi16, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_I32: {
        dx_i32 argument = va_arg(arguments, dx_i32);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi32, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_I64: {
        dx_i64 argument = va_arg(arguments, dx_i64);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi64, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N8: {
        dx_n8 argument = va_arg(arguments, dx_n8);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu8, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N16: {
        dx_n16 argument = va_arg(arguments, dx_n16);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu16, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N32: {
        dx_n32 argument = va_arg(arguments, dx_n32);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu32, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N64: {
        dx_n64 argument = va_arg(arguments, dx_n64);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu64, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_R32: {
        dx_f32 argument = va_arg(arguments, dx_f32);
        char temporary[44];
        int result = snprintf(temporary, 44, "%.*f", (int)format_spec.fractional_digits, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_R64: {
        dx_f64 argument = va_arg(arguments, dx_f64);
        char temporary[44];
        int result = snprintf(temporary, 44, "%.*lf", (int)format_spec.fractional_digits, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_inline_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_DOLLAR: {
        static char const bytes[] = "$";
        if (dx_inline_byte_array_append(buffer, bytes, sizeof(bytes) - 1)) {
          return 1;
        }
      } break;
      default: {
        // Expected: Format specifier. Received: Unknown format specifier prefix.
        dx_set_error(DX_ERROR_INVALID_ARGUMENT);
        return 1;
      } break;
      };
      start = current;
    } else {
      current++;
    }
  }
  if (start != current) {
    if (dx_inline_byte_array_append(buffer, start, current - start)) {
      return 1;
    }
  }
  return 0;
}
