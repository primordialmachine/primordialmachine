#include "dx/ddl/scanner.h"

#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result set(dx_data_definition_language_scanner* SELF, char const* bytes, dx_size number_of_bytes);

static dx_result step(dx_data_definition_language_scanner* SELF);

static dx_result get_word_text_bytes(char const** RETURN, dx_data_definition_language_scanner* SELF);

static dx_result get_word_text_number_of_bytes(dx_size* RETURN, dx_data_definition_language_scanner* SELF);

static dx_result get_word_start_offset(dx_size* RETURN, dx_data_definition_language_scanner* SELF);

static dx_result get_word_end_offset(dx_size* RETURN, dx_data_definition_language_scanner* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_data_definition_language_scanner_save_and_next(dx_data_definition_language_scanner* SELF);

static bool dx_data_definition_language_scanner_is_digit(dx_data_definition_language_scanner* SELF);

static bool dx_data_definition_language_scanner_is_alphabetic(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_on_single_quoted_string(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_on_double_quoted_string(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_on_name(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_on_number(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_skip_single_line_comment(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_skip_multi_line_comment(dx_data_definition_language_scanner* SELF);

static dx_result dx_data_definition_language_scanner_skip_nls_and_ws(dx_data_definition_language_scanner* SELF);

DX_DEFINE_OBJECT_TYPE("dx.data_definition_language.scanner",
                      dx_data_definition_language_scanner,
                      dx_scanner);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result set(dx_data_definition_language_scanner* SELF, char const* p, dx_size l) {
  SELF->start = p;
  SELF->end = p + l;
  SELF->current = p;
  SELF->kind = dx_ddl_word_kind_start_of_input;
  dx_inline_byte_array_clear(&SELF->text);
  return DX_SUCCESS;
}

static dx_result step(dx_data_definition_language_scanner* SELF) {
START:
  // If the current word is "end of input", return immediatly.
  if (SELF->kind == dx_ddl_word_kind_end_of_input) {
    return DX_SUCCESS;
  }
  // If the current word is "error", return immediatly.
  if (SELF->kind == dx_ddl_word_kind_error) {
    return DX_SUCCESS;
  }
  if (dx_data_definition_language_scanner_skip_nls_and_ws(SELF)) {
    return DX_FAILURE;
  }
  // We have reached the end of the input.
  if (SELF->current == SELF->end) {
    dx_inline_byte_array_clear(&SELF->text);
    SELF->kind = dx_ddl_word_kind_end_of_input;
    return DX_SUCCESS;
  }
  switch (*SELF->current) {
    case '/': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      SELF->current++;
      if (SELF->current == SELF->end) {
        dx_data_definition_language_diagnostics_on_unexpected_symbol(SELF->diagnostics);
        SELF->kind = dx_ddl_word_kind_error;
        SELF->range.end = SELF->current;
        return DX_SUCCESS;
      }
      SELF->range.end = SELF->current;
      if (*SELF->current == '/') {
        SELF->current++;
        if (dx_data_definition_language_scanner_skip_single_line_comment(SELF)) {
          return DX_FAILURE;
        }
        SELF->range.end = SELF->current;
        goto START;
      } else if (*SELF->current == '*') {
        SELF->current++;
        if (dx_data_definition_language_scanner_skip_multi_line_comment(SELF)) {
          return DX_FAILURE;
        }
        SELF->range.end = SELF->current;
        goto START;
      } else {
        if (dx_get_error() == DX_NO_ERROR) {
          dx_set_error(DX_ERROR_LEXICAL_ERROR);
        }
        dx_data_definition_language_diagnostics_on_unexpected_symbol(SELF->diagnostics);
        SELF->kind = dx_ddl_word_kind_error;
        SELF->range.end = SELF->current;
        return DX_SUCCESS;
      }
    } break;
    case ',': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, ",", sizeof(",") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_comma;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case '(': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "(", sizeof("(") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_left_parenthesis;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case ')': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, ")", sizeof(")") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_right_parenthesis;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case '[': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "[", sizeof("[") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_left_square_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case ']': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "]", sizeof("]") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_right_square_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case '{': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "{", sizeof("{") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_left_curly_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case '}': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "}", sizeof("}") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_right_curly_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case ':': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, ":", sizeof(":") - 1)) {
        return DX_FAILURE;
      }
      SELF->kind = dx_ddl_word_kind_colon;
      SELF->current++;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } break;
    case '\'': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      return dx_data_definition_language_scanner_on_single_quoted_string(SELF);
    } break;
    case '"': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      return dx_data_definition_language_scanner_on_double_quoted_string(SELF);
    } break;
    case '0': case '4': case '8':
    case '1': case '5': case '9':
    case '2': case '6':
    case '3': case '7':
    case '+':
    case '-':
    case '.': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      return dx_data_definition_language_scanner_on_number(SELF);
    } break;
    default: {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      return dx_data_definition_language_scanner_on_name(SELF);
    } break;
  };
}

static dx_result get_word_text_bytes(char const** RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->text.elements;
  return DX_SUCCESS;
}

static dx_result get_word_text_number_of_bytes(dx_size* RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->text.size;
  return DX_SUCCESS;
}

static dx_result get_word_start_offset(dx_size* RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->range.start - SELF->start;
  return DX_SUCCESS;
}

static dx_result get_word_end_offset(dx_size* RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->range.end - SELF->start;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_data_definition_language_scanner_save_and_next(dx_data_definition_language_scanner* SELF) {
  if (SELF->current == SELF->end) {
    return DX_FAILURE;
  }
  if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
    return DX_FAILURE;
  }
  SELF->current++;
  return DX_SUCCESS;
}

static bool dx_data_definition_language_scanner_is_digit(dx_data_definition_language_scanner* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return false;
  }
  if (SELF->current == SELF->end) {
    return false;
  }
  return ('0' <= *SELF->current && *SELF->current <= '9');
}

static bool dx_data_definition_language_scanner_is_alphabetic(dx_data_definition_language_scanner* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return false;
  }
  if (SELF->current == SELF->end) {
    return false;
  }
  return ('a' <= *SELF->current && *SELF->current <= 'z')
      || ('A' <= *SELF->current && *SELF->current <= 'Z');
}

static dx_result dx_data_definition_language_scanner_on_single_quoted_string(dx_data_definition_language_scanner* SELF) {
  if (!(SELF->current != SELF->end && *SELF->current == '\'')) {
    dx_set_error(DX_ERROR_LEXICAL_ERROR);
    return DX_FAILURE;
  }
  SELF->current++;
  bool last_was_slash = false;
  while (true) {
    if (SELF->current == SELF->end) {
      dx_data_definition_language_diagnostics_on_unclosed_string_literal(SELF->diagnostics);
      SELF->kind = dx_ddl_word_kind_error;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    } else if (*SELF->current == '\'') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return DX_FAILURE;
        }
      } else {
        SELF->current++;
        SELF->kind = dx_ddl_word_kind_string;
        SELF->range.end = SELF->current;
        return DX_SUCCESS;
      }
    } else if (*SELF->current == '\\') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return DX_FAILURE;
        }
      } else {
        last_was_slash = true;
        SELF->current++;
      }
    } else {
      if (dx_data_definition_language_scanner_save_and_next(SELF)) {
        return DX_FAILURE;
      }
    }
  }
}

static dx_result dx_data_definition_language_scanner_on_double_quoted_string(dx_data_definition_language_scanner* SELF) {
  if (!(SELF->current != SELF->end && *SELF->current == '\"')) {
    dx_set_error(DX_ERROR_LEXICAL_ERROR);
    return DX_FAILURE;
  }
  SELF->current++;
  bool last_was_slash = false;
  while (true) {
    if (SELF->current == SELF->end) {
      // Unclosed string literal error.
      // Expected string contents or closing single quote.
      // Received end of input.
      dx_set_error(DX_ERROR_LEXICAL_ERROR);
      return DX_FAILURE;
    } else if (*SELF->current == '\"') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return DX_FAILURE;
        }
      } else {
        SELF->current++;
        SELF->kind = dx_ddl_word_kind_string;
        SELF->range.end = SELF->current;
        return DX_SUCCESS;
      }
    } else if (*SELF->current == '\\') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return DX_FAILURE;
        }
      } else {
        last_was_slash = true;
        SELF->current++;
      }
    } else {
      if (dx_data_definition_language_scanner_save_and_next(SELF)) {
        return DX_FAILURE;
      }
    }
  }
}

static dx_result dx_data_definition_language_scanner_on_name(dx_data_definition_language_scanner* SELF) {
  while (SELF->current != SELF->end && *SELF->current == '_') {
    if (dx_data_definition_language_scanner_save_and_next(SELF)) {
      return DX_FAILURE;
    }
  }
  if (!dx_data_definition_language_scanner_is_alphabetic(SELF)) {
    if (dx_get_error()) {
      return DX_FAILURE;
    }
    dx_data_definition_language_diagnostics_on_invalid_name(SELF->diagnostics);
    SELF->kind = dx_ddl_word_kind_error;
    SELF->range.end = SELF->current;
    return DX_SUCCESS;
  }
  do {
    if (dx_data_definition_language_scanner_save_and_next(SELF)) {
      return DX_FAILURE;
    }
  } while (dx_data_definition_language_scanner_is_alphabetic(SELF) ||
           (SELF->current != SELF->end && *SELF->current == '_') ||
           dx_data_definition_language_scanner_is_digit(SELF));
  if (dx_get_error()) {
    return DX_FAILURE;
  }
  SELF->kind = dx_ddl_word_kind_name;
  SELF->range.end = SELF->current;
  return DX_SUCCESS;
}

static dx_result dx_data_definition_language_scanner_on_number(dx_data_definition_language_scanner* SELF) {
  // ('+'|'-')?
  if (SELF->current != SELF->end && (*SELF->current == '+' || *SELF->current == '-')) {
    if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
      return DX_FAILURE;
    }
    SELF->current++;
  }
  if (dx_data_definition_language_scanner_is_digit(SELF)) {
    // digit+ ('.' digit*)
    do {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return DX_FAILURE;
      }
      SELF->current++;
    } while (dx_data_definition_language_scanner_is_digit(SELF));
    if (dx_get_error()) {
      return DX_FAILURE;
    }
    if (SELF->current != SELF->end && *SELF->current == '.') {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return DX_FAILURE;
      }
      SELF->current++;
      while (dx_data_definition_language_scanner_is_digit(SELF)) {
        if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
          return DX_FAILURE;
        }
        SELF->current++;
      }
    }
  } else if (SELF->current != SELF->end && *SELF->current == '.') {
    // '.' digit+
    if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
      return DX_FAILURE;
    }
    if (!dx_data_definition_language_scanner_is_digit(SELF)) {
      if (dx_get_error()) {
        return DX_FAILURE;
      }
      dx_data_definition_language_diagnostics_on_invalid_number_literal(SELF->diagnostics);
      SELF->kind = dx_ddl_word_kind_error;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    }
    do {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return DX_FAILURE;
      }
      SELF->current++;
    } while (dx_data_definition_language_scanner_is_digit(SELF));
    if (dx_get_error()) {
      return DX_FAILURE;
    }
  } else {
    dx_set_error(DX_ERROR_LEXICAL_ERROR);
    return DX_FAILURE;
  }
  // exponent?
  // exponent = 'e'('+'|'-')? digit+
  if (SELF->current != SELF->end && *SELF->current == 'e') {
    if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
      return DX_FAILURE;
    }
    if (!dx_data_definition_language_scanner_is_digit(SELF)) {
      if (dx_get_error()) {
        return DX_FAILURE;
      }
      dx_data_definition_language_diagnostics_on_invalid_number_literal(SELF->diagnostics);
      SELF->kind = dx_ddl_word_kind_error;
      SELF->range.end = SELF->current;
      return DX_SUCCESS;
    }
    do {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return DX_FAILURE;
      }
    } while (dx_data_definition_language_scanner_is_digit(SELF));
    if (dx_get_error()) {
      return DX_FAILURE;
    }
  }
  SELF->kind = dx_ddl_word_kind_number;
  SELF->range.end = SELF->current;
  return DX_SUCCESS;
}

static void dx_data_definition_language_scanner_destruct(dx_data_definition_language_scanner* SELF) {
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
  dx_inline_byte_array_uninitialize(&SELF->text);
}

static void dx_data_definition_language_scanner_dispatch_construct(dx_data_definition_language_scanner_dispatch* SELF) {
  DX_SCANNER_DISPATCH(SELF)->get_word_end_offset = (dx_result(*)(dx_size*,dx_scanner*)) & get_word_end_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_start_offset = (dx_result(*)(dx_size*, dx_scanner*)) & get_word_start_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_bytes = (dx_result(*)(char const**, dx_scanner*)) & get_word_text_bytes;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_number_of_bytes = (dx_result(*)(dx_size*, dx_scanner*)) & get_word_text_number_of_bytes;
  DX_SCANNER_DISPATCH(SELF)->set = (dx_result(*)(dx_scanner*, char const*, dx_size)) & set;
  DX_SCANNER_DISPATCH(SELF)->step = (dx_result(*)(dx_scanner*)) & step;
}

dx_result dx_data_definition_language_scanner_construct(dx_data_definition_language_scanner* SELF, dx_data_definition_language_diagnostics* diagnostics) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_data_definition_language_scanner_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_scanner_construct(DX_SCANNER(SELF))) {
    return DX_FAILURE;
  }

  if (dx_inline_byte_array_initialize(&SELF->text)) {
    return DX_FAILURE;
  } 
  if (!diagnostics) {
    dx_inline_byte_array_uninitialize(&SELF->text);
    return DX_FAILURE;
  }
  SELF->diagnostics = diagnostics;
  DX_REFERENCE(SELF->diagnostics);

  static char const EMPTY[] = "";
  SELF->start = &EMPTY[0];
  SELF->end = SELF->start;
  SELF->current = SELF->start;
  SELF->kind = dx_ddl_word_kind_start_of_input;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static dx_result dx_data_definition_language_scanner_skip_single_line_comment(dx_data_definition_language_scanner* SELF) {
  while (!(SELF->current == SELF->end)
      && !(SELF->current != SELF->end && *SELF->current == '\n')
      && !(SELF->current != SELF->end && *SELF->current == '\r')) {
    SELF->current++;
  }
  return DX_SUCCESS;
}

static dx_result dx_data_definition_language_scanner_skip_multi_line_comment(dx_data_definition_language_scanner* SELF) {
  while (true) {
    if (SELF->current == SELF->end) {
      dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(SELF->diagnostics);
      dx_set_error(DX_ERROR_LEXICAL_ERROR);
      return DX_FAILURE;
    } else if (*SELF->current == '*') {
      SELF->current++;
      if (SELF->current == SELF->end) {
        dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(SELF->diagnostics);
        SELF->kind = dx_ddl_word_kind_error;
        SELF->range.end = SELF->current;
        return DX_SUCCESS;
      }
      if (*SELF->current == '/') {
        SELF->current++;
        return DX_SUCCESS;
      } else {
        continue;
      }
    } else {
      SELF->current++;
    }
  }
  return DX_SUCCESS;
}

static dx_result dx_data_definition_language_scanner_skip_nls_and_ws(dx_data_definition_language_scanner* SELF) {
  bool stop;
  do {
    stop = true;
    // Skip whitespace.
    if ((*SELF->current == ' ' || *SELF->current == '\t') && SELF->current != SELF->end) {
      stop = false;
      do {
        SELF->current++;
      } while ((*SELF->current == ' ' || *SELF->current == '\t') && SELF->current != SELF->end);
    }
    // Skip newlines.
    while ((*SELF->current == '\n' || *SELF->current == '\r') && SELF->current != SELF->end) {
      stop = false;
      char old = *SELF->current;
      SELF->current++;
      if ((*SELF->current == '\n' || *SELF->current == '\r') && *SELF->current != old && SELF->current != SELF->end) {
        SELF->current++;
      }
    }
  } while (!stop);
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_scanner_create(dx_data_definition_language_scanner** RETURN, dx_data_definition_language_diagnostics* diagnostics) {
  dx_data_definition_language_scanner* SELF = DX_DATA_DEFINITION_LANGUAGE_SCANNER(dx_object_alloc(sizeof(dx_data_definition_language_scanner)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_data_definition_language_scanner_construct(SELF, diagnostics)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_scanner_get_word_kind(dx_ddl_word_kind* RETURN, dx_data_definition_language_scanner const* SELF) {
  *RETURN = SELF->kind;
  return DX_SUCCESS;
}
