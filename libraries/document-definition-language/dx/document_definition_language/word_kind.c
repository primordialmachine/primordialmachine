#include "dx/document_definition_language/word_kind.h"

Core_defineEnumerationType("dx.document_definition_language.word_kind",
                           dx_document_definition_language_word_kind);

Core_Result dx_document_definition_language_word_kind_to_string(Core_String** RETURN, dx_document_definition_language_word_kind SELF) {
  switch (SELF) {
  #define DEFINE(SYMBOL, STRING) \
    case SYMBOL: { \
      return Core_String_create(RETURN, STRING, sizeof(STRING) - 1); \
    } break;
  #include "dx/document_definition_language/word_kind.i"
  #undef DEFINE
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };
}
