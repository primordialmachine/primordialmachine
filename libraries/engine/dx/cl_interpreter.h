#if !defined(DX_CL_INTERPRETER_H_INCLUDED)
#define DX_CL_INTERPRETER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_application_presenter dx_application_presenter;
typedef Core_Result(dx_cl_function)(dx_application_presenter* application_presenter);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A CL interpreter procedure.
DX_DECLARE_OBJECT_TYPE("dx.cl.interpreter_procedure",
                       dx_cl_interpreter_procedure,
                       Core_Object);

static inline dx_cl_interpreter_procedure* DX_CL_INTERPRETER_PROCEDURE(void* p) {
  return (dx_cl_interpreter_procedure*)p;
}

struct dx_cl_interpreter_procedure {
  Core_Object _parent;
  Core_String* name;
  dx_cl_function* pointer;
};

static inline dx_cl_interpreter_procedure_dispatch* DX_CL_INTERPRETER_PROCEDURE_DISPATCH(void* p) {
  return (dx_cl_interpreter_procedure_dispatch*)p;
}

struct dx_cl_interpreter_procedure_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_cl_interpreter_procedure_construct(dx_cl_interpreter_procedure* SELF, Core_String* name, dx_cl_function* pointer);

Core_Result dx_cl_interpreter_procedure_create(dx_cl_interpreter_procedure** RETURN, Core_String* name, dx_cl_function* pointer);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The CL interpreter.
DX_DECLARE_OBJECT_TYPE("dx.cl.interpreter",
                       dx_cl_interpreter,
                       Core_Object);

static inline dx_cl_interpreter* DX_CL_INTERPRETER(void* p) {
  return (dx_cl_interpreter*)p;
}

struct dx_cl_interpreter {
  Core_Object _parent;
  dx_inline_pointer_hashmap procedures;
};

static inline dx_cl_interpreter_dispatch* DX_CL_INTERPRETER_DISPATCH(void* p) {
  return (dx_cl_interpreter_dispatch*)p;
}

struct dx_cl_interpreter_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_cl_interpreter_construct(dx_cl_interpreter* SELF);

Core_Result dx_cl_interpreter_create(dx_cl_interpreter** RETURN);

Core_Result dx_cl_interpreter_execute(dx_cl_interpreter* SELF, dx_application_presenter* application_presenter, Core_String* command);

Core_Result dx_cl_interpreter_register_function(dx_cl_interpreter* SELF, Core_String* name, dx_cl_function* function);

Core_Result dx_cl_interpreter_get_functions(dx_object_array** RETURN, dx_cl_interpreter* SELF);

#endif // DX_CL_INTERPRETER
