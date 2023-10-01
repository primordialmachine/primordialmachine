/// @file dx/ddl/diagnostics.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_DIAGNOSTICS_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_DIAGNOSTICS_H_INCLUDED

#include "dx/core.h"
#include "dx/data_definition_language/configuration.h"

DX_DECLARE_OBJECT_TYPE("dx.data_definition_language.diagnostics",
                       dx_data_definition_language_diagnostics,
                       dx_object);

static inline dx_data_definition_language_diagnostics* DX_DATA_DEFINITION_LANGUAGE_DIAGNOSTICS(void* p) {
  return (dx_data_definition_language_diagnostics*)p;
}

struct dx_data_definition_language_diagnostics {
  dx_object _parent;
};

static inline dx_data_definition_language_diagnostics_dispatch* DX_DATA_DEFINITION_LANGUAGE_DIAGNOSTICS_DISPATCH(void* p) {
  return (dx_data_definition_language_diagnostics_dispatch*)p;
}

struct dx_data_definition_language_diagnostics_dispatch {
  dx_object_dispatch _parent;
};

/// @constructor{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_construct(dx_data_definition_language_diagnostics* SELF);

/// @create-operator{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_create(dx_data_definition_language_diagnostics** RETURN);

/// @method{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_on_lexical(dx_data_definition_language_diagnostics* SELF, dx_n64 offset);

/// @brief An unclosed string literal.
/// @method{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_on_unclosed_string_literal(dx_data_definition_language_diagnostics* SELF, dx_n64 offset);

/// @brief An invalid number literal.
/// @method{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_on_invalid_number_literal(dx_data_definition_language_diagnostics* SELF, dx_n64 offset);

/// @brief An unclosed multi-line comment.
/// @method{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_on_unclosed_multi_line_comment(dx_data_definition_language_diagnostics* SELF, dx_n64 offset);

/// @brief An invalid name.
/// @method{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_on_invalid_name(dx_data_definition_language_diagnostics* SELF, dx_n64 offset);

/// @brief An unexpected symbol.
/// @method{dx_data_definition_language_diagnostics}
dx_result dx_data_definition_language_diagnostics_on_unexpected_symbol(dx_data_definition_language_diagnostics* SELF, dx_n64 offset);

#endif // DX_DATA_DEFINITION_LANGUAGE_DIAGNOSTICS_H_INCLUDED
