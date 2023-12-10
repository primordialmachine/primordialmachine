#include "dx/document_definition_language/scanner.h"

#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result set(dx_document_definition_language_scanner* SELF, char const* bytes, Core_Size number_of_bytes);

static Core_Result step(dx_document_definition_language_scanner* SELF);

static Core_Result get_word_text_bytes(char const** RETURN, dx_document_definition_language_scanner* SELF);

static Core_Result get_word_text_number_of_bytes(Core_Size* RETURN, dx_document_definition_language_scanner* SELF);

static Core_Result get_word_start_offset(Core_Size* RETURN, dx_document_definition_language_scanner* SELF);

static Core_Result get_word_end_offset(Core_Size* RETURN, dx_document_definition_language_scanner* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_document_definition_language_scanner_save_and_next(dx_document_definition_language_scanner* SELF);

static Core_Result dx_document_definition_language_scanner_skip_nls(dx_document_definition_language_scanner* SELF);

DX_DEFINE_OBJECT_TYPE("dx.document_definition_language.scanner",
                      dx_document_definition_language_scanner,
                      dx_scanner);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result set(dx_document_definition_language_scanner* SELF, char const* p, Core_Size l) {
  SELF->start = p;
  SELF->end = p + l;
  SELF->current = p;
  SELF->kind = dx_document_definition_language_word_kind_start_of_input;
  dx_inline_byte_array_clear(&SELF->text);
  return Core_Success;
}

static Core_Result step(dx_document_definition_language_scanner* SELF) {
START:
  // If the current word is "end of input", return immediatly.
  if (SELF->kind == dx_document_definition_language_word_kind_end_of_input) {
    return Core_Success;
  }
  // If the current word is "error", return immediatly.
  if (SELF->kind == dx_document_definition_language_word_kind_error) {
    return Core_Success;
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
      Core_Boolean stop = false;
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
  return Core_Success;
}

static Core_Result get_word_text_bytes(char const** RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->text.elements;
  return Core_Success;
}

static Core_Result get_word_text_number_of_bytes(Core_Size* RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->text.size;
  return Core_Success;
}

static Core_Result get_word_start_offset(Core_Size* RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->range.start - SELF->start;
  return Core_Success;
}

static Core_Result get_word_end_offset(Core_Size* RETURN, dx_document_definition_language_scanner* SELF) {
  *RETURN = SELF->range.end - SELF->start;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_document_definition_language_scanner_save_and_next(dx_document_definition_language_scanner* SELF) {
  if (SELF->current == SELF->end) {
    return Core_Failure;
  }
  if (dx_inline_byte_array_append(&SELF->text, SELF->current, 1)) {
    return Core_Failure;
  }
  SELF->current++;
  return Core_Success;
}

static void dx_document_definition_language_scanner_destruct(dx_document_definition_language_scanner* SELF) {
  dx_inline_byte_array_uninitialize(&SELF->text);
}

static void dx_document_definition_language_scanner_constructDispatch(dx_document_definition_language_scanner_dispatch* SELF) {
  DX_SCANNER_DISPATCH(SELF)->get_word_end_offset = (Core_Result(*)(Core_Size*,dx_scanner*)) & get_word_end_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_start_offset = (Core_Result(*)(Core_Size*, dx_scanner*)) & get_word_start_offset;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_bytes = (Core_Result(*)(char const**, dx_scanner*)) & get_word_text_bytes;
  DX_SCANNER_DISPATCH(SELF)->get_word_text_number_of_bytes = (Core_Result(*)(Core_Size*, dx_scanner*)) & get_word_text_number_of_bytes;
  DX_SCANNER_DISPATCH(SELF)->set = (Core_Result(*)(dx_scanner*, char const*, Core_Size)) & set;
  DX_SCANNER_DISPATCH(SELF)->step = (Core_Result(*)(dx_scanner*)) & step;
}

Core_Result dx_document_definition_language_scanner_construct(dx_document_definition_language_scanner* SELF) {
  DX_CONSTRUCT_PREFIX(dx_document_definition_language_scanner);

  if (dx_scanner_construct(DX_SCANNER(SELF))) {
    return Core_Failure;
  }

  if (dx_inline_byte_array_initialize(&SELF->text)) {
    return Core_Failure;
  } 

  static char const EMPTY[] = "";
  SELF->start = &EMPTY[0];
  SELF->end = SELF->start;
  SELF->current = SELF->start;
  SELF->kind = dx_document_definition_language_word_kind_start_of_input;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static Core_Result dx_document_definition_language_scanner_skip_nls(dx_document_definition_language_scanner* SELF) {
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
  return Core_Success;
}

Core_Result dx_document_definition_language_scanner_create(dx_document_definition_language_scanner** RETURN) {
  DX_CREATE_PREFIX(dx_document_definition_language_scanner);
  if (dx_document_definition_language_scanner_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_document_definition_language_scanner_get_word_kind(dx_document_definition_language_word_kind* RETURN, dx_document_definition_language_scanner const* SELF) {
  *RETURN = SELF->kind;
  return Core_Success;
}
