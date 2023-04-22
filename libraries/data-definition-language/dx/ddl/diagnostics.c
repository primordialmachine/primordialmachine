#include "dx/ddl/diagnostics.h"

DX_DEFINE_OBJECT_TYPE("dx.ddl.diagnostics",
                      dx_data_definition_language_diagnostics,
                      dx_object);

static void dx_data_definition_language_diagnostics_destruct(dx_data_definition_language_diagnostics* SELF)
{/*Intentionally empty.*/}

static void dx_data_definition_language_diagnostics_dispatch_construct(dx_data_definition_language_diagnostics_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_data_definition_language_diagnostics_construct(dx_data_definition_language_diagnostics* SELF) {
  dx_rti_type* TYPE = dx_data_definition_language_diagnostics_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_diagnostics_create(dx_data_definition_language_diagnostics** RETURN) {
  dx_data_definition_language_diagnostics* SELF = DX_DATA_DEFINITION_LANGUAGE_DIAGNOSTICS(dx_object_alloc(sizeof(dx_data_definition_language_diagnostics)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_data_definition_language_diagnostics_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_diagnostics_on_lexical(dx_data_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "lexical error:\n", sizeof("lexical error:\n") - 1)) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_diagnostics_on_unclosed_string_literal(dx_data_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "lexical error: unclosed string literal\n", sizeof("lexical error: unclosed string literal\n") - 1)) {
    return DX_FAILURE;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_diagnostics_on_invalid_number_literal(dx_data_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "lexical error: invalid number literal\n", sizeof("lexical error: invalid number literal\n") - 1)) {
    return DX_FAILURE;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(dx_data_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "lexical error: unclosed multi-line comment\n", sizeof("lexical error: unclosed multi-line comment\n") - 1)) {
    return DX_FAILURE;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}

dx_result dx_data_definition_language_diagnostics_on_invalid_name(dx_data_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "lexical error: invalid name\n", sizeof("lexical error: invalid name\n") - 1)) {
    return DX_FAILURE;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}


dx_result dx_data_definition_language_diagnostics_on_unexpected_symbol(dx_data_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "lexical error: unexpected symbol\n", sizeof("lexical error: unexpected symbol\n") - 1)) {
    return DX_FAILURE;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}
