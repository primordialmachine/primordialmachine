/// @file dx/document_definition_language/word_kind.h
/// @brief Enumeration of word kinds of the Document Definition Language (DDL).
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DOCUMENT_DEFINITION_LANGUAGE_WORD_KIND_H_INCLUDED)
#define DX_DOCUMENT_DEFINITION_LANGUAGE_WORD_KIND_H_INCLUDED

#include "dx/core.h"
#include "dx/document_definition_language/configuration.h"

/// @brief Enumeration of word kinds.
/// @details
/// The following section describes the lexicals of the Document Definition Language (TDL).
///
/// ## newline
/// The word <code>newline</code> are
/// @code
/// newline := '\\n' | '\\r' | '\\n\\r' | '\\r\\n'
/// @endcode
///
/// ## lines
/// A line starts at an offset and is zero or more Bytes long.
/// It is terminated by a newline or by an end of the input.
/// The terminating newline belongs to the line.
///
DX_DECLARE_ENUMERATION_TYPE("dx.document_definition_language.word_kind",
                            dx_document_definition_language_word_kind);

enum dx_document_definition_language_word_kind {
#define DEFINE(SYMBOL, STRING) \
  SYMBOL,
#include "dx/document_definition_language/word_kind.i"
#undef DEFINE
};

dx_result dx_document_definition_language_word_kind_to_string(dx_string** RETURN, dx_document_definition_language_word_kind SELF);

#endif // DX_DOCUMENT_DEFINITION_LANGUAGE_WORD_KIND_H_INCLUDED
