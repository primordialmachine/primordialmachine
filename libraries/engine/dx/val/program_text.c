#include "dx/val/program_text.h"

DX_DEFINE_OBJECT_TYPE("dx.val.program.text",
                      dx_val_program_text,
                      Core_Object);

static void dx_val_program_text_destruct(dx_val_program_text* program_text) {
  switch (program_text->type) {
    case DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT: {
      DX_UNREFERENCE(program_text->vertex_program_text);
      program_text->vertex_program_text = NULL;
      DX_UNREFERENCE(program_text->fragment_program_text);
      program_text->fragment_program_text = NULL;
    } break;
    case DX_VAL_PROGRAM_TEXT_TYPE_VERTEX:
    case DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT: {
      DX_UNREFERENCE(program_text->program_text);
      program_text->program_text = NULL;
    } break;
  };
}

static void dx_val_program_text_constructDispatch(dx_val_program_text_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_val_program_text_construct_from_file(dx_val_program_text* SELF, dx_val_program_text_type type, Core_String* path) {
  DX_CONSTRUCT_PREFIX(dx_val_program_text);

  if (dx_string_contains_symbol(path, '\0')) {
    return Core_Failure;
  }
  Core_String* format = NULL;
  if (Core_String_create(&format, "${s}\0", sizeof("${s}\0"))) {
    return Core_Failure;
  }
  Core_String* path1 = NULL;
  if (Core_String_printf(&path1, format, path)) {
    DX_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  char* bytes; Core_Size number_of_bytes;
  if (dx_get_file_contents(path1->bytes, &bytes, &number_of_bytes)) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(path1);
  path1 = NULL;
  SELF->program_text = NULL;
  if (Core_String_create(&SELF->program_text, bytes, number_of_bytes)) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    return Core_Failure;
  }
  Core_Memory_deallocate(bytes);
  bytes = NULL;
  SELF->parent = NULL;
  SELF->type = type;
  
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_program_text_construct(dx_val_program_text* SELF, dx_val_program_text* vertex_program_text, dx_val_program_text* fragment_program_text) {
  DX_CONSTRUCT_PREFIX(dx_val_program_text);

  if (!vertex_program_text || DX_VAL_PROGRAM_TEXT_TYPE_VERTEX != vertex_program_text->type) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }

  if (!fragment_program_text || DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT != fragment_program_text->type) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  
  SELF->vertex_program_text = vertex_program_text;
  DX_REFERENCE(vertex_program_text);
  SELF->vertex_program_text->parent = SELF;

  SELF->fragment_program_text = fragment_program_text;
  DX_REFERENCE(fragment_program_text);
  SELF->fragment_program_text->parent = SELF;
  
  SELF->type = DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT;
  
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_program_text_create_from_file(dx_val_program_text** RETURN, dx_val_program_text_type type, Core_String* path) {
  DX_CREATE_PREFIX(dx_val_program_text);
  if (dx_val_program_text_construct_from_file(SELF, type, path)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_program_text_create(dx_val_program_text** RETURN, dx_val_program_text *vertex_program_text, dx_val_program_text* fragment_program_text) {
  DX_CREATE_PREFIX(dx_val_program_text);
  if (dx_val_program_text_construct(SELF, vertex_program_text, fragment_program_text)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;  
}

static Core_Result add_define(dx_val_program_text* SELF, Core_String* name) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "#define ${s} (1)\n${s}", sizeof("#define ${s} (1)\n${s}"))) {
    return Core_Failure;
  }
  Core_String* program_text = NULL;
  if (Core_String_printf(&program_text, format, name, SELF->program_text)) {
    DX_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  DX_UNREFERENCE(SELF->program_text);
  SELF->program_text = program_text;
  return Core_Success;
}

Core_Result dx_val_program_text_add_define(dx_val_program_text* SELF, Core_String* name) {
  switch (SELF->type) {
    case DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT: {
      if (add_define(SELF, name)) {
        return 1;
      }
      dx_val_program_text* p = SELF->parent;
      if (p) {
        p = p->vertex_program_text;
        if (add_define(p, name)) {
          return Core_Failure;
        }
      }
      return 0;
    } break;
    case DX_VAL_PROGRAM_TEXT_TYPE_VERTEX: {
      if (add_define(SELF, name)) {
        return 1;
      }
      dx_val_program_text* p = SELF->parent;
      if (p) {
        p = p->fragment_program_text;
        if (add_define(p, name)) {
          return 1;
        }
      }
      return 0;
    } break;
    case DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT: {
      if (add_define(SELF->vertex_program_text, name)) {
        return 1;
      }
      return add_define(SELF->fragment_program_text, name);
    } break;
  };
  return Core_Success;
}
