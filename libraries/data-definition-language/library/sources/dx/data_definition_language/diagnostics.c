#include "dx/data_definition_language/diagnostics.h"

Core_defineObjectType("dx.ddl.diagnostics",
                      dx_data_definition_language_diagnostics,
                      Core_Object);

static void dx_data_definition_language_diagnostics_destruct(dx_data_definition_language_diagnostics* SELF)
{/*Intentionally empty.*/}

static void dx_data_definition_language_diagnostics_constructDispatch(dx_data_definition_language_diagnostics_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_data_definition_language_diagnostics_construct(dx_data_definition_language_diagnostics* SELF) {
  DX_CONSTRUCT_PREFIX(dx_data_definition_language_diagnostics);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_data_definition_language_diagnostics_create(dx_data_definition_language_diagnostics** RETURN) {
  DX_CREATE_PREFIX(dx_data_definition_language_diagnostics);
  if (dx_data_definition_language_diagnostics_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_data_definition_language_diagnostics_on_lexical(dx_data_definition_language_diagnostics* SELF, Core_Natural64 offset) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "lexical error:\n", sizeof("lexical error:\n") - 1)) {
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}

Core_Result dx_data_definition_language_diagnostics_on_unclosed_string_literal(dx_data_definition_language_diagnostics* SELF, Core_Natural64 offset) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "lexical error: unclosed string literal\n", sizeof("lexical error: unclosed string literal\n") - 1)) {
    return Core_Failure;
  }
  dx_log(format->bytes, format->number_of_bytes);
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}

Core_Result dx_data_definition_language_diagnostics_on_invalid_number_literal(dx_data_definition_language_diagnostics* SELF, Core_Natural64 offset) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "lexical error: invalid number literal\n", sizeof("lexical error: invalid number literal\n") - 1)) {
    return Core_Failure;
  }
  dx_log(format->bytes, format->number_of_bytes);
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}

Core_Result dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(dx_data_definition_language_diagnostics* SELF, Core_Natural64 offset) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "lexical error: unclosed multi-line comment\n", sizeof("lexical error: unclosed multi-line comment\n") - 1)) {
    return Core_Failure;
  }
  dx_log(format->bytes, format->number_of_bytes);
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}

Core_Result dx_data_definition_language_diagnostics_on_invalid_name(dx_data_definition_language_diagnostics* SELF, Core_Natural64 offset) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "lexical error: invalid name\n", sizeof("lexical error: invalid name\n") - 1)) {
    return Core_Failure;
  }
  dx_log(format->bytes, format->number_of_bytes);
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}


Core_Result dx_data_definition_language_diagnostics_on_unexpected_symbol(dx_data_definition_language_diagnostics* SELF, Core_Natural64 offset) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "lexical error: unexpected symbol\n", sizeof("lexical error: unexpected symbol\n") - 1)) {
    return Core_Failure;
  }
  dx_log(format->bytes, format->number_of_bytes);
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}
