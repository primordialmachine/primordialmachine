/// @file GUI/Border.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/Border.h"



#include "./../Shape2.h"



struct Machine_GUI_Border {
  Machine_GUI_Widget parent;
  Machine_Math_Vector3* color;
  float width;
  Machine_Math_Rectangle2* rectangle;
  Machine_GUI_Widget* child;
  /// @brief When the child changes or the rectangle changes,
  /// we need to update the childs rectangle accordingly.
  bool childDirty;
  Machine_Rectangle2* borderRectangle;
};

static void Machine_GUI_Border_visit(Machine_GUI_Border* self);

static void Machine_GUI_Border_construct(Machine_GUI_Border* self, size_t numberOfArguments, const Machine_Value* arguments);

static const Machine_Math_Rectangle2* Machine_GUI_Border_getRectangle(Machine_GUI_Border* self);

static void Machine_GUI_Border_setRectangle(Machine_GUI_Border* self, const Machine_Math_Rectangle2* rectangle);

static const Machine_Math_Vector2* Machine_GUI_Border_getPosition(Machine_GUI_Border* self);

static void Machine_GUI_Border_setPosition(Machine_GUI_Border* self, const Machine_Math_Vector2* position);

static const Machine_Math_Vector2* Machine_GUI_Border_getSize(Machine_GUI_Border* self);

static void Machine_GUI_Border_setSize(Machine_GUI_Border* self, const Machine_Math_Vector2* size);

static const Machine_Math_Vector2* Machine_GUI_Border_getPreferredSize(Machine_GUI_Border* self);

MACHINE_DEFINE_CLASSTYPE(Machine_GUI_Border)
MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_Border, Machine_GUI_Widget, &Machine_GUI_Border_visit, &Machine_GUI_Border_construct, NULL)

static void Machine_GUI_Border_visit(Machine_GUI_Border* self) {
  if (self->color) {
    Machine_visit(self->color);
  }
  if (self->child) {
    Machine_visit(self->child);
  }
  if (self->rectangle) {
    Machine_visit(self->rectangle);
  }
  if (self->borderRectangle) {
    Machine_visit(self->borderRectangle);
  }
}

static void Machine_GUI_Border_render(Machine_GUI_Border* self, Machine_GUI_Context* context, float width, float height) {
  if (self->child) {
    if (self->childDirty) {
      Machine_Math_Rectangle2* rectangle = Machine_Math_Rectangle2_clone(self->rectangle);
      Machine_Math_Rectangle2_inset(rectangle, self->width, self->width, self->width, self->width);
      Machine_GUI_Widget_setRectangle(self->child, rectangle);
      self->childDirty = false;
    }
    Machine_GUI_Widget_render(self->child, context, width, height);
    
    Machine_Rectangle2_setColor(self->borderRectangle, self->color);
    if (self->width > 0.f) {
      { // left/top
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(self->rectangle), Machine_Math_Rectangle2_getMinY(self->rectangle));

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, self->width, self->width);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // right/top
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMaxX(self->rectangle) - self->width, Machine_Math_Rectangle2_getMinY(self->rectangle));

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, self->width, self->width);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // left/bottom
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(self->rectangle), Machine_Math_Rectangle2_getMaxY(self->rectangle) - self->width);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, self->width, self->width);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // right/bottom
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMaxX(self->rectangle) - self->width, Machine_Math_Rectangle2_getMaxY(self->rectangle) - self->width);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, self->width, self->width);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // left side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(self->rectangle), Machine_Math_Rectangle2_getMinY(self->rectangle) + self->width);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, self->width, Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(self->rectangle)) - self->width * 2.f);
        
        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2 *)self->borderRectangle, width, height);
      }
      { // right side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMaxX(self->rectangle) - self->width, Machine_Math_Rectangle2_getMinY(self->rectangle) + self->width);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, self->width, Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(self->rectangle)) - self->width * 2.f);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // top side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(self->rectangle) + self->width, Machine_Math_Rectangle2_getMinY(self->rectangle));

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(self->rectangle)) - self->width * 2.f, self->width);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // bottom side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(self->rectangle) + self->width, Machine_Math_Rectangle2_getMaxY(self->rectangle) - self->width);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(self->rectangle)) - self->width * 2.f, self->width);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
    }
  }
}

static void Machine_GUI_Border_construct(Machine_GUI_Border* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_GUI_Widget_construct((Machine_GUI_Widget*)self, numberOfArguments, arguments);
  self->width = 1.f;
  self->color = Machine_Math_Vector3_create(0.1f, 0.1f, 0.1f);
  self->rectangle = Machine_Math_Rectangle2_create();
  self->borderRectangle = Machine_Rectangle2_create();
  self->childDirty = true;
  ((Machine_GUI_Widget*)self)->render = (void (*)(Machine_GUI_Widget*, Machine_GUI_Context*, float, float)) &Machine_GUI_Border_render;
  ((Machine_GUI_Widget*)self)->setRectangle = (void (*)(Machine_GUI_Widget*, const Machine_Math_Rectangle2*)) & Machine_GUI_Border_setRectangle;
  ((Machine_GUI_Widget*)self)->getRectangle = (const Machine_Math_Rectangle2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_Border_getRectangle;
  ((Machine_GUI_Widget*)self)->setPosition = (void (*)(Machine_GUI_Widget*, const Machine_Math_Vector2*)) & Machine_GUI_Border_setPosition;
  ((Machine_GUI_Widget*)self)->getPosition = (const Machine_Math_Vector2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_Border_getPosition;
  ((Machine_GUI_Widget*)self)->setSize = (void (*)(Machine_GUI_Widget*, const Machine_Math_Vector2*)) & Machine_GUI_Border_setSize;
  ((Machine_GUI_Widget*)self)->getSize = (const Machine_Math_Vector2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_Border_getSize;
  ((Machine_GUI_Widget*)self)->getPreferredSize = (const Machine_Math_Vector2 * (*)(Machine_GUI_Widget*)) & Machine_GUI_Border_getPreferredSize;
  Machine_setClassType((Machine_Object*)self, Machine_GUI_Border_getClassType());
}

Machine_GUI_Border* Machine_GUI_Border_create() {
  Machine_ClassType* ty = Machine_GUI_Border_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Machine_GUI_Border* self = (Machine_GUI_Border*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

Machine_GUI_Widget* Machine_GUI_Widget_getChild(Machine_GUI_Border* self) {
  return self->child;
}

void Machine_GUI_Border_setChild(Machine_GUI_Border* self, Machine_GUI_Widget *child) {
  if (self->child != child) {
    self->child = child;
    self->childDirty = true;
  }
}

const Machine_Math_Vector3* Machine_GUI_Border_getBorderColor(Machine_GUI_Border* self) {
  return self->color;
}

void Machine_GUI_Border_setBorderColor(Machine_GUI_Border* self, Machine_Math_Vector3* color) {
  Machine_Math_Vector3_copy(self->color, color);
}

float Machine_GUI_Border_getBorderWidth(Machine_GUI_Border* self) {
  return self->width;
}

void Machine_GUI_Border_setBorderWidth(Machine_GUI_Border* self, float width) {
  self->width = width;
}

static const Machine_Math_Rectangle2* Machine_GUI_Border_getRectangle(Machine_GUI_Border* self) {
  return self->rectangle;
}

static void Machine_GUI_Border_setRectangle(Machine_GUI_Border* self, const Machine_Math_Rectangle2* rectangle) {
  Machine_Math_Rectangle2_copy(self->rectangle, rectangle);
  self->childDirty = true;
}

static const Machine_Math_Vector2* Machine_GUI_Border_getPosition(Machine_GUI_Border* self) {
  return Machine_Math_Rectangle2_getPosition(self->rectangle);
}

static void Machine_GUI_Border_setPosition(Machine_GUI_Border* self, const Machine_Math_Vector2* position) {
  Machine_Math_Rectangle2_setPosition(self->rectangle, position);
  self->childDirty = true;
}

static const Machine_Math_Vector2* Machine_GUI_Border_getSize(Machine_GUI_Border* self) {
  return Machine_Math_Rectangle2_getSize(self->rectangle);
}

static void Machine_GUI_Border_setSize(Machine_GUI_Border* self, const Machine_Math_Vector2* size) {
  Machine_Math_Rectangle2_setSize(self->rectangle, size);
  self->childDirty = true;
}

static const Machine_Math_Vector2* Machine_GUI_Border_getPreferredSize(Machine_GUI_Border* self) {
  const Machine_Math_Vector2* preferredSize = self->child ? Machine_GUI_Widget_getPreferredSize(self->child) : Machine_Math_Vector2_create();
  Machine_Math_Vector2* borderSize = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(borderSize, self->width * 2.f, self->width * 2.f);
  return Machine_Math_Vector2_sum(preferredSize, borderSize);
}
