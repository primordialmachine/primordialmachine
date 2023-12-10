#include "dx/data_definition_language/node_kind.h"

DX_DEFINE_ENUMERATION_TYPE("dx.ddl.node_kind",
                           dx_ddl_node_kind);

Core_Result dx_ddl_node_kind_to_string(Core_String** RETURN, dx_ddl_node_kind SELF) {
  switch (SELF) {
  #define DEFINE(SYMBOL, STRING) \
    case SYMBOL: { \
      return Core_String_create(RETURN, STRING, sizeof(STRING) - 1); \
    } break;
  #include "dx/data_definition_language/node_kind.i"
  #undef DEFINE
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };
}
