/// @file dx/ddl/parser.h
/// @brief Syntactical functionality (aka "parsing") of Asset Description Language (ADL) files.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_PARSER_H_INCLUDED

#include "dx/ddl/diagnostics.h"

#include "dx/data_definition_language/word_kind.h"
#include "dx/ddl/word.h"
#include "dx/ddl/scanner.h"
#include "dx/data_definition_language/node_kind.h"
#include "dx/ddl/node.h"

/// @brief A parser used for parsing programs of the language "2023-06-01".
DX_DECLARE_OBJECT_TYPE("dx.ddl.parser",
                       dx_ddl_parser,
                       dx_object);

static inline dx_ddl_parser* DX_DATA_DEFINITION_LANGUAGE_PARSER(void* p) {
  return (dx_ddl_parser*)p;
}

struct dx_ddl_parser {
  dx_object _parent;
  /// @brief A pointer to the underlaying scanner.
  dx_data_definition_language_scanner* scanner;
  /// @brief A pointer to the underlaying diagnostics.
  dx_data_definition_language_diagnostics* diagnostics;
};

static inline dx_ddl_parser_dispatch* DX_DATA_DEFINITION_LANGUAGE_PARSER_DISPATCH(void* p) {
  return (dx_ddl_parser_dispatch*)p;
}

struct dx_ddl_parser_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this parser with an empty input.
/// @param SELF A pointer to this parser.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @success
/// The parser was assigned the empty input and is in the start state w.r.t. the specified input.
/// @method-call
dx_result dx_ddl_parser_construct(dx_ddl_parser* SELF, dx_data_definition_language_scanner* scanner, dx_data_definition_language_diagnostics* diagnostics);

/// @brief Create this parser with an empty input.
/// @param RETURN A pointer to a <code>dx_ddl_parser*</code> variable.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @return A pointer to the parser on success. The null pointer on failure.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the parser.
/// The parser was assigned the empty input and is in the start state w.r.t. the specified input.
/// @method-call
dx_result dx_ddl_parser_create(dx_ddl_parser** RETURN, dx_data_definition_language_scanner* scanner, dx_data_definition_language_diagnostics* diagnostics);

/// @brief Set the input to this parser.
/// @param SELF A pointer to this parser.
/// @param p A pointer to an array of @a l Bytes.
/// @param l The number of Bytes in the array pointed to by @a p.
/// @success The parser was assigned the input and is in the start state w.r.t. the specified input.
/// @method-call
dx_result dx_ddl_parser_set(dx_ddl_parser* SELF, char const* p, dx_size l);

/// @brief Get if the current word is of the specified word type.
/// @param SELF A pointer to this parser.
/// @param word_kind The word type.
/// @return @a true if the current word is of the specified word type. @a false if it is not. @a false is also returned on failure.
/// @method-call
bool dx_ddl_parser_is_word_kind(dx_ddl_parser const* SELF, dx_ddl_word_kind word_kind);

/// @internal
/// @brief Get the word type of the current word.
/// @param SELF A pointer to this parser.
/// @return The word type. This may be #dx_ddl_word_kind_error. #dx_ddl_word_kind_error is also returned on failure.
/// @method-call
dx_result dx_ddl_parser_get_word_kind(dx_ddl_word_kind* RETURN, dx_ddl_parser const* SELF);

/// @brief Run this parser.
/// @param RETURN A pointer to a <code>dx_ddl_node*</code> variable.
/// @param SELF A pointer to this parser.
/// @success <code>*RETURN</code> was assigned a pointer to the <code>dx_ddl_node</code> of the run.
/// @method-call
dx_result dx_ddl_parser_run(dx_ddl_node** RETURN, dx_ddl_parser* SELF);

#if defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS) && DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS

dx_result dx_ddl_parser_tests();

#endif // DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS

#endif // DX_DATA_DEFINITION_LANGUAGE_PARSER_H_INCLUDED
