/// @file GDL/Syntactical/Node.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../../GDL/Syntactical/Node.h"

static void Machine_GDL_Node_visit(Machine_GDL_Node* self);

static void Machine_GDL_Node_construct(Machine_GDL_Node* self, size_t numberOfArguments, const Machine_Value* arguments);

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GDL_Node, Machine_Object, &Machine_GDL_Node_visit, &Machine_GDL_Node_construct, NULL)

static void Machine_GDL_Node_visit(Machine_GDL_Node* self) {
  if (self->children) {
    Machine_visit(self->children);
  }
  if (self->text) {
    Machine_visit(self->text);
  }
}

static void Machine_GDL_Node_construct(Machine_GDL_Node* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);
  self->kind = Machine_Value_getInteger(&(arguments[0]));
  self->text = Machine_Value_getString(&(arguments[1]));
  self->children = Machine_List_create();
  self->parent = NULL;
  Machine_setClassType((Machine_Object*)self, Machine_GDL_Node_getClassType());
}

Machine_GDL_Node* Machine_GDL_Node_create(Machine_GDL_NodeKind kind, Machine_String* text) {
  Machine_ClassType* ty = Machine_GDL_Node_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 2;
  Machine_Value ARGUMENTS[2];
  Machine_Value_setInteger(&(ARGUMENTS[0]), kind);
  Machine_Value_setString(&(ARGUMENTS[1]), text);
  Machine_GDL_Node* self = (Machine_GDL_Node*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}
