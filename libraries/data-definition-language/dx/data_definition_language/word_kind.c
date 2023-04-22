#include "dx/data_definition_language/word_kind.h"

DX_DEFINE_ENUMERATION_TYPE("dx.ddl.word_kind",
                           dx_ddl_word_kind);

dx_result dx_ddl_word_kind_to_string(dx_string** RETURN, dx_ddl_word_kind SELF) {
  switch (SELF) {
  #define DEFINE(SYMBOL, STRING) \
    case SYMBOL: { \
      return dx_string_create(RETURN, STRING, sizeof(STRING) - 1); \
    } break;
  #include "dx/data_definition_language/word_kind.i"
  #undef DEFINE
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    } break;
  };
}
