/// @file dx/ddl/scanner.h
/// @brief Lexical analyser (aka scanner aka lexer) for Data Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_SCANNER_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_SCANNER_H_INCLUDED

#include "dx/data_definition_language/diagnostics.h"
#include "dx/data_definition_language/word_kind.h"
#include "dX/data_definition_language/word.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A scanner used for scanning programs of the Data Definition Language.
Core_declareObjectType("dx.data_definition_language.scanner",
                       dx_data_definition_language_scanner,
                       dx_scanner);

static inline dx_data_definition_language_scanner* DX_DATA_DEFINITION_LANGUAGE_SCANNER(void* p) {
  return (dx_data_definition_language_scanner*)p;
}

struct dx_data_definition_language_scanner {
  Core_Scanner _parent;

  /// @brief A pointer to the diagnostics used by this scanner.
  dx_data_definition_language_diagnostics* diagnostics;

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
  Core_InlineArrayListN8 text;
  /// @brief The kind of the current word.
  dx_data_definition_language_word_kind kind;
};

static inline dx_data_definition_language_scanner_Dispatch* DX_DATA_DEFINITION_LANGUAGE_SCANNER_DISPATCH(void* p) {
  return (dx_data_definition_language_scanner_Dispatch*)p;
}

struct dx_data_definition_language_scanner_Dispatch {
  Core_Scanner_Dispatch _parent;
};

/// @brief Construct this scanner with an empty input.
/// @param diagnostics A pointer to the diagnostics used by this scanner.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @constructor{dx_data_definition_language_scanner}
Core_Result dx_data_definition_language_scanner_construct(dx_data_definition_language_scanner* SELF, dx_data_definition_language_diagnostics* diagnostics);

/// @brief Create this scanner with an empty input.
/// @param diagnostics A pointer to the diagnostics used by this scanner.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @create-operator{dx_data_definition_language_scanner}
Core_Result dx_data_definition_language_scanner_create(dx_data_definition_language_scanner** RETURN, dx_data_definition_language_diagnostics* diagnostics);

/// @brief Get the type of the current word.
/// @param RETURN A pointer to a <code>dx_data_definition_language_word_kind</code> variable.
/// @success <code>*RETURN</code> was assigned the word kind.
/// @method{dx_data_definition_language_scanner}
Core_Result dx_data_definition_language_scanner_get_word_kind(dx_data_definition_language_word_kind* RETURN, dx_data_definition_language_scanner const* SELF);

#endif // DX_DATA_DEFINITION_LANGUAGE_SCANNER_H_INCLUDED
