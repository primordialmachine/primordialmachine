#if !defined(MACHINE_GUI_GDL_CONTEXT_H_INCLUDED)
#define MACHINE_GUI_GDL_CONTEXT_H_INCLUDED

#include "_Runtime.h"
#include "_GDL.h"

/// @brief A context.
/// @extends Machine.Object
MACHINE_DECLARE_CLASSTYPE(Machine_GUI_GDL_Context)

struct Machine_GUI_GDL_Context {
  Machine_Object parent;

  Machine_GDL_Context* context;
  
#define DEFINE(NAME,STRING) \
  Machine_String *NAME;
#include "./../../GUI/GDL/Context.i"
#undef DEFINE

};

/// @brief Create a GUI <-> GDL context.
/// @return The context.
Machine_GUI_GDL_Context* Machine_GUI_GDL_Context_create(Machine_GDL_Context *context);

#endif // MACHINE_GUI_GDL_CONTEXT_H_INCLUDED
