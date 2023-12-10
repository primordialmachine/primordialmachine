/// @file dx/document_definition_language/scanner.h
/// @brief Lexical analyser (aka scanner aka lexer) for the Document Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DOCUMENT_DEFINITION_LANGUAGE_SCANNER_H_INCLUDED)
#define DX_DOCUMENT_DEFINITION_LANGUAGE_SCANNER_H_INCLUDED

#include "dx/document_definition_language/word_kind.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A scanner used for scanning programs of the Document Definition Language (DDL).
/// The first token is the "start of input" token. The last token is the "end of input" or an "error" token.
/// 
/// A "step" of the scanner begins at the current offset.
/// If the current word is "end of input" or "error", the "step" ends.
/// 
/// Otherwise there are three cases:
/// 
/// 1.1)
/// The scanner consumes symbols until it encounters an "encoding error symbol".
/// It will create an "error" word.
/// That "error" word's Byte range includes all symbols CONSUMED so far in this run plus the Byte range of the "encoding error symbol".
/// 1.2)
/// The scanner consumes symbols until it consumes a sequence of newline symbols ('\n', '\r', '\n\r', '\r\n').
/// It will create a "line" word.
/// That "line" word's Byte range includes all symbols CONSUMED so far in this run.
/// 1.3)
/// The scanner reaches an "end of input" symbol and has consumed zero symbols before encountering that symbol.
/// The scanner creates an "end of input" word.
/// That "end of input" word's Byte range is the Byte range of the "end of input" symbol.
/// 1.4)
/// The scanner reaches and "end of input" symbol and has consumed one or more symbols before encountering that symbol.
/// It will create a "line" word.
/// That "line" word's Byte range includes all symbols CONSUMED so far in this run plus the Byte range of the "end of input" symbol.
DX_DECLARE_OBJECT_TYPE("dx.document_definition_language.scanner",
                       dx_document_definition_language_scanner,
                       dx_scanner);

static inline dx_document_definition_language_scanner* DX_DOCUMENT_DEFINITION_LANGUAGE_SCANNER(void* p) {
  return (dx_document_definition_language_scanner*)p;
}

struct dx_document_definition_language_scanner {
  dx_scanner _parent;

  /// @brief Pointer to the beginning of the first Byte of the input.
  char const* start;
  /// @brief Pointer to the end of the last Byte of the input.
  char const* end;
  /// @brief Pointer to the beginning of the current Byte.
  char const* current;
  
  /// @brief The range of the word.
  struct {
    /// @brief The start.
    char const* start;
    /// @brief The end.
    char const* end;
  } range;

  /// @brief The text of the current word.
  dx_inline_byte_array text;
  /// @brief The kind of the current word.
  dx_document_definition_language_word_kind kind;
};

static inline dx_document_definition_language_scanner_dispatch* DX_DOCUMENT_DEFINITION_LANGUAGE_SCANNER_DISPATCH(void* p) {
  return (dx_document_definition_language_scanner_dispatch*)p;
}

struct dx_document_definition_language_scanner_dispatch {
  dx_scanner_dispatch _parent;
};

/// @brief Construct this scanner with an empty input.
/// @param SELF A pointer to this scanner.
/// @param diagnostics A pointer to the diagnostics used by this scanner.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @method-call
Core_Result dx_document_definition_language_scanner_construct(dx_document_definition_language_scanner* SELF);

/// @brief Create this scanner with an empty input.
/// @param RETURN A pointer to a <code>dx_document_definition_language_scanner*</code> variable.
/// @method-call
/// @success
/// <code>*RETURN</code> was assigned a pointer to the dx_document_definition_language_scanner object.
/// The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @method-call
Core_Result dx_document_definition_language_scanner_create(dx_document_definition_language_scanner** RETURN);

/// @brief Get the type of the current word.
/// @param RETURN A pointer to a <code>dx_data_definition_language_word_kind</code> variable.
/// @param SELF a pointer to this scanner.
/// @success <code>*RETURN</code> was assigned the word kind.
/// @method-call
Core_Result dx_document_definition_language_scanner_get_word_kind(dx_document_definition_language_word_kind* RETURN, dx_document_definition_language_scanner const* SELF);

#endif // DX_DOCUMENT_DEFINITION_LANGUAGE_SCANNER_H_INCLUDED
