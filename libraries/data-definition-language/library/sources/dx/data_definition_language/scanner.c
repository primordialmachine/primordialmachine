#include "dx/data_definition_language/scanner.h"

#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result set(dx_data_definition_language_scanner* SELF, char const* bytes, Core_Size number_of_bytes);

static Core_Result step(dx_data_definition_language_scanner* SELF);

static Core_Result get_word_text_bytes(char const** RETURN, dx_data_definition_language_scanner* SELF);

static Core_Result get_word_text_number_of_bytes(Core_Size* RETURN, dx_data_definition_language_scanner* SELF);

static Core_Result get_word_start_offset(Core_Size* RETURN, dx_data_definition_language_scanner* SELF);

static Core_Result get_word_end_offset(Core_Size* RETURN, dx_data_definition_language_scanner* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_data_definition_language_scanner_save_and_next(dx_data_definition_language_scanner* SELF);

static bool dx_data_definition_language_scanner_is_digit(dx_data_definition_language_scanner* SELF);

static bool dx_data_definition_language_scanner_is_alphabetic(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_on_single_quoted_string(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_on_double_quoted_string(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_on_name(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_on_number(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_skip_single_line_comment(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_skip_multi_line_comment(dx_data_definition_language_scanner* SELF);

static Core_Result dx_data_definition_language_scanner_skip_nls_and_ws(dx_data_definition_language_scanner* SELF);

DX_DEFINE_OBJECT_TYPE("dx.data_definition_language.scanner",
                      dx_data_definition_language_scanner,
                      dx_scanner);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result set(dx_data_definition_language_scanner* SELF, char const* p, Core_Size l) {
  SELF->start = p;
  SELF->end = p + l;
  SELF->current = p;
  SELF->kind = dx_data_definition_language_word_kind_start_of_input;
  dx_inline_byte_array_clear(&SELF->text);
  return Core_Success;
}

static Core_Result step(dx_data_definition_language_scanner* SELF) {
START:
  // If the current word is "end of input", return immediatly.
  if (SELF->kind == dx_data_definition_language_word_kind_end_of_input) {
    return Core_Success;
  }
  // If the current word is "error", return immediatly.
  if (SELF->kind == dx_data_definition_language_word_kind_error) {
    return Core_Success;
  }
  if (dx_data_definition_language_scanner_skip_nls_and_ws(SELF)) {
    return Core_Failure;
  }
  // We have reached the end of the input.
  if (SELF->current == SELF->end) {
    dx_inline_byte_array_clear(&SELF->text);
    SELF->kind = dx_data_definition_language_word_kind_end_of_input;
    return Core_Success;
  }
  switch (*SELF->current) {
    case '/': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      SELF->current++;
      if (SELF->current == SELF->end) {
        dx_data_definition_language_diagnostics_on_unexpected_symbol(SELF->diagnostics, SELF->current - SELF->start);
        SELF->kind = dx_data_definition_language_word_kind_error;
        SELF->range.end = SELF->current;
        return Core_Success;
      }
      SELF->range.end = SELF->current;
      if (*SELF->current == '/') {
        SELF->current++;
        if (dx_data_definition_language_scanner_skip_single_line_comment(SELF)) {
          return Core_Failure;
        }
        SELF->range.end = SELF->current;
        goto START;
      } else if (*SELF->current == '*') {
        SELF->current++;
        if (dx_data_definition_language_scanner_skip_multi_line_comment(SELF)) {
          return Core_Failure;
        }
        SELF->range.end = SELF->current;
        goto START;
      } else {
        if (Core_Error_NoError == Core_getError()) {
          Core_setError(Core_Error_LexicalAnalysisFailed);
        }
        dx_data_definition_language_diagnostics_on_unexpected_symbol(SELF->diagnostics, SELF->current - SELF->start);
        SELF->kind = dx_data_definition_language_word_kind_error;
        SELF->range.end = SELF->current;
        return Core_Success;
      }
    } break;
    case ',': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, ",", sizeof(",") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_comma;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case '(': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "(", sizeof("(") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_left_parenthesis;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case ')': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, ")", sizeof(")") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_right_parenthesis;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case '[': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "[", sizeof("[") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_left_square_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case ']': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "]", sizeof("]") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_right_square_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case '{': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "{", sizeof("{") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_left_curly_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case '}': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, "}", sizeof("}") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_right_curly_bracket;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
    } break;
    case ':': {
      SELF->range.start = SELF->current;
      SELF->range.end = SELF->current;
      dx_inline_byte_array_clear(&SELF->text);
      if (dx_inline_byte_array_append(&SELF->text, ":", sizeof(":") - 1)) {
        return Core_Failure;
      }
      SELF->kind = dx_data_definition_language_word_kind_colon;
      SELF->current++;
      SELF->range.end = SELF->current;
      return Core_Success;
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

static Core_Result get_word_text_bytes(char const** RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->text.elements;
  return Core_Success;
}

static Core_Result get_word_text_number_of_bytes(Core_Size* RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->text.size;
  return Core_Success;
}

static Core_Result get_word_start_offset(Core_Size* RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->current - SELF->start;
  return Core_Success;
}

static Core_Result get_word_end_offset(Core_Size* RETURN, dx_data_definition_language_scanner* SELF) {
  *RETURN = SELF->range.end - SELF->start;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_data_definition_language_scanner_save_and_next(dx_data_definition_language_scanner* SELF) {
  if (SELF->current == SELF->end) {
    return Core_Failure;
  }
  if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
    return Core_Failure;
  }
  SELF->current++;
  return Core_Success;
}

static bool dx_data_definition_language_scanner_is_digit(dx_data_definition_language_scanner* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
  if (SELF->current == SELF->end) {
    return false;
  }
  return ('0' <= *SELF->current && *SELF->current <= '9');
}

static bool dx_data_definition_language_scanner_is_alphabetic(dx_data_definition_language_scanner* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
  if (SELF->current == SELF->end) {
    return false;
  }
  return ('a' <= *SELF->current && *SELF->current <= 'z')
      || ('A' <= *SELF->current && *SELF->current <= 'Z');
}

static Core_Result dx_data_definition_language_scanner_on_single_quoted_string(dx_data_definition_language_scanner* SELF) {
  if (!(SELF->current != SELF->end && *SELF->current == '\'')) {
    Core_setError(Core_Error_LexicalAnalysisFailed);
    return Core_Failure;
  }
  SELF->current++;
  bool last_was_slash = false;
  while (true) {
    if (SELF->current == SELF->end) {
      dx_data_definition_language_diagnostics_on_unclosed_string_literal(SELF->diagnostics, SELF->current - SELF->start);
      SELF->kind = dx_data_definition_language_word_kind_error;
      SELF->range.end = SELF->current;
      return Core_Success;
    } else if (*SELF->current == '\'') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return Core_Failure;
        }
      } else {
        SELF->current++;
        SELF->kind = dx_data_definition_language_word_kind_string;
        SELF->range.end = SELF->current;
        return Core_Success;
      }
    } else if (*SELF->current == '\\') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return Core_Failure;
        }
      } else {
        last_was_slash = true;
        SELF->current++;
      }
    } else {
      if (dx_data_definition_language_scanner_save_and_next(SELF)) {
        return Core_Failure;
      }
    }
  }
}

static Core_Result dx_data_definition_language_scanner_on_double_quoted_string(dx_data_definition_language_scanner* SELF) {
  if (!(SELF->current != SELF->end && *SELF->current == '\"')) {
    Core_setError(Core_Error_LexicalAnalysisFailed);
    return Core_Failure;
  }
  SELF->current++;
  bool last_was_slash = false;
  while (true) {
    if (SELF->current == SELF->end) {
      // Unclosed string literal error.
      // Expected string contents or closing single quote.
      // Received end of input.
      Core_setError(Core_Error_LexicalAnalysisFailed);
      return Core_Failure;
    } else if (*SELF->current == '\"') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return Core_Failure;
        }
      } else {
        SELF->current++;
        SELF->kind = dx_data_definition_language_word_kind_string;
        SELF->range.end = SELF->current;
        return Core_Success;
      }
    } else if (*SELF->current == '\\') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_data_definition_language_scanner_save_and_next(SELF)) {
          return Core_Failure;
        }
      } else {
        last_was_slash = true;
        SELF->current++;
      }
    } else {
      if (dx_data_definition_language_scanner_save_and_next(SELF)) {
        return Core_Failure;
      }
    }
  }
}

static Core_Result dx_data_definition_language_scanner_on_name(dx_data_definition_language_scanner* SELF) {
  while (SELF->current != SELF->end && *SELF->current == '_') {
    if (dx_data_definition_language_scanner_save_and_next(SELF)) {
      return Core_Failure;
    }
  }
  if (!dx_data_definition_language_scanner_is_alphabetic(SELF)) {
    if (Core_getError()) {
      return Core_Failure;
    }
    dx_data_definition_language_diagnostics_on_invalid_name(SELF->diagnostics, SELF->current - SELF->start);
    SELF->kind = dx_data_definition_language_word_kind_error;
    SELF->range.end = SELF->current;
    return Core_Success;
  }
  do {
    if (dx_data_definition_language_scanner_save_and_next(SELF)) {
      return Core_Failure;
    }
  } while (dx_data_definition_language_scanner_is_alphabetic(SELF) ||
           (SELF->current != SELF->end && *SELF->current == '_') ||
           dx_data_definition_language_scanner_is_digit(SELF));
  if (Core_getError()) {
    return Core_Failure;
  }
  SELF->kind = dx_data_definition_language_word_kind_name;
  SELF->range.end = SELF->current;
  return Core_Success;
}

static Core_Result dx_data_definition_language_scanner_on_number(dx_data_definition_language_scanner* SELF) {
  // ('+'|'-')?
  if (SELF->current != SELF->end && (*SELF->current == '+' || *SELF->current == '-')) {
    if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
      return Core_Failure;
    }
    SELF->current++;
  }
  if (dx_data_definition_language_scanner_is_digit(SELF)) {
    // digit+ ('.' digit*)
    do {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return Core_Failure;
      }
      SELF->current++;
    } while (dx_data_definition_language_scanner_is_digit(SELF));
    if (Core_getError()) {
      return Core_Failure;
    }
    if (SELF->current != SELF->end && *SELF->current == '.') {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return Core_Failure;
      }
      SELF->current++;
      while (dx_data_definition_language_scanner_is_digit(SELF)) {
        if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
          return Core_Failure;
        }
        SELF->current++;
      }
    }
  } else if (SELF->current != SELF->end && *SELF->current == '.') {
    // '.' digit+
    if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
      return Core_Failure;
    }
    if (!dx_data_definition_language_scanner_is_digit(SELF)) {
      if (Core_getError()) {
        return Core_Failure;
      }
      dx_data_definition_language_diagnostics_on_invalid_number_literal(SELF->diagnostics, SELF->current - SELF->start);
      SELF->kind = dx_data_definition_language_word_kind_error;
      SELF->range.end = SELF->current;
      return Core_Success;
    }
    do {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return Core_Failure;
      }
      SELF->current++;
    } while (dx_data_definition_language_scanner_is_digit(SELF));
    if (Core_getError()) {
      return Core_Failure;
    }
  } else {
    Core_setError(Core_Error_LexicalAnalysisFailed);
    return Core_Failure;
  }
  // exponent?
  // exponent = 'e'('+'|'-')? digit+
  if (SELF->current != SELF->end && *SELF->current == 'e') {
    if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
      return Core_Failure;
    }
    if (!dx_data_definition_language_scanner_is_digit(SELF)) {
      if (Core_getError()) {
        return Core_Failure;
      }
      dx_data_definition_language_diagnostics_on_invalid_number_literal(SELF->diagnostics, SELF->current - SELF->start);
      SELF->kind = dx_data_definition_language_word_kind_error;
      SELF->range.end = SELF->current;
      return Core_Success;
    }
    do {
      if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
        return Core_Failure;
      }
    } while (dx_data_definition_language_scanner_is_digit(SELF));
    if (Core_getError()) {
      return Core_Failure;
    }
  }
  SELF->kind = dx_data_definition_language_word_kind_number;
  SELF->range.end = SELF->current;
  return Core_Success;
}

static void dx_data_definition_language_scanner_destruct(dx_data_definition_language_scanner* SELF) {
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
  dx_inline_byte_array_uninitialize(&SELF->text);
}

static void dx_data_definition_language_scanner_constructDispatch(dx_data_definition_language_scanner_dispatch* SELF) {
  DX_SCANNER_DISPATCH(SELF)->get_word_end_offset = (Core_Result(*)(Core_Size*,dx_scanner*)) & get_word_end_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_start_offset = (Core_Result(*)(Core_Size*, dx_scanner*)) & get_word_start_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_bytes = (Core_Result(*)(char const**, dx_scanner*)) & get_word_text_bytes;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_number_of_bytes = (Core_Result(*)(Core_Size*, dx_scanner*)) & get_word_text_number_of_bytes;
  DX_SCANNER_DISPATCH(SELF)->set = (Core_Result(*)(dx_scanner*, char const*, Core_Size)) & set;
  DX_SCANNER_DISPATCH(SELF)->step = (Core_Result(*)(dx_scanner*)) & step;
}

Core_Result dx_data_definition_language_scanner_construct(dx_data_definition_language_scanner* SELF, dx_data_definition_language_diagnostics* diagnostics) {
  DX_CONSTRUCT_PREFIX(dx_data_definition_language_scanner);

  if (dx_scanner_construct(DX_SCANNER(SELF))) {
    return Core_Failure;
  }

  if (dx_inline_byte_array_initialize(&SELF->text)) {
    return Core_Failure;
  } 
  if (!diagnostics) {
    dx_inline_byte_array_uninitialize(&SELF->text);
    return Core_Failure;
  }
  SELF->diagnostics = diagnostics;
  DX_REFERENCE(SELF->diagnostics);

  static char const EMPTY[] = "";
  SELF->start = &EMPTY[0];
  SELF->end = SELF->start;
  SELF->current = SELF->start;
  SELF->kind = dx_data_definition_language_word_kind_start_of_input;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static Core_Result dx_data_definition_language_scanner_skip_single_line_comment(dx_data_definition_language_scanner* SELF) {
  while (!(SELF->current == SELF->end)
      && !(SELF->current != SELF->end && *SELF->current == '\n')
      && !(SELF->current != SELF->end && *SELF->current == '\r')) {
    SELF->current++;
  }
  return Core_Success;
}

static Core_Result dx_data_definition_language_scanner_skip_multi_line_comment(dx_data_definition_language_scanner* SELF) {
  while (true) {
    if (SELF->current == SELF->end) {
      dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(SELF->diagnostics, SELF->current - SELF->start);
      Core_setError(Core_Error_LexicalAnalysisFailed);
      return Core_Failure;
    } else if (*SELF->current == '*') {
      SELF->current++;
      if (SELF->current == SELF->end) {
        dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(SELF->diagnostics, SELF->current - SELF->start);
        SELF->kind = dx_data_definition_language_word_kind_error;
        SELF->range.end = SELF->current;
        return Core_Success;
      }
      if (*SELF->current == '/') {
        SELF->current++;
        return Core_Success;
      } else {
        continue;
      }
    } else {
      SELF->current++;
    }
  }
  return Core_Success;
}

static Core_Result dx_data_definition_language_scanner_skip_nls_and_ws(dx_data_definition_language_scanner* SELF) {
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
  return Core_Success;
}

Core_Result dx_data_definition_language_scanner_create(dx_data_definition_language_scanner** RETURN, dx_data_definition_language_diagnostics* diagnostics) {
  DX_CREATE_PREFIX(dx_data_definition_language_scanner);
  if (dx_data_definition_language_scanner_construct(SELF, diagnostics)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_data_definition_language_scanner_get_word_kind(dx_data_definition_language_word_kind* RETURN, dx_data_definition_language_scanner const* SELF) {
  *RETURN = SELF->kind;
  return Core_Success;
}
