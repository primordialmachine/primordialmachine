/// @file GUI/TextLabel.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/TextLabel.h"
#include "./../Shape2.h"
#include "./../GUI/Widget.h"




struct Machine_GUI_TextLabel {
  Machine_GUI_Widget parent;

  /// @brief The layout for rendering the text.
  Machine_Text_Layout* foreground;
  
  /// @brief The shape for rendering the background.
  Machine_Rectangle2* background;

  bool childDirty;
};

static void Machine_GUI_TextLabel_visit(Machine_GUI_TextLabel* self) {
  if (self->foreground) {
    Machine_visit(self->foreground);
  }
  if (self->background) {
    Machine_visit(self->background);
  }
}

static void Machine_GUI_TextLabel_setPosition(Machine_GUI_TextLabel* self, const Machine_Math_Vector2* position);

static const Machine_Math_Vector2* Machine_GUI_TextLabel_getPosition(Machine_GUI_TextLabel* self);

static void Machine_GUI_TextLabel_setSize(Machine_GUI_TextLabel* self, const Machine_Math_Vector2* size);

static const Machine_Math_Vector2* Machine_GUI_TextLabel_getSize(Machine_GUI_TextLabel* self);

static void Machine_GUI_TextLabel_setRectangle(Machine_GUI_TextLabel* self, const Machine_Math_Rectangle2* rectangle);

static const Machine_Math_Rectangle2* Machine_GUI_TextLabel_getRectangle(Machine_GUI_TextLabel* self);

static const Machine_Math_Vector2* Machine_GUI_TextLabel_getPreferredSize(Machine_GUI_TextLabel* self);

static void Machine_GUI_TextLabel_render(Machine_GUI_TextLabel* self, Machine_GUI_Context* context, float width, float height);

void Machine_GUI_TextLabel_construct(Machine_GUI_TextLabel* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_GUI_Widget_construct((Machine_GUI_Widget*)self, numberOfArguments, arguments);
  Machine_Fonts_Font *font = Machine_Fonts_createFont("RobotoSlab-Regular.ttf", 20);
  self->foreground = Machine_Text_Layout_create(Machine_String_create("", strlen("")), font);
  self->background = Machine_Rectangle2_create();
  self->childDirty = true;
  ((Machine_GUI_Widget*)self)->render = (void (*)(Machine_GUI_Widget *, Machine_GUI_Context*, float, float))&Machine_GUI_TextLabel_render;
  ((Machine_GUI_Widget*)self)->setRectangle = (void (*)(Machine_GUI_Widget*, const Machine_Math_Rectangle2*)) & Machine_GUI_TextLabel_setRectangle;
  ((Machine_GUI_Widget*)self)->getRectangle = (const Machine_Math_Rectangle2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_TextLabel_getRectangle;
  ((Machine_GUI_Widget*)self)->setPosition= (void (*)(Machine_GUI_Widget*, const Machine_Math_Vector2*)) & Machine_GUI_TextLabel_setPosition;
  ((Machine_GUI_Widget*)self)->getPosition = (const Machine_Math_Vector2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_TextLabel_getPosition;
  ((Machine_GUI_Widget*)self)->setSize = (void (*)(Machine_GUI_Widget*, const Machine_Math_Vector2*)) & Machine_GUI_TextLabel_setSize;
  ((Machine_GUI_Widget*)self)->getSize = (const Machine_Math_Vector2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_TextLabel_getSize;
  ((Machine_GUI_Widget*)self)->getPreferredSize = (const Machine_Math_Vector2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_TextLabel_getPreferredSize;
  Machine_setClassType((Machine_Object*)self, Machine_GUI_TextLabel_getClassType());
}

MACHINE_DEFINE_CLASSTYPE(Machine_GUI_TextLabel)
MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_TextLabel, Machine_GUI_Widget, &Machine_GUI_TextLabel_visit, &Machine_GUI_TextLabel_construct, NULL)

Machine_GUI_TextLabel* Machine_GUI_TextLabel_create() {
  Machine_ClassType* ty = Machine_GUI_TextLabel_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Machine_GUI_TextLabel* self = (Machine_GUI_TextLabel*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static const Machine_Math_Vector2* Machine_GUI_TextLabel_getPreferredSize(Machine_GUI_TextLabel* self) {
  return Machine_Math_Rectangle2_getSize(Machine_Text_Layout_getBounds(self->foreground));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setText(Machine_GUI_TextLabel* self, Machine_String* text) {
  Machine_Text_Layout_setText(self->foreground, text);
  self->childDirty = true;
}

Machine_String* Machine_GUI_TextLabel_getText(Machine_GUI_TextLabel* self) {
  return Machine_Text_Layout_getText(self->foreground);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setBackgroundColor(Machine_GUI_TextLabel* self, const Machine_Math_Vector3* backgroundColor) {
  Machine_Rectangle2_setColor(self->background, backgroundColor);
  self->childDirty = true;
}

const Machine_Math_Vector3* Machine_GUI_TextLabel_getBackgroundColor(Machine_GUI_TextLabel* self) {
  return Machine_Rectangle2_getColor(self->background);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setForegroundColor(Machine_GUI_TextLabel* self, const Machine_Math_Vector3* foregroundColor) {
  Machine_Text_Layout_setColor(self->foreground, foregroundColor);
  self->childDirty = true;
}

const Machine_Math_Vector3* Machine_GUI_TextLabel_getForegroundColor(Machine_GUI_TextLabel* self) {
  return Machine_Text_Layout_getColor(self->foreground);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setPosition(Machine_GUI_TextLabel* self, const Machine_Math_Vector2* position) {
  Machine_Rectangle2_setPosition(self->background, position);
  self->childDirty = true;
}

const Machine_Math_Vector2* Machine_GUI_TextLabel_getPosition(Machine_GUI_TextLabel* self) {
  return Machine_Rectangle2_getPosition(self->background);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setSize(Machine_GUI_TextLabel* self, const Machine_Math_Vector2* size) {
  Machine_Rectangle2_setSize(self->background, size);
  self->childDirty = true;
}

const Machine_Math_Vector2* Machine_GUI_TextLabel_getSize(Machine_GUI_TextLabel* self) {
  return Machine_Rectangle2_getSize(self->background);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setRectangle(Machine_GUI_TextLabel* self, const Machine_Math_Rectangle2 *rectangle) {
  Machine_Rectangle2_setRectangle(self->background, rectangle);
  self->childDirty = true;
}

const Machine_Math_Rectangle2* Machine_GUI_TextLabel_getRectangle(Machine_GUI_TextLabel* self) {
  return Machine_Rectangle2_getRectangle(self->background);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_render(Machine_GUI_TextLabel* self, Machine_GUI_Context* context, float width, float height) {
  if (self->childDirty) {
    // TODO: Only do this layouting if necessary.
    Machine_Math_Rectangle2* clipRect = Machine_Rectangle2_getRectangle(self->background);
    const Machine_Math_Vector2* widgetCenter = Machine_Math_Rectangle2_getCenter(Machine_Rectangle2_getRectangle(self->background));

    const Machine_Math_Rectangle2* textBounds = Machine_Text_Layout_getBounds(self->foreground);
    const Machine_Math_Vector2* textCenter = Machine_Math_Rectangle2_getCenter(textBounds);
    Machine_Math_Vector2* delta = Machine_Math_Vector2_difference(widgetCenter, textCenter);
    const Machine_Math_Vector2* oldPosition = Machine_Text_Layout_getPosition(self->foreground);
    Machine_Math_Vector2* newPosition = Machine_Math_Vector2_sum(oldPosition, delta);
    Machine_Text_Layout_setPosition(self->foreground, newPosition);
    Machine_Text_Layout_setClipRectangle(self->foreground, clipRect);

    self->childDirty = false;
  }
  Machine_Shape2_render((Machine_Shape2 *)self->background, width, height);
  Machine_Text_Layout_render(self->foreground, width, height);
}
