#include "dx/val/program_text.h"

DX_DEFINE_OBJECT_TYPE("dx.val.program.text",
                      dx_val_program_text,
                      dx_object);

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

static void dx_val_program_text_dispatch_construct(dx_val_program_text_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_val_program_text_construct_from_file(dx_val_program_text* SELF, dx_val_program_text_type type, dx_string* path) {
  dx_rti_type* TYPE = dx_val_program_text_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_string_contains_symbol(path, '\0')) {
    return DX_FAILURE;
  }
  dx_string* format = NULL;
  if (dx_string_create(&format, "${s}\0", sizeof("${s}\0"))) {
    return DX_FAILURE;
  }
  dx_string *path1 = dx_string_printf(format, path);
  DX_UNREFERENCE(format);
  if (!path1) {
    return DX_FAILURE;
  }
  char* bytes; dx_size number_of_bytes;
  if (dx_get_file_contents(path1->bytes, &bytes, &number_of_bytes)) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(path1);
  path1 = NULL;
  SELF->program_text = NULL;
  if (dx_string_create(&SELF->program_text, bytes, number_of_bytes)) {
    dx_memory_deallocate(bytes);
    bytes = NULL;
    return DX_FAILURE;
  }
  dx_memory_deallocate(bytes);
  bytes = NULL;
  SELF->parent = NULL;
  SELF->type = type;
  
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_program_text_construct(dx_val_program_text* SELF, dx_val_program_text* vertex_program_text, dx_val_program_text* fragment_program_text) {
  dx_rti_type* TYPE = dx_val_program_text_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!vertex_program_text || DX_VAL_PROGRAM_TEXT_TYPE_VERTEX != vertex_program_text->type) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!fragment_program_text || DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT != fragment_program_text->type) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  
  SELF->vertex_program_text = vertex_program_text;
  DX_REFERENCE(vertex_program_text);
  SELF->vertex_program_text->parent = SELF;

  SELF->fragment_program_text = fragment_program_text;
  DX_REFERENCE(fragment_program_text);
  SELF->fragment_program_text->parent = SELF;
  
  SELF->type = DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT;
  
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_program_text_create_from_file(dx_val_program_text** RETURN, dx_val_program_text_type type, dx_string* path) {
  DX_CREATE_PREFIX(dx_val_program_text)
  if (dx_val_program_text_construct_from_file(SELF, type, path)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_program_text_create(dx_val_program_text** RETURN, dx_val_program_text *vertex_program_text, dx_val_program_text* fragment_program_text) {
  DX_CREATE_PREFIX(dx_val_program_text)
  if (dx_val_program_text_construct(SELF, vertex_program_text, fragment_program_text)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;  
}

static dx_result add_define(dx_val_program_text* SELF, dx_string* name) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "#define ${s} (1)\n${s}", sizeof("#define ${s} (1)\n${s}"))) {
    return DX_FAILURE;
  }
  dx_string* program_text = dx_string_printf(format, name, SELF->program_text);
  DX_UNREFERENCE(format);
  format = NULL;
  if (!program_text) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(SELF->program_text);
  SELF->program_text = NULL;
  SELF->program_text = program_text;
  return DX_SUCCESS;
}

dx_result dx_val_program_text_add_define(dx_val_program_text* SELF, dx_string* name) {
  switch (SELF->type) {
    case DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT: {
      if (add_define(SELF, name)) {
        return 1;
      }
      dx_val_program_text* p = SELF->parent;
      if (p) {
        p = p->vertex_program_text;
        if (add_define(p, name)) {
          return DX_FAILURE;
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
  return DX_SUCCESS;
}
