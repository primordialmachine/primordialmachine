#include "dx/data_definition_language/word.h"

DX_DEFINE_OBJECT_TYPE("dx.ddl.word",
                      dx_ddl_word,
                      dx_object);

static void dx_ddl_word_destruct(dx_ddl_word* SELF) {
  DX_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_ddl_word_dispatch_construct(dx_ddl_word_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_ddl_word_construct(dx_ddl_word* SELF, dx_data_definition_language_word_kind kind, dx_string* text) {
  dx_rti_type* TYPE = dx_ddl_word_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!SELF || !text) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ddl_word_create(dx_ddl_word** RETURN, dx_data_definition_language_word_kind kind, dx_string* text) {
  DX_CREATE_PREFIX(dx_ddl_word)
  if (dx_ddl_word_construct(SELF, kind, text)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
