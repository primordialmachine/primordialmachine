/// @file GUI/TextLabel.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/TextLabel.h"


#include "./../GUI/Context.h"
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

static const Machine_Math_Vector2* Machine_GUI_TextLabel_getPreferredSize(const Machine_GUI_TextLabel* self);

static void Machine_GUI_TextLabel_render(Machine_GUI_TextLabel* self, float width, float height);

static Machine_Value boundsChangedCallback(size_t numberOfArguments, const Machine_Value* arguments) {
  MACHINE_ASSERT(numberOfArguments == 1, Machine_Status_InvalidNumberOfArguments);
  Machine_GUI_TextLabel* self = (Machine_GUI_TextLabel*)Machine_Value_getObject(&arguments[0]);
  self->childDirty = true;
  Machine_Value result;
  Machine_Value_setVoid(&result, Machine_Void_Void);
  return result;
}

void Machine_GUI_TextLabel_construct(Machine_GUI_TextLabel* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_GUI_Widget_construct((Machine_GUI_Widget*)self, numberOfArguments, arguments);
  Machine_Fonts_Font* font = Machine_Fonts_createFont("RobotoSlab-Regular.ttf", 20);
  self->foreground = Machine_Text_Layout_create(Machine_String_create("", strlen("")), font);
  self->background = Machine_Rectangle2_create();
  self->childDirty = true;
  ((Machine_GUI_Widget*)self)->render = (void (*)(Machine_GUI_Widget*, float, float)) & Machine_GUI_TextLabel_render;
  ((Machine_GUI_Widget*)self)->getPreferredSize = (const Machine_Math_Vector2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_TextLabel_getPreferredSize;

  Machine_GUI_Widget_subscribe((Machine_GUI_Widget*)self, ((Machine_GUI_Widget*)self)->context->signalsContext->PositionChanged, (Machine_Object *)self, &boundsChangedCallback);
  Machine_GUI_Widget_subscribe((Machine_GUI_Widget*)self, ((Machine_GUI_Widget*)self)->context->signalsContext->SizeChanged, (Machine_Object*)self, &boundsChangedCallback);

  Machine_setClassType((Machine_Object*)self, Machine_GUI_TextLabel_getClassType());
}

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_TextLabel, Machine_GUI_Widget, &Machine_GUI_TextLabel_visit, &Machine_GUI_TextLabel_construct, NULL)

Machine_GUI_TextLabel* Machine_GUI_TextLabel_create(Machine_GUI_Context* context) {
  Machine_ClassType* ty = Machine_GUI_TextLabel_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 1;
  Machine_Value ARGUMENTS[1];
  Machine_Value_setObject(&ARGUMENTS[0], (Machine_Object*)context);
  Machine_GUI_TextLabel* self = (Machine_GUI_TextLabel*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static const Machine_Math_Vector2* Machine_GUI_TextLabel_getPreferredSize(const Machine_GUI_TextLabel* self) {
  return Machine_Math_Rectangle2_getSize(Machine_Text_Layout_getBounds(self->foreground));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setText(Machine_GUI_TextLabel* self, Machine_String* text) {
  Machine_Text_Layout_setText(self->foreground, text);
  self->childDirty = true;
}

Machine_String* Machine_GUI_TextLabel_getText(const Machine_GUI_TextLabel* self) {
  return Machine_Text_Layout_getText(self->foreground);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setBackgroundColor(Machine_GUI_TextLabel* self, const Machine_Math_Vector4* backgroundColor) {
  Machine_Rectangle2_setColor(self->background, backgroundColor);
  self->childDirty = true;
}

const Machine_Math_Vector4* Machine_GUI_TextLabel_getBackgroundColor(const Machine_GUI_TextLabel* self) {
  return Machine_Rectangle2_getColor(self->background);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_setForegroundColor(Machine_GUI_TextLabel* self, const Machine_Math_Vector3* foregroundColor) {
  Machine_Text_Layout_setColor(self->foreground, foregroundColor);
  self->childDirty = true;
}

const Machine_Math_Vector3* Machine_GUI_TextLabel_getForegroundColor(const Machine_GUI_TextLabel* self) {
  return Machine_Text_Layout_getColor(self->foreground);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_TextLabel_render(Machine_GUI_TextLabel* self, float width, float height) {
  if (self->childDirty) {
    Machine_Rectangle2_setRectangle(self->background, ((Machine_GUI_Widget*)self)->rectangle);
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
  Machine_Shape2_render((Machine_Shape2*)self->background, width, height);
  Machine_Text_Layout_render(self->foreground, width, height);
}
