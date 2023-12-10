#include "dx/document.h"

#include "dx/document_definition_language.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.text_line",
                      dx_text_range,
                      Core_Object);

static void dx_text_range_destruct(dx_text_range* SELF)
{/*Intentionally empty.*/}

static void dx_text_range_constructDispatch(dx_text_range_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_text_range_construct(dx_text_range* SELF, Core_Size start, Core_Size length) {
  DX_CONSTRUCT_PREFIX(dx_text_range);
  SELF->start = start;
  SELF->length = length;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_text_range_create(dx_text_range** RETURN, Core_Size start, Core_Size length) {
  DX_CREATE_PREFIX(dx_text_range);
  if (dx_text_range_construct(SELF, start, length)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.text_document",
                      dx_text_document,
                      Core_Object);

static void dx_text_document_destruct(dx_text_document* SELF) {
  DX_UNREFERENCE(SELF->lines);
  SELF->lines = NULL;
  DX_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_text_document_constructDispatch(dx_text_document_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_text_document_construct(dx_text_document* SELF) {
  DX_CONSTRUCT_PREFIX(dx_text_document);
  if (dx_object_array_create(&SELF->lines, 0)) {
    return Core_Failure;
  }
  if (dx_string_buffer_create(&SELF->text)) {
    DX_UNREFERENCE(SELF->lines);
    SELF->lines = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_text_document_create(dx_text_document** RETURN) {
  DX_CREATE_PREFIX(dx_text_document);
  if (dx_text_document_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result get_last_line_or_null(dx_text_range** RETURN, dx_text_document* SELF) {
  Core_Size size = 0;
  if (dx_object_array_get_size(&size, SELF->lines)) {
    return Core_Failure;
  }
  if (size == 0) {
    *RETURN = NULL;
    return Core_Success;
  }
  return dx_object_array_get_at((Core_Object**)RETURN, SELF->lines, size - 1);
}

static Core_Result dx_string_buffer_ends_with_newline(Core_Boolean* RETURN, dx_string_buffer* SELF) {
  if (SELF->backend.size < 1) {
    *RETURN = Core_False;
    return Core_Success;
  }
  *RETURN = (SELF->backend.elements[SELF->backend.size - 1] == '\n')
         || (SELF->backend.elements[SELF->backend.size - 1] == '\r');
  return Core_Success;
}

static Core_Result dx_text_document_append_lines(dx_text_document* SELF, dx_object_array* lines) {
  Core_Size existing_number_of_lines = 0, number_of_lines = 0;
  if (dx_object_array_get_size(&existing_number_of_lines, SELF->lines) || dx_object_array_get_size(&number_of_lines, lines)) {
    return Core_Failure;
  }
  // if the array is empty, there is nothing to do.
  if (0 == number_of_lines) {
    return Core_Success;
  }
  // if the last existing line does not end with a newline,
  // append the first (incoming) line to that line.
  dx_text_range* last_line = NULL;
  if (get_last_line_or_null(&last_line, SELF)) {
    return Core_Failure;
  }
  Core_Size i = 0;
  if (last_line) {
    Core_Boolean ends_with_newline = false;
    if (dx_string_buffer_ends_with_newline(&ends_with_newline, SELF->text)) {
      return Core_Failure;
    }
    dx_text_range* current_line = NULL;
    if (dx_object_array_get_at((Core_Object**)&current_line, lines, 0)) {
      return Core_Failure;
    }
    if (!ends_with_newline) {
      last_line->length += current_line->length;
      i++;
    }
  }
  // add the remaining lines.
  for (; i < number_of_lines; ++i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((Core_Object**)&line, lines, i)) {
      return Core_Failure;
    }
    if (dx_object_array_append(SELF->lines, CORE_OBJECT(line))) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

static Core_Result parse(dx_object_array** RETURN, Core_String* text) {
  dx_object_array* lines;
  if (dx_object_array_create(&lines, 0)) {
    return Core_Failure;
  }
  // Scan the text.
  dx_document_definition_language_scanner* scanner = NULL;
  if (dx_document_definition_language_scanner_create(&scanner)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  Core_Size number_of_bytes = 0;
  if (Core_String_getNumberOfBytes(&number_of_bytes, text)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  void const* bytes = NULL;
  if (Core_String_getBytes(&bytes, text)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  if (dx_scanner_set(DX_SCANNER(scanner), bytes, number_of_bytes)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  dx_document_definition_language_word_kind word_kind;
  // start of input
  if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  if (word_kind != dx_document_definition_language_word_kind_start_of_input) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    return Core_Failure;
  }
  if (dx_scanner_step(DX_SCANNER(scanner))) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  // lines*
  if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    return Core_Failure;
  }
  while (dx_document_definition_language_word_kind_line == word_kind) {
    Core_Size word_start_offset = 0,
      word_end_offset = 0;
    if (dx_scanner_get_word_start_offset(&word_start_offset, DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
    if (dx_scanner_get_word_end_offset(&word_end_offset, DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
    dx_text_range* current_line = NULL;
    if (dx_text_range_create(&current_line, word_start_offset, word_end_offset - word_start_offset)) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
    if (dx_object_array_append(lines, CORE_OBJECT(current_line))) {
      DX_UNREFERENCE(current_line);
      current_line = NULL;
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(current_line);
    current_line = NULL;
    if (dx_scanner_step(DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
    if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
  }
  // end of input
  if (word_kind != dx_document_definition_language_word_kind_end_of_input) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  
  *RETURN = lines;

  return Core_Success;
}

Core_Result dx_text_document_set_text(dx_text_document* SELF, Core_String* text) {
  dx_object_array* lines = NULL;
  if (parse(&lines, text)) {
    return Core_Failure;
  }
  Core_Size number_of_lines;
  if (dx_object_array_get_size(&number_of_lines, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  dx_object_array_clear(SELF->lines);
  if (dx_text_document_append_lines(SELF, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(lines);
  lines = NULL;
  if (dx_string_buffer_set_string(SELF->text, text)) {
    /// @todo Can we recover?
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_text_document_get_text(Core_String** RETURN, dx_text_document* SELF) {
  return dx_string_buffer_to_string(RETURN, SELF->text);
}

Core_Result dx_text_document_append_text(dx_text_document* SELF, Core_String* text) {
  // Get the start offset.
  Core_Size start_offset;
  if (dx_string_buffer_get_number_of_bytes(&start_offset, SELF->text)) {
    return Core_Failure;
  }
  dx_object_array* lines = NULL;
  if (parse(&lines, text)) {
    return Core_Failure;
  }
  Core_Size number_of_lines;
  if (dx_object_array_get_size(&number_of_lines, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }

  for (Core_Size i = 0; i < number_of_lines; ++i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((Core_Object**) & line, lines, i)) {
      DX_UNREFERENCE(lines);
      lines = NULL;
      return Core_Failure;
    }
    line->start += start_offset;
  }

  if (dx_text_document_append_lines(SELF, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(lines);
  lines = NULL;
  if (dx_string_buffer_append_string(SELF->text, text)) {
    /// @todo Can we recover?
    return Core_Failure;
  }
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
