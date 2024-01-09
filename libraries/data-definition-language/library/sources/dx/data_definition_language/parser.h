/// @file dx/ddl/parser.h
/// @brief Syntactical functionality (aka "parsing") of Asset Description Language (ADL) files.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_PARSER_H_INCLUDED

#include "dx/data_definition_language/diagnostics.h"

#include "dx/data_definition_language/word_kind.h"
#include "dx/data_definition_language/word.h"
#include "dx/data_definition_language/scanner.h"
#include "dx/data_definition_language/node_kind.h"
#include "dx/data_definition_language/node.h"

/// @brief A parser used for parsing programs of the language "2023-06-01".
Core_declareObjectType("dx.ddl.parser",
                       dx_ddl_parser,
                       Core_Object);

static inline dx_ddl_parser* DX_DATA_DEFINITION_LANGUAGE_PARSER(void* p) {
  return (dx_ddl_parser*)p;
}

struct dx_ddl_parser {
  Core_Object _parent;
  /// @brief A pointer to the underlaying scanner.
  dx_data_definition_language_scanner* scanner;
  /// @brief A pointer to the underlaying diagnostics.
  dx_data_definition_language_diagnostics* diagnostics;
};

static inline dx_ddl_parser_Dispatch* DX_DATA_DEFINITION_LANGUAGE_PARSER_DISPATCH(void* p) {
  return (dx_ddl_parser_Dispatch*)p;
}

struct dx_ddl_parser_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Construct this parser with an empty input.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @success
/// The parser was assigned the empty input and is in the start state w.r.t. the specified input.
/// @constructor{dx_ddl_parser}
Core_Result dx_ddl_parser_construct(dx_ddl_parser* SELF, dx_data_definition_language_scanner* scanner, dx_data_definition_language_diagnostics* diagnostics);

/// @brief Create this parser with an empty input.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @success The parser was assigned the empty input and is in the start state w.r.t. the specified input.
/// @create-operator{dx_ddl_parser}
Core_Result dx_ddl_parser_create(dx_ddl_parser** RETURN, dx_data_definition_language_scanner* scanner, dx_data_definition_language_diagnostics* diagnostics);

/// @brief Set the input to this parser.
/// @param SELF A pointer to this parser.
/// @param p A pointer to an array of @a l Bytes.
/// @param l The number of Bytes in the array pointed to by @a p.
/// @success The parser was assigned the input and is in the start state w.r.t. the specified input.
/// @method{dx_ddl_parser}
Core_Result dx_ddl_parser_set(dx_ddl_parser* SELF, char const* p, Core_Size l);

/// @internal
/// @brief Get the word type of the current word.
/// @param SELF A pointer to this parser.
/// @return The word type. This may be #dx_data_definition_language_word_kind_error. #dx_data_definition_language_word_kind_error is also returned on failure.
/// @method{dx_ddl_parser}
Core_Result dx_ddl_parser_get_word_kind(dx_data_definition_language_word_kind* RETURN, dx_ddl_parser const* SELF);

/// @brief Run this parser.
/// @param RETURN A pointer to a <code>dx_ddl_node*</code> variable.
/// @param SELF A pointer to this parser.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the <code>dx_ddl_node</code> of the run.
/// The caller acquired a reference to that object.
/// @method{dx_ddl_parser}
Core_Result dx_ddl_parser_run(dx_ddl_node** RETURN, dx_ddl_parser* SELF);

#if defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS) && DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS

Core_Result dx_ddl_parser_tests();

#endif // DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS

#endif // DX_DATA_DEFINITION_LANGUAGE_PARSER_H_INCLUDED
