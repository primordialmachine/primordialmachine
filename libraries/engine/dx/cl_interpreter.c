#include "dx/cl_interpreter.h"

#include <string.h>
#include "dx/application_presenter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.cl.interpreter_procedure",
                      dx_cl_interpreter_procedure,
                      Core_Object);

static void dx_cl_interpreter_procedure_destruct(dx_cl_interpreter_procedure* SELF) {
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_cl_interpreter_procedure_constructDispatch(dx_cl_interpreter_procedure_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_cl_interpreter_procedure_construct(dx_cl_interpreter_procedure* SELF, Core_String* name, dx_cl_function* pointer) {
  DX_CONSTRUCT_PREFIX(dx_cl_interpreter_procedure);
  SELF->name = name;
  DX_REFERENCE(name);
  SELF->pointer = pointer;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_cl_interpreter_procedure_create(dx_cl_interpreter_procedure** RETURN, Core_String* name, dx_cl_function* pointer) {
  DX_CREATE_PREFIX(dx_cl_interpreter_procedure)
  if (dx_cl_interpreter_procedure_construct(SELF, name, pointer)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.cl.interpreter",
                      dx_cl_interpreter,
                      Core_Object);

static void dx_cl_interpreter_destruct(dx_cl_interpreter* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
}

static void dx_cl_interpreter_constructDispatch(dx_cl_interpreter_dispatch* SELF)
{/*Intentionally empty.*/}

static void on_key_added(Core_Object** a);

static void on_key_removed(Core_Object** a);

static void on_value_added(Core_Object** a);

static void on_value_removed(Core_Object** a);

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a);

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static Core_Result help(dx_application_presenter* application_presenter);

static void on_key_added(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_key_removed(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static void on_value_added(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_value_removed(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result help(dx_application_presenter* application_presenter) {
  Core_String* NEWLINE = NULL,
           * INDENT = NULL;
  if (Core_String_create(&NEWLINE, "\n", sizeof("\n") - 1)) {
    return Core_Failure;
  }
  if (Core_String_create(&INDENT, "  ", sizeof("  ") - 1)) {
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }

  dx_cl_interpreter* cl_interpreter = NULL;
  if (dx_application_presenter_get_cl_interpreter(&cl_interpreter, application_presenter)) {
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  dx_object_array* procedures = NULL;
  if (dx_cl_interpreter_get_functions(&procedures, cl_interpreter)) {
    DX_UNREFERENCE(cl_interpreter);
    cl_interpreter = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
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
    return Core_Failure;
  }
  Core_String* text = NULL;
  if (Core_String_create(&text, "available commands:\n", strlen("available commands:\n"))) {
    DX_UNREFERENCE(procedures);
    procedures = NULL;
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  if (dx_console_append_output_text(console, text)) {
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
    return Core_Failure;
  }
  DX_UNREFERENCE(text);
  text = NULL;
  Core_Size size;
  if (dx_object_array_get_size(&size, procedures)) {
    DX_UNREFERENCE(procedures);
    procedures = NULL;
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(INDENT);
    INDENT = NULL;
    DX_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  for (Core_Size i = 0, n = size; i < n; ++i) {
    dx_cl_interpreter_procedure* p = NULL;
    if (dx_object_array_get_at((Core_Object**) & p, procedures, i)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
    if (dx_console_append_output_text(console, INDENT)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
    if (dx_console_append_output_text(console, p->name)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
    if (dx_console_append_output_text(console, NEWLINE)) {
      DX_UNREFERENCE(procedures);
      procedures = NULL;
      DX_UNREFERENCE(console);
      console = NULL;
      DX_UNREFERENCE(INDENT);
      INDENT = NULL;
      DX_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
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
  return Core_Success;
}

Core_Result dx_cl_interpreter_construct(dx_cl_interpreter* SELF) {
  DX_CONSTRUCT_PREFIX(dx_cl_interpreter);
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&on_compare_keys,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&on_hash_key,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&on_key_added,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&on_key_removed,
    .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&on_value_added,
    .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&on_value_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->procedures, &configuration)) {
    return Core_Failure;
  }
  Core_String* name = NULL;
  if (Core_String_create(&name, "help", strlen("help"))) {
    dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
    return Core_Failure;
  }
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_cl_interpreter_procedure_create(&p, name, &help)) {
    DX_UNREFERENCE(name);
    name = NULL;
    dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->procedures, name, p)) {
    DX_UNREFERENCE(name);
    name = NULL;
    dx_inline_pointer_hashmap_uninitialize(&SELF->procedures);
    return Core_Failure;
  }
  DX_UNREFERENCE(name);
  name = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_cl_interpreter_create(dx_cl_interpreter** RETURN) {
  DX_CREATE_PREFIX(dx_cl_interpreter);
  if (dx_cl_interpreter_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_cl_interpreter_execute(dx_cl_interpreter* SELF, dx_application_presenter* application_presenter, Core_String* name) {
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_inline_pointer_hashmap_get((dx_inline_pointer_hashmap_value*)&p, &SELF->procedures, name)) {
    return Core_Failure;
  }
  if (p->pointer(application_presenter)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_cl_interpreter_register_function(dx_cl_interpreter* SELF, Core_String* name, dx_cl_function* function) {
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_inline_pointer_hashmap_get((dx_inline_pointer_hashmap_value*)&p, &SELF->procedures, name)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    }
  }
  if (p) {
    Core_setError(Core_Error_Exists);
    return Core_Failure;
  }
  if (dx_cl_interpreter_procedure_create(&p, name, function)) {
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->procedures, name, p)) {
    DX_UNREFERENCE(p);
    p = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(p);
  p = NULL;
  return Core_Success;
}

Core_Result dx_cl_interpreter_get_functions(dx_object_array** RETURN, dx_cl_interpreter* SELF) {
  dx_object_array* functions = NULL;
  Core_Size size;
  if (dx_inline_pointer_hashmap_get_size(&size, &SELF->procedures)) {
    return Core_Failure;
  }
  if (dx_object_array_create(&functions, size)) {
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_get_values(&SELF->procedures, &functions->backend.backend)) {
    return Core_Failure;
  }
  *RETURN = functions;
  return Core_Success;
}
