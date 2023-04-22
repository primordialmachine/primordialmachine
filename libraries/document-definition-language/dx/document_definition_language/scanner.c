#include "dx/document_definition_language/scanner.h"

#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result set(dx_document_definition_language_scanner* SELF, char const* bytes, dx_size number_of_bytes);

static dx_result step(dx_document_definition_language_scanner* SELF);

static dx_result get_word_text_bytes(char const** RETURN, dx_document_definition_language_scanner* SELF);

static dx_result get_word_text_number_of_bytes(dx_size* RETURN, dx_document_definition_language_scanner* SELF);

static dx_result get_word_start_offset(dx_size* RETURN, dx_document_definition_language_scanner* SELF);

static dx_result get_word_end_offset(dx_size* RETURN, dx_document_definition_language_scanner* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_document_definition_language_scanner_save_and_next(dx_document_definition_language_scanner* SELF);

static dx_result dx_document_definition_language_scanner_skip_nls(dx_document_definition_language_scanner* SELF);

DX_DEFINE_OBJECT_TYPE("dx.document_definition_language.scanner",
                      dx_document_definition_language_scanner,
                      dx_scanner);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result set(dx_document_definition_language_scanner* SELF, char const* p, dx_size l) {
  SELF->start = p;
  SELF->end = p + l;
  SELF->current = p;
  SELF->kind = dx_document_definition_language_word_kind_start_of_input;
  dx_inline_byte_array_clear(&SELF->text);
  return DX_SUCCESS;
}

static dx_result step(dx_document_definition_language_scanner* SELF) {
START:
  // If the current word is "end of input", return immediatly.
  if (SELF->kind == dx_document_definition_language_word_kind_end_of_input) {
    return DX_SUCCESS;
  }
  // If the current word is "error", return immediatly.
  if (SELF->kind == dx_document_definition_language_word_kind_error) {
    return DX_SUCCESS;
  }
  SELF->range.start = SELF->current;
  SELF->range.end = SELF->current;
  dx_inline_byte_array_clear(&SELF->text);
  while (true) {
    // end of input
    if (SELF->current == SELF->end) {
      SELF->range.end = SELF->current;
      if (SELF->range.start < SELF->current) {
        dx_inline_byte_array_append(&SELF->text, SELF->range.start, SELF->range.end - SELF->range.start);
        SELF->kind = dx_document_definition_language_word_kind_line;
      } else {
        SELF->kind = dx_document_definition_language_word_kind_end_of_input;
      }
      break;
    // new line
    } else if (*SELF->current == '\n' || *SELF->current == '\r') {
      dx_bool stop = false;
      char old = *SELF->current;
      SELF->current++;
      if ((*SELF->current == '\n' || *SELF->current == '\r') && *SELF->current != old && SELF->current != SELF->end) {
        SELF->current++;
      }
      SELF->range.end = SELF->current;
      dx_inline_byte_array_append(&SELF->text, SELF->range.start, SELF->range.end - SELF->range.start);
      SELF->kind = dx_document_definition_language_word_kind_line;
      break;
    } else {
      SELF->current++;
    }
  }
  return DX_SUCCESS;
}

static dx_result get_word_text_bytes(char const** RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->text.elements;
  return DX_SUCCESS;
}

static dx_result get_word_text_number_of_bytes(dx_size* RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->text.size;
  return DX_SUCCESS;
}

static dx_result get_word_start_offset(dx_size* RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->range.start - SELF->start;
  return DX_SUCCESS;
}

static dx_result get_word_end_offset(dx_size* RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->range.end - SELF->start;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_document_definition_language_scanner_save_and_next(dx_document_definition_language_scanner* SELF) {
  if (SELF->current == SELF->end) {
    return DX_FAILURE;
  }
  if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
    return DX_FAILURE;
  }
  SELF->current++;
  return DX_SUCCESS;
}

static void dx_document_definition_language_scanner_destruct(dx_document_definition_language_scanner* SELF) {
  dx_inline_byte_array_uninitialize(&SELF->text);
}

static void dx_document_definition_language_scanner_dispatch_construct(dx_document_definition_language_scanner_dispatch* SELF) {
  DX_SCANNER_DISPATCH(SELF)->get_word_end_offset = (dx_result(*)(dx_size*,dx_scanner*)) & get_word_end_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_start_offset = (dx_result(*)(dx_size*, dx_scanner*)) & get_word_start_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_bytes = (dx_result(*)(char const**, dx_scanner*)) & get_word_text_bytes;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_number_of_bytes = (dx_result(*)(dx_size*, dx_scanner*)) & get_word_text_number_of_bytes;
  DX_SCANNER_DISPATCH(SELF)->set = (dx_result(*)(dx_scanner*, char const*, dx_size)) & set;
  DX_SCANNER_DISPATCH(SELF)->step = (dx_result(*)(dx_scanner*)) & step;
}

dx_result dx_document_definition_language_scanner_construct(dx_document_definition_language_scanner* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_document_definition_language_scanner_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_scanner_construct(DX_SCANNER(SELF))) {
    return DX_FAILURE;
  }

  if (dx_inline_byte_array_initialize(&SELF->text)) {
    return DX_FAILURE;
  } 

  static char const EMPTY[] = "";
  SELF->start = &EMPTY[0];
  SELF->end = SELF->start;
  SELF->current = SELF->start;
  SELF->kind = dx_document_definition_language_word_kind_start_of_input;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static dx_result dx_document_definition_language_scanner_skip_nls(dx_document_definition_language_scanner* SELF) {
  bool stop;
  do {
    stop = true;
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

dx_result dx_document_definition_language_scanner_create(dx_document_definition_language_scanner** RETURN) {
  dx_document_definition_language_scanner* SELF = DX_DOCUMENT_DEFINITION_LANGUAGE_SCANNER(dx_object_alloc(sizeof(dx_document_definition_language_scanner)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_document_definition_language_scanner_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_document_definition_language_scanner_get_word_kind(dx_document_definition_language_word_kind* RETURN, dx_document_definition_language_scanner const* SELF) {
  *RETURN = SELF->kind;
  return DX_SUCCESS;
}
