#include "./../../GUI/GDL/Context.h"

#include <string.h>

static void Machine_GUI_GDL_Context_visit(Machine_GUI_GDL_Context* self) {
  if (self->context) {
    Machine_visit(self->context);
  }
#define DEFINE(NAME, STRING) \
  if (self->NAME) { \
    Machine_visit(self->NAME); \
  }
#include "./../../GUI/GDL/Context.h"
#undef DEFINE
}

static void Machine_GUI_GDL_Context_construct(Machine_GUI_GDL_Context* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);

  self->context = (Machine_GDL_Context *)Machine_Value_getObject(&arguments[0]);
  
#define DEFINE(NAME,STRING) \
  self->NAME = Machine_String_create(STRING, strlen(STRING));
#include "./../../GUI/GDL/Context.i"
#undef DEFINE

  Machine_setClassType((Machine_Object*)self, Machine_GUI_GDL_Context_getClassType());
}

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_GDL_Context, Machine_Object, &Machine_GUI_GDL_Context_visit, &Machine_GUI_GDL_Context_construct, NULL)

Machine_GUI_GDL_Context* Machine_GUI_GDL_Context_create(Machine_GDL_Context *context) {
  Machine_ClassType* ty = Machine_GUI_GDL_Context_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 1;
  Machine_Value ARGUMENTS[1];
  Machine_Value_setObject(&ARGUMENTS[0], (Machine_Object *)context);
  Machine_GUI_GDL_Context* self = (Machine_GUI_GDL_Context*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}
