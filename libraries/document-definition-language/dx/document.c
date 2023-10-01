#include "dx/document.h"

#include "dx/document_definition_language.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.text_line",
                      dx_text_range,
                      dx_object);

static void dx_text_range_destruct(dx_text_range* SELF)
{/*Intentionally empty.*/}

static void dx_text_range_dispatch_construct(dx_text_range_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_text_range_construct(dx_text_range* SELF, dx_size start, dx_size length) {
  dx_rti_type* TYPE = dx_text_range_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->start = start;
  SELF->length = length;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_text_range_create(dx_text_range** RETURN, dx_size start, dx_size length) {
  DX_CREATE_PREFIX(dx_text_range)
  if (dx_text_range_construct(SELF, start, length)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.text_document",
                      dx_text_document,
                      dx_object);

static void dx_text_document_destruct(dx_text_document* SELF) {
  DX_UNREFERENCE(SELF->lines);
  SELF->lines = NULL;
  DX_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_text_document_dispatch_construct(dx_text_document_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_text_document_construct(dx_text_document* SELF) {
  dx_rti_type* TYPE = dx_text_document_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_object_array_create(&SELF->lines, 0)) {
    return DX_FAILURE;
  }
  if (dx_string_buffer_create(&SELF->text)) {
    DX_UNREFERENCE(SELF->lines);
    SELF->lines = NULL;
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_text_document_create(dx_text_document** RETURN) {
  DX_CREATE_PREFIX(dx_text_document)
  if (dx_text_document_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result get_last_line_or_null(dx_text_range** RETURN, dx_text_document* SELF) {
  dx_size size = 0;
  if (dx_object_array_get_size(&size, SELF->lines)) {
    return DX_FAILURE;
  }
  if (size == 0) {
    *RETURN = NULL;
    return DX_SUCCESS;
  }
  return dx_object_array_get_at((dx_object**)RETURN, SELF->lines, size - 1);
}

static dx_result dx_string_buffer_ends_with_newline(dx_bool* RETURN, dx_string_buffer* SELF) {
  if (SELF->backend.size < 1) {
    *RETURN = DX_FALSE;
    return DX_SUCCESS;
  }
  *RETURN = (SELF->backend.elements[SELF->backend.size - 1] == '\n')
         || (SELF->backend.elements[SELF->backend.size - 1] == '\r');
  return DX_SUCCESS;
}

static dx_result dx_text_document_append_lines(dx_text_document* SELF, dx_object_array* lines) {
  dx_size existing_number_of_lines = 0, number_of_lines = 0;
  if (dx_object_array_get_size(&existing_number_of_lines, SELF->lines) || dx_object_array_get_size(&number_of_lines, lines)) {
    return DX_FAILURE;
  }
  // if the array is empty, there is nothing to do.
  if (0 == number_of_lines) {
    return DX_SUCCESS;
  }
  // if the last existing line does not end with a newline,
  // append the first (incoming) line to that line.
  dx_text_range* last_line = NULL;
  if (get_last_line_or_null(&last_line, SELF)) {
    return DX_FAILURE;
  }
  dx_size i = 0;
  if (last_line) {
    dx_bool ends_with_newline = false;
    if (dx_string_buffer_ends_with_newline(&ends_with_newline, SELF->text)) {
      return DX_FAILURE;
    }
    dx_text_range* current_line = NULL;
    if (dx_object_array_get_at((dx_object**)&current_line, lines, 0)) {
      return DX_FAILURE;
    }
    if (!ends_with_newline) {
      last_line->length += current_line->length;
      i++;
    }
  }
  // add the remaining lines.
  for (; i < number_of_lines; ++i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((dx_object**)&line, lines, i)) {
      return DX_FAILURE;
    }
    if (dx_object_array_append(SELF->lines, DX_OBJECT(line))) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static dx_result parse(dx_object_array** RETURN, dx_string* text) {
  dx_object_array* lines;
  if (dx_object_array_create(&lines, 0)) {
    return DX_FAILURE;
  }
  // Scan the text.
  dx_document_definition_language_scanner* scanner = NULL;
  if (dx_document_definition_language_scanner_create(&scanner)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, text)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  void const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, text)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  if (dx_scanner_set(DX_SCANNER(scanner), bytes, number_of_bytes)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  dx_document_definition_language_word_kind word_kind;
  // start of input
  if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  if (word_kind != dx_document_definition_language_word_kind_start_of_input) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    return DX_FAILURE;
  }
  if (dx_scanner_step(DX_SCANNER(scanner))) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  // lines*
  if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    return DX_FAILURE;
  }
  while (dx_document_definition_language_word_kind_line == word_kind) {
    dx_size word_start_offset = 0,
      word_end_offset = 0;
    if (dx_scanner_get_word_start_offset(&word_start_offset, DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    if (dx_scanner_get_word_end_offset(&word_end_offset, DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    dx_text_range* current_line = NULL;
    if (dx_text_range_create(&current_line, word_start_offset, word_end_offset - word_start_offset)) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    if (dx_object_array_append(lines, DX_OBJECT(current_line))) {
      DX_UNREFERENCE(current_line);
      current_line = NULL;
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(current_line);
    current_line = NULL;
    if (dx_scanner_step(DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
  }
  // end of input
  if (word_kind != dx_document_definition_language_word_kind_end_of_input) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  
  *RETURN = lines;

  return DX_SUCCESS;
}

dx_result dx_text_document_set_text(dx_text_document* SELF, dx_string* text) {
  dx_object_array* lines = NULL;
  if (parse(&lines, text)) {
    return DX_FAILURE;
  }
  dx_size number_of_lines;
  if (dx_object_array_get_size(&number_of_lines, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  dx_object_array_clear(SELF->lines);
  if (dx_text_document_append_lines(SELF, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(lines);
  lines = NULL;
  if (dx_string_buffer_set_string(SELF->text, text)) {
    /// @todo Can we recover?
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_text_document_append_text(dx_text_document* SELF, dx_string* text) {
  // Get the start offset.
  dx_size start_offset;
  if (dx_string_buffer_get_number_of_bytes(&start_offset, SELF->text)) {
    return DX_FAILURE;
  }
  dx_object_array* lines = NULL;
  if (parse(&lines, text)) {
    return DX_FAILURE;
  }
  dx_size number_of_lines;
  if (dx_object_array_get_size(&number_of_lines, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }

  for (dx_size i = 0; i < number_of_lines; ++i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((dx_object**) & line, lines, i)) {
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    line->start += start_offset;
  }

  if (dx_text_document_append_lines(SELF, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(lines);
  lines = NULL;
  if (dx_string_buffer_append_string(SELF->text, text)) {
    /// @todo Can we recover?
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
