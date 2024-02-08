/// @file dx/scanner.h
/// @brief The base of a lexical analyser (aka scanner, aka lexer, ...).
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(CORE_SCANNER_H_INCLUDED)
#define CORE_SCANNER_H_INCLUDED

#include "dx/core/object.h"

/// @brief The base of a lexical analyser (aka scanner, aka lexer, ...).
/// A scanner provides
/// - the bytes of the scanned text (getWordTextBytes, getWordTextNumberOfBytes).
/// - the start offset and the end offset, in Bytes, of the scanned scanned text
Core_declareObjectType("Core.Scanner",
                       Core_Scanner,
                       Core_Object);

static inline Core_Scanner* CORE_SCANNER(void* p) {
  return (Core_Scanner*)p;
}

struct Core_Scanner {
  Core_Object _parent;
};

static inline Core_Scanner_Dispatch* CORE_SCANNER_DISPATCH(void* p) {
  return (Core_Scanner_Dispatch*)p;
}

struct Core_Scanner_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*setInput)(Core_Scanner* SELF, char const* bytes, Core_Size number_of_bytes);
  Core_Result(*step)(Core_Scanner* SELF);
  Core_Result(*getWordTextBytes)(char const** RETURN, Core_Scanner* SELF);
  Core_Result(*getWordTextNumberOfBytes)(Core_Size* RETURN, Core_Scanner* SELF);
  Core_Result(*getWordStartOffset)(Core_Size* RETURN, Core_Scanner* SELF);
  Core_Result(*getWordEndOffset)(Core_Size* RETURN, Core_Scanner* SELF);
};

/// @brief Construct this Core_Scanner object.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @constructor{Core_Scanner}
Core_Result Core_Scanner_construct(Core_Scanner* SELF);

/// @brief Set the input to this scanner.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @success The scanner was assigned the input and is in the start state w.r.t. the specified input.
/// @method{Core_Scanner}
static inline Core_Result Core_Scanner_setInput(Core_Scanner* SELF, char const* bytes, Core_Size number_of_bytes) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, setInput, SELF, bytes, number_of_bytes);
}

/// @brief Compute the next word.
/// @remarks The scanner does not advance if it has
/// - the current word is the end of the input word
/// - the current word is an error word
/// @method{Core_Scanner}
static inline Core_Result Core_Scanner_step(Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, step, SELF);
}

/// @brief Get the text of the current word.
/// @param RETURN A pointer to a <code>char const*</code> variable.
/// @success <code>*RETURN</code> was assigned pointer to the Bytes of the word text.
/// @method{Core_Scanner}
static inline Core_Result Core_Scanner_getWordTextBytes(char const** RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordTextBytes, RETURN, SELF);
}

/// @brief Get the length, in Bytes, of the text of the curren word.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the number of Bytes of the word text.
/// @method{Core_Scanner}
static inline Core_Result Core_Scanner_getWordTextNumberOfBytes(Core_Size* RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordTextNumberOfBytes, RETURN, SELF);
}

/// @brief Get the start offset of the current word.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the start offset of the current word.
/// @method{Core_Scanner}
static inline Core_Result Core_Scanner_getWordStartOffset(Core_Size* RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordStartOffset, RETURN, SELF);
}

/// @brief Get the end offset of the current word.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the end offset of the current word.
/// @method{Core_Scanner}
static inline Core_Result Core_Scanner_getWordEndOffset(Core_Size* RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordEndOffset, RETURN, SELF);
}

#endif // CORE_SCANNER_H_INCLUDED
