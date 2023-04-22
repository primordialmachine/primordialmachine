#include "dx/cl_interpreter.h"

#include <string.h>
#include "dx/application_presenter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.cl.interpreter_procedure",
                      dx_cl_interpreter_procedure,
                      dx_object);

static void dx_cl_interpreter_procedure_destruct(dx_cl_interpreter_procedure* SELF) {
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_cl_interpreter_procedure_dispatch_construct(dx_cl_interpreter_procedure_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_cl_interpreter_procedure_construct(dx_cl_interpreter_procedure* SELF, dx_string* name, dx_cl_function* pointer) {
  dx_rti_type* TYPE = dx_cl_interpreter_procedure_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->name = name;
  DX_REFERENCE(name);
  SELF->pointer = pointer;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_cl_interpreter_procedure_create(dx_cl_interpreter_procedure** RETURN, dx_string* name, dx_cl_function* pointer) {
  dx_cl_interpreter_procedure* SELF = DX_CL_INTERPRETER_PROCEDURE(dx_object_alloc(sizeof(dx_cl_interpreter_procedure)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_cl_interpreter_procedure_construct(SELF, name, pointer)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.cl.interpreter",
                      dx_cl_interpreter,
                      dx_object);

static void dx_cl_interpreter_destruct(dx_cl_interpreter* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
}

static void dx_cl_interpreter_dispatch_construct(dx_cl_interpreter_dispatch* SELF)
{/*Intentionally empty.*/}

static void on_key_added(dx_object** a);

static void on_key_removed(dx_object** a);

static void on_value_added(dx_object** a);

static void on_value_removed(dx_object** a);

static dx_result on_hash_key(dx_size* RETURN,dx_object** a);

static dx_result on_compare_keys(dx_bool* RETURN, dx_object** a, dx_object** b);

static dx_result help(dx_application_presenter* application_presenter);

static void on_key_added(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_key_removed(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static void on_value_added(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_value_removed(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static dx_result on_hash_key(dx_size* RETURN, dx_object** a) {
  *RETURN = dx_string_get_hash_value(DX_STRING(*a));
  return DX_SUCCESS;
}

static dx_result on_compare_keys(dx_bool* RETURN, dx_object** a, dx_object** b) {
  *RETURN = dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
  return DX_SUCCESS;
}

static dx_result help(dx_application_presenter* application_presenter) {
  dx_string* NEWLINE = NULL,
           * INDENT = NULL;
  if (dx_string_create(&NEWLINE, "\n", sizeof("\n") - 1)) {
    return DX_FAILURE;
  }
  if (dx_string_create(&INDENT, "  ", sizeof("  ") - 1)) {
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }

  dx_cl_interpreter* cl_interpreter = NULL;
  if (dx_application_presenter_get_cl_interpreter(&cl_interpreter, application_presenter)) {
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }
  dx_object_array* procedures = NULL;
  if (dx_cl_interpreter_get_functions(&procedures, cl_interpreter)) {
    DX_UNREFERENCE(cl_interpreter);
    cl_interpreter = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(cl_interpreter);
  cl_interpreter = NULL;
  dx_console* console = NULL;
  if (dx_application_presenter_get_console(&console, application_presenter)) {
    DX_UNREFERENCE(procedures);
    procedures = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }
  dx_string* text = NULL;
  if (dx_string_create(&text, "available commands:\n", strlen("available commands:\n"))) {
    DX_UNREFERENCE(procedures);
    procedures = NULL;
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }
  if (dx_console_append_text(console, text)) {
    DX_UNREFERENCE(text);
    text = NULL;
    DX_UNREFERENCE(procedures);
    procedures = NULL;
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(text);
  text = NULL;
  dx_size size;
  if (dx_object_array_get_size(&size, procedures)) {
    DX_UNREFERENCE(procedures);
    procedures = NULL;
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return DX_FAILURE;
  }
  for (dx_size i = 0, n = size; i < n; ++i) {
    dx_cl_interpreter_procedure* p = NULL;
    if (dx_object_array_get_at((dx_object**) & p, procedures, i)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return DX_FAILURE;
    }
    if (dx_console_append_text(console, INDENT)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return DX_FAILURE;
    }
    if (dx_console_append_text(console, p->name)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return DX_FAILURE;
    }
    if (dx_console_append_text(console, NEWLINE)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return DX_FAILURE;
    }
  }
  DX_UNREFERENCE(procedures);
  procedures = NULL;
  DX_UNREFERENCE(console);
  console = NULL;
  DX_UNREFERENCE(INDENT);
  INDENT = NULL;
  DX_UNREFERENCE(NEWLINE);
  NEWLINE = NULL;
  return DX_SUCCESS;
}

dx_result dx_cl_interpreter_construct(dx_cl_interpreter* SELF) {
  dx_rti_type* TYPE = dx_cl_interpreter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION configuration = {
    .compare_keys_callback = (dx_result(*)(dx_bool*,void**,void**)) & on_compare_keys,
    .hash_key_callback = (dx_result(*)(dx_size*,void**)) & on_hash_key,
    .key_added_callback = (void(*)(void**)) & on_key_added,
    .key_removed_callback = (void(*)(void**)) & on_key_removed,
    .value_added_callback = (void(*)(void**)) &on_value_added,
    .value_removed_callback = (void(*)(void**)) &on_value_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->procedures, &configuration)) {
    return DX_FAILURE;
  }
  dx_string* name = NULL;
  if (dx_string_create(&name, "help", strlen("help"))) {
    dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
    return DX_FAILURE;
  }
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_cl_interpreter_procedure_create(&p, name, &help)) {
    DX_UNREFERENCE(name);
    name = NULL;
    dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->procedures, name, p)) {
    DX_UNREFERENCE(name);
    name = NULL;
    dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(name);
  name = NULL;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_cl_interpreter_create(dx_cl_interpreter** RETURN) {
  dx_cl_interpreter* SELF = DX_CL_INTERPRETER(dx_object_alloc(sizeof(dx_cl_interpreter)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_cl_interpreter_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_cl_interpreter_execute(dx_cl_interpreter* SELF, dx_application_presenter* application_presenter, dx_string* name) {
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_inline_pointer_hashmap_get((dx_inline_pointer_hashmap_value*)&p, &SELF->procedures, name)) {
    return DX_FAILURE;
  }
  if (p->pointer(application_presenter)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_cl_interpreter_register_function(dx_cl_interpreter* SELF, dx_string* name, dx_cl_function* function) {
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_inline_pointer_hashmap_get((dx_inline_pointer_hashmap_value*)&p, &SELF->procedures, name)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    }
  }
  if (p) {
    dx_set_error(DX_ERROR_EXISTS);
    return DX_FAILURE;
  }
  if (dx_cl_interpreter_procedure_create(&p, name, function)) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->procedures, name, p)) {
    DX_UNREFERENCE(p);
    p = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(p);
  p = NULL;
  return DX_SUCCESS;
}

dx_result dx_cl_interpreter_get_functions(dx_object_array** RETURN, dx_cl_interpreter* SELF) {
  dx_object_array* functions = NULL;
  dx_size size;
  if (dx_inline_pointer_hashmap_get_size(&size, &SELF->procedures)) {
    return DX_FAILURE;
  }
  if (dx_object_array_create(&functions, size)) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_get_values(&SELF->procedures, &functions->backend.backend)) {
    return DX_FAILURE;
  }
  *RETURN = functions;
  return DX_SUCCESS;
}
