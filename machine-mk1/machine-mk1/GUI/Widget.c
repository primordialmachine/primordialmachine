/// @file GUI/Widget.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/Widget.h"


#include "./../GUI/WidgetList.h"


static void Machine_GUI_Widget_visit(Machine_GUI_Widget* self);

MACHINE_DEFINE_CLASSTYPE(Machine_GUI_Widget)
MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_Widget, Machine_Object, &Machine_GUI_Widget_visit, &Machine_GUI_Widget_construct, NULL)

static void Machine_GUI_Widget_visit(Machine_GUI_Widget *self) {
  if (self->name) {
    Machine_visit(self->name);
  }
  if (self->children) {
    Machine_visit(self->children);
  }
}

void Machine_GUI_Widget_construct(Machine_GUI_Widget* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);
  self->name = NULL;
  self->children = Machine_GUI_WidgetList_create();
  Machine_setClassType((Machine_Object*)self, Machine_GUI_Widget_getClassType());
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_render(Machine_GUI_Widget* self, Machine_GUI_Context* context, float width, float height) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->render);
  self->render(self, context, width, height);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_setPosition(Machine_GUI_Widget* self, const Machine_Math_Vector2* position) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->setPosition);
  MACHINE_ASSERT_NOTNULL(position);
  self->setPosition(self, position);
}

const Machine_Math_Vector2* Machine_GUI_Widget_getPosition(Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getPosition);
  return self->getPosition(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_setSize(Machine_GUI_Widget* self, const Machine_Math_Vector2* size) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->setSize);
  MACHINE_ASSERT_NOTNULL(size);
  self->setSize(self, size);
}

const Machine_Math_Vector2* Machine_GUI_Widget_getSize(Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  return self->getSize(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_setRectangle(Machine_GUI_Widget* self, const Machine_Math_Rectangle2* rectangle) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->setRectangle);
  MACHINE_ASSERT_NOTNULL(rectangle);
  self->setRectangle(self, rectangle);
}

const Machine_Math_Rectangle2* Machine_GUI_Widget_getRectangle(Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getRectangle);
  return self->getRectangle(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

const Machine_Math_Vector2* Machine_GUI_Widget_getPreferredSize(Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getPreferredSize);
  return self->getPreferredSize(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Machine_String* Machine_GUI_Widget_getName(Machine_GUI_Widget* self) {
  return self->name;
}

void Machine_GUI_Widget_setName(Machine_GUI_Widget* self, Machine_String* name) {
  self->name = name;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
