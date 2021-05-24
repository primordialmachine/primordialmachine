/// @file GUI/Context.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/Context.h"


#include "./../GUI/WidgetList.h"


static void Machine_GUI_Context_visit(Machine_GUI_Context* self);

static void Machine_GUI_Context_construct(Machine_GUI_Context* self, size_t numberOfArguments, const Machine_Value* arguments);

MACHINE_DEFINE_CLASSTYPE(Machine_GUI_Context)
MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_Context, Machine_Object, &Machine_GUI_Context_visit, &Machine_GUI_Context_construct, NULL)

static void Machine_GUI_Context_visit(Machine_GUI_Context* self) {
  if (self->widgets) {
    Machine_visit(self->widgets);
  }
}

static void Machine_GUI_Context_construct(Machine_GUI_Context* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);
  self->widgets = Machine_GUI_WidgetList_create();
  Machine_setClassType((Machine_Object*)self, Machine_GUI_Context_getClassType());
}

Machine_GUI_Context* Machine_GUI_Context_create() {
  Machine_ClassType* ty = Machine_GUI_Context_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Machine_GUI_Context* self = (Machine_GUI_Context*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

void Machine_GUI_Context_appendChild(Machine_GUI_Context* self, Machine_GUI_Widget* widget) {
  Machine_GUI_WidgetList_append(self->widgets, widget);
}

Machine_GUI_Widget* Machine_GUI_Context_getChildAt(const Machine_GUI_Context* self, size_t index) {
  return Machine_GUI_WidgetList_getAt(self->widgets, index);
}

size_t Machine_GUI_Context_getNumberOfChildren(const Machine_GUI_Context* self) {
  return Machine_GUI_WidgetList_getSize(self->widgets);
}

void Machine_GUI_Context_render(Machine_GUI_Context* self, float width, float height) {
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self->widgets); i < n; ++i) {
    Machine_GUI_Widget* widget = (Machine_GUI_Widget*)Machine_GUI_WidgetList_getAt(self->widgets, i);
    Machine_GUI_Widget_render(widget, self, width, height);
  }
}

void Machine_GUI_Context_setCanvasSize(Machine_GUI_Context* self, float canvasWidth, float canvasHeight) {
  self->canvasWidth = canvasWidth;
  self->canvasHeight = canvasHeight;
}

float Machine_GUI_Context_getCanvasWidth(const Machine_GUI_Context* self) {
  return self->canvasWidth;
}

float Machine_GUI_Context_getCanvasHeight(const Machine_GUI_Context* self) {
  return self->canvasHeight;
}
