/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_NODE_KIND_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_NODE_KIND_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_ENUMERATION_TYPE("dx.ddl.node_kind",
                            dx_ddl_node_kind);

enum dx_ddl_node_kind {
#define DEFINE(SYMBOL, STRING) \
  SYMBOL,
#include "dx/data_definition_language/node_kind.i"
#undef DEFINE
};

Core_Result dx_ddl_node_kind_to_string(Core_String** RETURN, dx_ddl_node_kind SELF);

#endif // DX_DATA_DEFINITION_LANGUAGE_NODE_KIND_H_INCLUDED
