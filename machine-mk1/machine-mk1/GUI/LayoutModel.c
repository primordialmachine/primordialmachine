/// @file GUI/LayoutModel.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/LayoutModel.h"

static void Machine_GUI_LayoutModel_visit(Machine_GUI_LayoutModel* self)
{/*Intentionally empty.*/}

void Machine_GUI_LayoutModel_construct(Machine_GUI_LayoutModel* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);
  self->primaryDirection = Machine_GUI_Layout_Direction_Column;
  self->primaryJustification = Machine_GUI_Layout_Justification_Start;
  self->primaryInterChildSpacing = 0.f;
  Machine_setClassType((Machine_Object*)self, Machine_GUI_LayoutModel_getClassType());
}

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_LayoutModel, Machine_Object, &Machine_GUI_LayoutModel_visit, &Machine_GUI_LayoutModel_construct, NULL)

Machine_GUI_LayoutModel* Machine_GUI_LayoutModel_create() {
  Machine_ClassType* ty = Machine_GUI_LayoutModel_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Machine_GUI_LayoutModel* self = (Machine_GUI_LayoutModel*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}



Machine_GUI_Layout_Direction Machine_GUI_LayoutModel_getPrimaryDirection(const Machine_GUI_LayoutModel* self) {
  return self->primaryDirection;
}

void Machine_GUI_LayoutModel_setPrimaryDirection(Machine_GUI_LayoutModel* self, Machine_GUI_Layout_Direction primaryDirection) {
  self->primaryDirection = primaryDirection;
}



Machine_GUI_Layout_Justification Machine_GUI_LayoutModel_getPrimaryJustification(const Machine_GUI_LayoutModel* self) {
  return self->primaryJustification;
}

void Machine_GUI_LayoutModel_setPrimaryJustification(Machine_GUI_LayoutModel* self, Machine_GUI_Layout_Justification primaryJustifcation) {
  self->primaryJustification = primaryJustifcation;
}



Machine_Real Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(const Machine_GUI_LayoutModel* self) {
  return self->primaryInterChildSpacing;
}

void Machine_GUI_LayoutModel_setPrimaryInterChildspacing(Machine_GUI_LayoutModel* self, Machine_Real primaryInterChildSpacing) {
  self->primaryInterChildSpacing = primaryInterChildSpacing;
}
