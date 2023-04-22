/// @file dx/ddl/word.h
/// @brief Output of the lexical analyses of the Data Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_WORD_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_WORD_H_INCLUDED

#include "dx/data_definition_language/word_kind.h"

/// @brief An word for programs of the Data Definition Language (DDL).
DX_DECLARE_OBJECT_TYPE("dx.ddl.word",
                       dx_ddl_word,
                       dx_object);

static inline dx_ddl_word* DX_DDL_WORD(void* p) {
  return (dx_ddl_word*)p;
}

struct dx_ddl_word {
  dx_object parent;
  dx_ddl_word_kind kind;
  dx_string* text;
};

static inline dx_ddl_word_dispatch* DX_DDL_WORD_DISPATCH(void* p) {
  return (dx_ddl_word_dispatch*)p;
}

struct dx_ddl_word_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this word with the specified word kind and the specified word text.
/// @param SELF A pointer to this dx_ddl_word object.
/// @param kind The kind of the word.
/// @param text The text of the word.
dx_result dx_ddl_word_construct(dx_ddl_word* SELF, dx_ddl_word_kind kind, dx_string* text);

/// @brief Create this word with the specified word kind and the specified word text.
/// @param RETURN A pointer to a <code>dx_ddl_word*</code> variable.
/// @param kind The kind of the word.
/// @param text The text of the word.
/// @success <code>*RETURN</code> was assigned a pointer to the dx_data_definition_language_word object.
/// @default-runtime-calling-convention
dx_result dx_ddl_word_create(dx_ddl_word** RETURN, dx_ddl_word_kind kind, dx_string* text);

#endif // DX_DATA_DEFINITION_LANGUAGE_WORD_H_INCLUDED
