/// @file GUI/Border.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/Border.h"




#include "./../GUI/Context.h"
#include "./../Shape2.h"
#include "./../GUI/BorderModel.h"




struct Machine_GUI_Border {
  Machine_GUI_Widget parent;
  Machine_GUI_BorderModel *borderModel;
  Machine_GUI_Widget* child;
  /// @brief When the child changes or the rectangle changes,
  /// we need to update the childs rectangle accordingly.
  bool childDirty;
  Machine_Rectangle2* borderRectangle;
};

static void Machine_GUI_Border_visit(Machine_GUI_Border* self);

static void Machine_GUI_Border_construct(Machine_GUI_Border* self, size_t numberOfArguments, const Machine_Value* arguments);

static const Machine_Math_Vector2* Machine_GUI_Border_getPreferredSize(Machine_GUI_Border* self);

static const Machine_Math_Rectangle2* Machine_GUI_Border_getCanvasRectangle(const Machine_GUI_Border* self);

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_Border, Machine_GUI_Widget, &Machine_GUI_Border_visit, &Machine_GUI_Border_construct, NULL)

static void Machine_GUI_Border_visit(Machine_GUI_Border* self) {
  if (self->borderModel) {
    Machine_visit(self->borderModel);
  }
  if (self->child) {
    Machine_visit(self->child);
  }
  if (self->borderRectangle) {
    Machine_visit(self->borderRectangle);
  }
}

static const Machine_Math_Rectangle2* Machine_GUI_Border_getCanvasRectangle(const Machine_GUI_Border* self) {
  Machine_Math_Rectangle2* rectangle = Machine_Math_Rectangle2_clone(Machine_GUI_Widget_getRectangle((Machine_GUI_Widget*)self));
  Machine_Math_Rectangle2_inset(rectangle, Machine_GUI_BorderModel_getLeftWidth(self->borderModel), Machine_GUI_BorderModel_getTopWidth(self->borderModel),
                                           Machine_GUI_BorderModel_getRightWidth(self->borderModel), Machine_GUI_BorderModel_getBottomWidth(self->borderModel));
  return rectangle;
}

static void Machine_GUI_Border_render(Machine_GUI_Border* self, float width, float height) {
  if (self->child) {
    if (self->childDirty) {
      const Machine_Math_Rectangle2* canvasRectangle = Machine_GUI_Widget_getCanvasRectangle((Machine_GUI_Widget *)self);
      Machine_GUI_Widget_setRectangle(self->child, canvasRectangle);
      self->childDirty = false;
    }
    Machine_GUI_Widget_render(self->child, width, height);
    
    {
      const Machine_Math_Vector4* c = Machine_GUI_BorderModel_getColor(self->borderModel);
      Machine_Rectangle2_setColor(self->borderRectangle, c);
    }
    if (Machine_GUI_BorderModel_getLeftWidth(self->borderModel) > 0.f ||
        Machine_GUI_BorderModel_getRightWidth(self->borderModel) > 0.f ||
        Machine_GUI_BorderModel_getTopWidth(self->borderModel) > 0.f ||
        Machine_GUI_BorderModel_getBottomWidth(self->borderModel) > 0.f) {
      float lw = Machine_GUI_BorderModel_getLeftWidth(self->borderModel);
      float rw = Machine_GUI_BorderModel_getRightWidth(self->borderModel);
      float tw = Machine_GUI_BorderModel_getTopWidth(self->borderModel);
      float bw = Machine_GUI_BorderModel_getBottomWidth(self->borderModel);
      const Machine_Math_Rectangle2* R = Machine_GUI_Widget_getRectangle((Machine_GUI_Widget*)self); // TODO: Should be: getAbsoluteRectangle.
      { // left/top
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(R), Machine_Math_Rectangle2_getMinY(R));

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, lw, tw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // right/top
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMaxX(R) - rw, Machine_Math_Rectangle2_getMinY(R));

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, rw, tw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // left/bottom
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(R), Machine_Math_Rectangle2_getMaxY(R) - bw);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, lw, bw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // right/bottom
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMaxX(R) - rw, Machine_Math_Rectangle2_getMaxY(R) - bw);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, rw, bw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // left side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(R), Machine_Math_Rectangle2_getMinY(R) + tw);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, lw, Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(R)) - tw - bw);
        
        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2 *)self->borderRectangle, width, height);
      }
      { // right side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMaxX(R) - rw, Machine_Math_Rectangle2_getMinY(R) + tw);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, rw, Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(R)) - tw - bw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // top side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(R) + lw, Machine_Math_Rectangle2_getMinY(R));

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(R)) - lw - rw, tw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
      { // bottom side
        Machine_Math_Rectangle2* r = Machine_Math_Rectangle2_create();

        //
        Machine_Math_Vector2* position = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(position, Machine_Math_Rectangle2_getMinX(R) + lw, Machine_Math_Rectangle2_getMaxY(R) - bw);

        //
        Machine_Math_Vector2* size = Machine_Math_Vector2_create();
        Machine_Math_Vector2_set(size, Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(R)) - lw - rw, bw);

        Machine_Math_Rectangle2_setPosition(r, position);
        Machine_Math_Rectangle2_setSize(r, size);

        Machine_Rectangle2_setRectangle(self->borderRectangle, r);
        Machine_Shape2_render((Machine_Shape2*)self->borderRectangle, width, height);
      }
    }
  }
}

static Machine_Value boundsChangedCallback(size_t numberOfArguments, const Machine_Value* arguments) {
  MACHINE_ASSERT(numberOfArguments == 1, Machine_Status_InvalidNumberOfArguments);
  Machine_GUI_Border *self = (Machine_GUI_Border *)Machine_Value_getObject(&arguments[0]);
  self->childDirty = true;
  Machine_Value result;
  Machine_Value_setVoid(&result, Machine_Void_Void);
  return result;
}

static void Machine_GUI_Border_construct(Machine_GUI_Border* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_GUI_Widget_construct((Machine_GUI_Widget*)self, numberOfArguments, arguments);
  self->borderModel = Machine_GUI_BorderModel_create();
  Machine_GUI_BorderModel_setWidth(self->borderModel, 1.f);
  self->borderRectangle = Machine_Rectangle2_create();
  self->childDirty = true;
  ((Machine_GUI_Widget*)self)->render = (void (*)(Machine_GUI_Widget*, float, float)) &Machine_GUI_Border_render;
  ((Machine_GUI_Widget*)self)->getPreferredSize = (const Machine_Math_Vector2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Border_getPreferredSize;
  ((Machine_GUI_Widget*)self)->getCanvasRectangle = (const Machine_Math_Rectangle2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Border_getCanvasRectangle;

  Machine_GUI_Widget_subscribe((Machine_GUI_Widget*)self, ((Machine_GUI_Widget*)self)->context->signalsContext->PositionChanged, (Machine_Object*)self, &boundsChangedCallback);
  Machine_GUI_Widget_subscribe((Machine_GUI_Widget*)self, ((Machine_GUI_Widget*)self)->context->signalsContext->SizeChanged, (Machine_Object*)self, &boundsChangedCallback);

  Machine_setClassType((Machine_Object*)self, Machine_GUI_Border_getClassType());
}

Machine_GUI_Border* Machine_GUI_Border_create(Machine_GUI_Context* context) {
  Machine_ClassType* ty = Machine_GUI_Border_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 1;
  Machine_Value ARGUMENTS[1];
  Machine_Value_setObject(&ARGUMENTS[0], (Machine_Object *)context);
  Machine_GUI_Border* self = (Machine_GUI_Border*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

Machine_GUI_Widget* Machine_GUI_Widget_getChild(Machine_GUI_Border* self) {
  return self->child;
}

void Machine_GUI_Border_setChild(Machine_GUI_Border* self, Machine_GUI_Widget *child) {
  if (self->child == child) {
    return;
  }
  if (self->child != NULL) {
    self->child->parent = NULL;
  }
  self->child = child;
  if (self->child) {
    child->parent = (Machine_GUI_Widget *)self;
  }
  self->childDirty = true;
}

const Machine_Math_Vector4* Machine_GUI_Border_getBorderColor(const Machine_GUI_Border* self) {
  return Machine_GUI_BorderModel_getColor(self->borderModel);
}

void Machine_GUI_Border_setBorderColor(Machine_GUI_Border* self, const Machine_Math_Vector4* color) {
  Machine_GUI_BorderModel_setColor(self->borderModel, color);
}

float Machine_GUI_Border_getBorderLeftWidth(const Machine_GUI_Border* self) {
  return Machine_GUI_BorderModel_getLeftWidth(self->borderModel);
}

float Machine_GUI_Border_getBorderRightWidth(const Machine_GUI_Border* self) {
  return Machine_GUI_BorderModel_getRightWidth(self->borderModel);
}

float Machine_GUI_Border_getBorderTopWidth(const Machine_GUI_Border* self) {
  return Machine_GUI_BorderModel_getTopWidth(self->borderModel);
}

float Machine_GUI_Border_getBorderBottomWidth(const Machine_GUI_Border* self) {
  return Machine_GUI_BorderModel_getBottomWidth(self->borderModel);
}

void Machine_GUI_Border_setBorderWidth(Machine_GUI_Border* self, float width) {
  Machine_GUI_BorderModel_setWidth(self->borderModel, width);
}

void Machine_GUI_Border_setLeftBorderWidth(Machine_GUI_Border* self, float width) {
  Machine_GUI_BorderModel_setLeftWidth(self->borderModel, width);
}

void Machine_GUI_Border_setRightBorderWidth(Machine_GUI_Border* self, float width) {
  Machine_GUI_BorderModel_setRightWidth(self->borderModel, width);
}

void Machine_GUI_Border_setTopBorderWidth(Machine_GUI_Border* self, float width) {
  Machine_GUI_BorderModel_setTopWidth(self->borderModel, width);
}

void Machine_GUI_Border_setBottomBorderWidth(Machine_GUI_Border* self, float width) {
  Machine_GUI_BorderModel_setBottomWidth(self->borderModel, width);
}

static const Machine_Math_Vector2* Machine_GUI_Border_getPreferredSize(Machine_GUI_Border* self) {
  const Machine_Math_Vector2* preferredSize = self->child ? Machine_GUI_Widget_getPreferredSize(self->child) : Machine_Math_Vector2_create();
  Machine_Math_Vector2* borderSize = Machine_Math_Vector2_create();
  float w = Machine_GUI_BorderModel_getLeftWidth(self->borderModel)
          + Machine_GUI_BorderModel_getRightWidth(self->borderModel);
  float h = Machine_GUI_BorderModel_getTopWidth(self->borderModel)
          + Machine_GUI_BorderModel_getBottomWidth(self->borderModel);
  Machine_Math_Vector2_set(borderSize, w, h);
  return Machine_Math_Vector2_sum(preferredSize, borderSize);
}
