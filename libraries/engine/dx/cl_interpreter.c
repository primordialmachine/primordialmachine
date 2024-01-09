#include "dx/cl_interpreter.h"

#include <string.h>
#include "dx/application_presenter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.cl.interpreter_procedure",
                      dx_cl_interpreter_procedure,
                      Core_Object);

static void dx_cl_interpreter_procedure_destruct(dx_cl_interpreter_procedure* SELF) {
  CORE_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_cl_interpreter_procedure_constructDispatch(dx_cl_interpreter_procedure_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_cl_interpreter_procedure_construct(dx_cl_interpreter_procedure* SELF, Core_String* name, dx_cl_function* pointer) {
  DX_CONSTRUCT_PREFIX(dx_cl_interpreter_procedure);
  SELF->name = name;
  CORE_REFERENCE(name);
  SELF->pointer = pointer;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_cl_interpreter_procedure_create(dx_cl_interpreter_procedure** RETURN, Core_String* name, dx_cl_function* pointer) {
  DX_CREATE_PREFIX(dx_cl_interpreter_procedure)
  if (dx_cl_interpreter_procedure_construct(SELF, name, pointer)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.cl.interpreter",
                      dx_cl_interpreter,
                      Core_Object);

static void dx_cl_interpreter_destruct(dx_cl_interpreter* SELF) {
  Core_InlinePointerHashmap_uninitialize(&SELF->procedures);
}

static void dx_cl_interpreter_constructDispatch(dx_cl_interpreter_Dispatch* SELF)
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
  CORE_REFERENCE(*a);
}

static void on_key_removed(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  CORE_UNREFERENCE(*a);
}

static void on_value_added(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  CORE_REFERENCE(*a);
}

static void on_value_removed(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  CORE_UNREFERENCE(*a);
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
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }

  dx_cl_interpreter* cl_interpreter = NULL;
  if (dx_application_presenter_get_cl_interpreter(&cl_interpreter, application_presenter)) {
    CORE_UNREFERENCE(INDENT);
    INDENT = NULL;
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  dx_object_array* procedures = NULL;
  if (dx_cl_interpreter_get_functions(&procedures, cl_interpreter)) {
    CORE_UNREFERENCE(cl_interpreter);
    cl_interpreter = NULL;
    CORE_UNREFERENCE(INDENT);
    INDENT = NULL;
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(cl_interpreter);
  cl_interpreter = NULL;
  dx_console* console = NULL;
  if (dx_application_presenter_get_console(&console, application_presenter)) {
    CORE_UNREFERENCE(procedures);
    procedures = NULL;
    CORE_UNREFERENCE(INDENT);
    INDENT = NULL;
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  Core_String* text = NULL;
  if (Core_String_create(&text, "available commands:\n", strlen("available commands:\n"))) {
    CORE_UNREFERENCE(procedures);
    procedures = NULL;
    CORE_UNREFERENCE(console);
    console = NULL;
    CORE_UNREFERENCE(INDENT);
    INDENT = NULL;
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  if (dx_console_append_output_text(console, text)) {
    CORE_UNREFERENCE(text);
    text = NULL;
    CORE_UNREFERENCE(procedures);
    procedures = NULL;
    CORE_UNREFERENCE(console);
    console = NULL;
    CORE_UNREFERENCE(INDENT);
    INDENT = NULL;
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(text);
  text = NULL;
  Core_Size size;
  if (dx_object_array_get_size(&size, procedures)) {
    CORE_UNREFERENCE(procedures);
    procedures = NULL;
    CORE_UNREFERENCE(console);
    console = NULL;
    CORE_UNREFERENCE(INDENT);
    INDENT = NULL;
    CORE_UNREFERENCE(NEWLINE);
    NEWLINE = NULL;
    return Core_Failure;
  }
  for (Core_Size i = 0, n = size; i < n; ++i) {
    dx_cl_interpreter_procedure* p = NULL;
    if (dx_object_array_get_at((Core_Object**) & p, procedures, i)) {
      CORE_UNREFERENCE(procedures);
      procedures = NULL;
      CORE_UNREFERENCE(console);
      console = NULL;
      CORE_UNREFERENCE(INDENT);
      INDENT = NULL;
      CORE_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
    if (dx_console_append_output_text(console, INDENT)) {
      CORE_UNREFERENCE(procedures);
      procedures = NULL;
      CORE_UNREFERENCE(console);
      console = NULL;
      CORE_UNREFERENCE(INDENT);
      INDENT = NULL;
      CORE_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
    if (dx_console_append_output_text(console, p->name)) {
      CORE_UNREFERENCE(procedures);
      procedures = NULL;
      CORE_UNREFERENCE(console);
      console = NULL;
      CORE_UNREFERENCE(INDENT);
      INDENT = NULL;
      CORE_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
    if (dx_console_append_output_text(console, NEWLINE)) {
      CORE_UNREFERENCE(procedures);
      procedures = NULL;
      CORE_UNREFERENCE(console);
      console = NULL;
      CORE_UNREFERENCE(INDENT);
      INDENT = NULL;
      CORE_UNREFERENCE(NEWLINE);
      NEWLINE = NULL;
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(procedures);
  procedures = NULL;
  CORE_UNREFERENCE(console);
  console = NULL;
  CORE_UNREFERENCE(INDENT);
  INDENT = NULL;
  CORE_UNREFERENCE(NEWLINE);
  NEWLINE = NULL;
  return Core_Success;
}

Core_Result dx_cl_interpreter_construct(dx_cl_interpreter* SELF) {
  DX_CONSTRUCT_PREFIX(dx_cl_interpreter);
  Core_InlinePointerHashMap_Configuration configuration = {
    .compareKeysCallback = (Core_InlinePointerHashmap_compare_keys_callback*)&on_compare_keys,
    .hashKeyCallback = (Core_InlinePointerHashmap_hash_key_callback*)&on_hash_key,
    .keyAddedCallback = (Core_InlinePointerHashMap_KeyAddedCallback*)&on_key_added,
    .keyRemovedCallback = (Core_InlinePointerHashMap_KeyRemovedCallback*)&on_key_removed,
    .valueAddedCallback = (Core_InlinePointerHashmap_ValueAddedCallback*)&on_value_added,
    .valueRemovedCallback = (Core_InlinePointerHashMap_ValueRemovedCallback*)&on_value_removed,
  };
  if (Core_InlinePointerHashmap_initialize(&SELF->procedures, &configuration)) {
    return Core_Failure;
  }
  Core_String* name = NULL;
  if (Core_String_create(&name, "help", strlen("help"))) {
    Core_InlinePointerHashmap_uninitialize(&SELF->procedures);
    return Core_Failure;
  }
  dx_cl_interpreter_procedure* p = NULL;
  if (dx_cl_interpreter_procedure_create(&p, name, &help)) {
    CORE_UNREFERENCE(name);
    name = NULL;
    Core_InlinePointerHashmap_uninitialize(&SELF->procedures);
    return Core_Failure;
  }
  if (Core_InlinePointerHashmap_set(&SELF->procedures, name, p)) {
    CORE_UNREFERENCE(name);
    name = NULL;
    Core_InlinePointerHashmap_uninitialize(&SELF->procedures);
    return Core_Failure;
  }
  CORE_UNREFERENCE(name);
  name = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_cl_interpreter_create(dx_cl_interpreter** RETURN) {
  DX_CREATE_PREFIX(dx_cl_interpreter);
  if (dx_cl_interpreter_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_cl_interpreter_execute(dx_cl_interpreter* SELF, dx_application_presenter* application_presenter, Core_String* name) {
  dx_cl_interpreter_procedure* p = NULL;
  if (Core_InlinePointerHashmap_get((Core_InlinePointerHashmap_Value*)&p, &SELF->procedures, name)) {
    return Core_Failure;
  }
  if (p->pointer(application_presenter)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_cl_interpreter_register_function(dx_cl_interpreter* SELF, Core_String* name, dx_cl_function* function) {
  dx_cl_interpreter_procedure* p = NULL;
  if (Core_InlinePointerHashmap_get((Core_InlinePointerHashmap_Value*)&p, &SELF->procedures, name)) {
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
  if (Core_InlinePointerHashmap_set(&SELF->procedures, name, p)) {
    CORE_UNREFERENCE(p);
    p = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(p);
  p = NULL;
  return Core_Success;
}

Core_Result dx_cl_interpreter_get_functions(dx_object_array** RETURN, dx_cl_interpreter* SELF) {
  dx_object_array* functions = NULL;
  Core_Size size;
  if (Core_InlinePointerHashmap_getSize(&size, &SELF->procedures)) {
    return Core_Failure;
  }
  if (dx_object_array_create(&functions, size)) {
    return Core_Failure;
  }
  if (Core_InlinePointerHashmap_getValues(&SELF->procedures, &functions->backend.backend)) {
    return Core_Failure;
  }
  *RETURN = functions;
  return Core_Success;
}
