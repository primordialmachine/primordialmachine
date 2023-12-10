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
                       Core_Object);

static inline dx_ddl_word* DX_DDL_WORD(void* p) {
  return (dx_ddl_word*)p;
}

struct dx_ddl_word {
  Core_Object _parent;
  dx_data_definition_language_word_kind kind;
  Core_String* text;
};

static inline dx_ddl_word_dispatch* DX_DDL_WORD_DISPATCH(void* p) {
  return (dx_ddl_word_dispatch*)p;
}

struct dx_ddl_word_dispatch {
  Core_Object_Dispatch _parent;
};

/// @details The word is constructed with the specified word kind and the specified word text.
/// @param kind The kind of the word.
/// @param text The text of the word.
/// @constructor{dx_ddl_word}
Core_Result dx_ddl_word_construct(dx_ddl_word* SELF, dx_data_definition_language_word_kind kind, Core_String* text);

/// @details The word is created with the specified word kind and the specified word text.
/// @param kind The kind of the word.
/// @param text The text of the word.
/// @create-operator{dx_ddl_word}
Core_Result dx_ddl_word_create(dx_ddl_word** RETURN, dx_data_definition_language_word_kind kind, Core_String* text);

#endif // DX_DATA_DEFINITION_LANGUAGE_WORD_H_INCLUDED
