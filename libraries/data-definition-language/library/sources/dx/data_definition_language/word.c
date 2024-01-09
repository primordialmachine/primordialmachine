#include "dx/data_definition_language/word.h"

Core_defineObjectType("dx.ddl.word",
                      dx_ddl_word,
                      Core_Object);

static void dx_ddl_word_destruct(dx_ddl_word* SELF) {
  CORE_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_ddl_word_constructDispatch(dx_ddl_word_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_ddl_word_construct(dx_ddl_word* SELF, dx_data_definition_language_word_kind kind, Core_String* text) {
  DX_CONSTRUCT_PREFIX(dx_ddl_word);
  if (!text) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ddl_word_create(dx_ddl_word** RETURN, dx_data_definition_language_word_kind kind, Core_String* text) {
  DX_CREATE_PREFIX(dx_ddl_word);
  if (dx_ddl_word_construct(SELF, kind, text)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
