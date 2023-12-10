/// @file dx/scanner.h
/// @brief The base of a lexical analyser (aka scanner, aka lexer, ...).
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_CORE_SCANNER_H_INCLUDED)
#define DX_CORE_SCANNER_H_INCLUDED

#include "dx/core/object.h"

/// @brief The base of a lexical analyser (aka scanner, aka lexer, ...).
/// A scanner provides
/// - the bytes of the scanned text (get_word_text_bytes, get_word_text_number_of_bytes).
/// - the start offset and the end offset, in Bytes, of the scanned scanned text
DX_DECLARE_OBJECT_TYPE("dx.scanner",
                       dx_scanner,
                       Core_Object);

static inline dx_scanner* DX_SCANNER(void* p) {
  return (dx_scanner*)p;
}

struct dx_scanner {
  Core_Object _parent;
};

static inline dx_scanner_dispatch* DX_SCANNER_DISPATCH(void* p) {
  return (dx_scanner_dispatch*)p;
}

struct dx_scanner_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*set)(dx_scanner* SELF, char const* bytes, Core_Size number_of_bytes);
  Core_Result(*step)(dx_scanner* SELF);
  Core_Result(*get_word_text_bytes)(char const** RETURN, dx_scanner* SELF);
  Core_Result(*get_word_text_number_of_bytes)(Core_Size* RETURN, dx_scanner* SELF);
  Core_Result(*get_word_start_offset)(Core_Size* RETURN, dx_scanner* SELF);
  Core_Result(*get_word_end_offset)(Core_Size* RETURN, dx_scanner* SELF);
};

/// @brief Construct this dx_scanner object.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @constructor{dx_scanner}
Core_Result dx_scanner_construct(dx_scanner* SELF);

/// @brief Set the input to this scanner.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @success The scanner was assigned the input and is in the start state w.r.t. the specified input.
/// @method{dx_scanner}
static inline Core_Result dx_scanner_set(dx_scanner* SELF, char const* bytes, Core_Size number_of_bytes) {
  DX_OBJECT_VIRTUALCALL(dx_scanner, set, SELF, bytes, number_of_bytes);
}

/// @brief Compute the next word.
/// @remarks The scanner does not advance if it has
/// - the current word is the end of the input word
/// - the current word is an error word
/// @method{dx_scanner}
static inline Core_Result dx_scanner_step(dx_scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_scanner, step, SELF);
}

/// @brief Get the text of the current word.
/// @param RETURN A pointer to a <code>char const*</code> variable.
/// @success <code>*RETURN</code> was assigned pointer to the Bytes of the word text.
/// @method{dx_scanner}
static inline Core_Result dx_scanner_get_word_text_bytes(char const** RETURN, dx_scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_scanner, get_word_text_bytes, RETURN, SELF);
}

/// @brief Get the length, in Bytes, of the text of the curren word.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the number of Bytes of the word text.
/// @method{dx_scanner}
static inline Core_Result dx_scanner_get_word_text_number_of_bytes(Core_Size* RETURN, dx_scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_scanner, get_word_text_number_of_bytes, RETURN, SELF);
}

/// @brief Get the start offset of the current word.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the start offset of the current word.
/// @method{dx_scanner}
static inline Core_Result dx_scanner_get_word_start_offset(Core_Size* RETURN, dx_scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_scanner, get_word_start_offset, RETURN, SELF);
}

/// @brief Get the end offset of the current word.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the end offset of the current word.
/// @method{dx_scanner}
static inline Core_Result dx_scanner_get_word_end_offset(Core_Size* RETURN, dx_scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_scanner, get_word_end_offset, RETURN, SELF);
}

#endif // DX_CORE_SCANNER_H_INCLUDED
