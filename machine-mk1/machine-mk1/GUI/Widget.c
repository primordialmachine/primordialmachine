/// @file GUI/Widget.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#include "./../GUI/Widget.h"
#include "./../GUI/Context.h"

static void Machine_GUI_Widget_visit(Machine_GUI_Widget* self);

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_Widget, Machine_Object, &Machine_GUI_Widget_visit, &Machine_GUI_Widget_construct, NULL)

static void Machine_GUI_Widget_visit(Machine_GUI_Widget *self) {
  if (self->parent) {
    Machine_visit(self->parent);
  }
  if (self->rectangle) {
    Machine_visit(self->rectangle);
  }
  if (self->connections) {
    Machine_visit(self->connections);
  }
}

static void Machine_GUI_Widget_setRectangleImpl(Machine_GUI_Widget* self, const Machine_Math_Rectangle2* rectangle) {
  Machine_Math_Rectangle2_copy(self->rectangle, rectangle);
  Machine_GUI_Widget_emitPositionChangedSignal(self);
  Machine_GUI_Widget_emitSizeChangedSignal(self);
}

static void Machine_GUI_Widget_setPositionImpl(Machine_GUI_Widget* self, const Machine_Math_Vector2* position) {
  Machine_Math_Rectangle2_setPosition(self->rectangle, position);
  Machine_GUI_Widget_emitPositionChangedSignal(self);
}

static void Machine_GUI_Widget_setSizeImpl(Machine_GUI_Widget* self, const Machine_Math_Vector2* size) {
  Machine_Math_Rectangle2_setSize(self->rectangle, size);
  Machine_GUI_Widget_emitSizeChangedSignal(self);
}

static const Machine_Math_Rectangle2* Machine_GUI_Widget_getRectangleImpl(const Machine_GUI_Widget* self) {
  return self->rectangle;
}

static const Machine_Math_Rectangle2* Machine_GUI_Widget_getCanvasRectangleImpl(const Machine_GUI_Widget* self) {
  return Machine_GUI_Widget_getRectangle(self);
}

static const Machine_Math_Vector2* Machine_GUI_Widget_getPositionImpl(const Machine_GUI_Widget* self) {
  return Machine_Math_Rectangle2_getPosition(Machine_GUI_Widget_getRectangle(self));
}

static const Machine_Math_Vector2* Machine_GUI_Widget_getSizeImpl(const Machine_GUI_Widget* self) {
  return Machine_Math_Rectangle2_getSize(Machine_GUI_Widget_getRectangle(self));
}

static const Machine_Math_Vector2* Machine_GUI_Widget_getAbsolutePositionImpl(const Machine_GUI_Widget* self) {
  const Machine_Math_Vector2* position = Machine_GUI_Widget_getPosition(self);
  if (self->parent) {
    Machine_Math_Vector2* position_ = Machine_Math_Vector2_clone(position);
    Machine_Math_Vector2_add(position_, position_, Machine_GUI_Widget_getAbsolutePosition(self->parent));
    return position_;
  } else {
    return position;
  }
}

static const Machine_Math_Rectangle2* Machine_GUI_Widget_getAbsoluteRectangleImpl(const Machine_GUI_Widget* self) {
  const Machine_Math_Rectangle2* rectangle = Machine_GUI_Widget_getRectangle(self);
  if (self->parent) {
    Machine_Math_Rectangle2* rectangle_ = Machine_Math_Rectangle2_clone(rectangle);
    Machine_Math_Rectangle2_translate(rectangle_, Machine_GUI_Widget_getAbsolutePosition(self->parent));
    return rectangle_;
  } else {
    return rectangle;
  }
}

static const Machine_Math_Rectangle2* Machine_GUI_Widget_getAbsoluteCanvasRectangleImpl(const Machine_GUI_Widget* self) {
  const Machine_Math_Rectangle2* rectangle = Machine_GUI_Widget_getCanvasRectangle(self);
  if (self->parent) {
    Machine_Math_Rectangle2* rectangle_ = Machine_Math_Rectangle2_clone(rectangle);
    Machine_Math_Rectangle2_translate(rectangle_, Machine_Math_Rectangle2_getPosition(Machine_GUI_Widget_getAbsoluteCanvasRectangle(self->parent)));
    return rectangle_;
  }
  else {
    return rectangle;
  }
}

void Machine_GUI_Widget_construct(Machine_GUI_Widget* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);
  MACHINE_ASSERT(numberOfArguments == 1, Machine_Status_InvalidNumberOfArguments);
  self->context = (Machine_GUI_Context *)Machine_Value_getObject(&arguments[0]);
  self->rectangle = Machine_Math_Rectangle2_create();
  self->parent = NULL;
  self->getRectangle = (const Machine_Math_Rectangle2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getRectangleImpl;
  self->getPosition = (const Machine_Math_Vector2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getPositionImpl;
  self->getSize = (const Machine_Math_Vector2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getSizeImpl;
  self->getCanvasRectangle = (const Machine_Math_Rectangle2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getCanvasRectangleImpl;
  self->getAbsolutePosition = (const Machine_Math_Vector2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getAbsolutePositionImpl;
  self->getAbsoluteRectangle = (const Machine_Math_Rectangle2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getAbsoluteRectangleImpl;
  self->getAbsoluteCanvasRectangle = (const Machine_Math_Rectangle2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Widget_getAbsoluteCanvasRectangleImpl;
  self->setRectangle = &Machine_GUI_Widget_setRectangleImpl;
  self->setPosition = &Machine_GUI_Widget_setPositionImpl;
  self->setSize = &Machine_GUI_Widget_setSizeImpl;
  Machine_setClassType((Machine_Object*)self, Machine_GUI_Widget_getClassType());
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_render(Machine_GUI_Widget* self, float width, float height) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->render);
  self->render(self, width, height);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_setPosition(Machine_GUI_Widget* self, const Machine_Math_Vector2* position) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->setPosition);
  MACHINE_ASSERT_NOTNULL(position);
  self->setPosition(self, position);
}

const Machine_Math_Vector2* Machine_GUI_Widget_getPosition(const Machine_GUI_Widget* self) {
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

const Machine_Math_Vector2* Machine_GUI_Widget_getSize(const Machine_GUI_Widget* self) {
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

const Machine_Math_Rectangle2* Machine_GUI_Widget_getRectangle(const Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getRectangle);
  return self->getRectangle(self);
}

const Machine_Math_Rectangle2* Machine_GUI_Widget_getCanvasRectangle(const Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getCanvasRectangle);
  return self->getCanvasRectangle(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

const Machine_Math_Vector2* Machine_GUI_Widget_getPreferredSize(const Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getPreferredSize);
  return self->getPreferredSize(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

const Machine_Math_Vector2* Machine_GUI_Widget_getAbsolutePosition(const Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getAbsolutePosition);
  return self->getAbsolutePosition(self);
}

const Machine_Math_Rectangle2* Machine_GUI_Widget_getAbsoluteRectangle(const Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getAbsoluteRectangle);
  return self->getAbsoluteRectangle(self);
}

const Machine_Math_Rectangle2* Machine_GUI_Widget_getAbsoluteCanvasRectangle(const Machine_GUI_Widget* self) {
  MACHINE_ASSERT_NOTNULL(self);
  MACHINE_ASSERT_NOTNULL(self->getAbsoluteCanvasRectangle);
  return self->getAbsoluteCanvasRectangle(self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_subscribe(Machine_GUI_Widget* self, Machine_String* name, Machine_Object* context, Machine_ForeignProcedure* callback) {
  Machine_Signals_Connection *connection = Machine_Signals_Connection_create(name, context, callback);
  if (self->connections == NULL) {
    self->connections = Machine_List_create();
  }
  Machine_Value value;
  Machine_Value_setObject(&value, (Machine_Object*)connection);
  Machine_List_append(self->connections, value);
}

void Machine_GUI_Widget_unsubscribe(Machine_GUI_Widget* self, Machine_String* name, Machine_Object* context, Machine_ForeignProcedure* callback) {
  if (self->connections) {
    for (size_t i = 0, n = Machine_Collection_getSize((Machine_Collection*)self->connections); i < n; ++i) {
      Machine_Value temporary = Machine_List_getAt(self->connections, i);
      Machine_Signals_Connection* c = (Machine_Signals_Connection*)Machine_Value_getObject(&temporary);
      if (Machine_String_isEqualTo(c->name, name) &&
          Machine_Object_isEqualTo(c->context, context) &&
          Machine_ForeignProcedure_isEqualTo(c->callback, callback)) {
        Machine_List_removeAtFast(self->connections, i);
        break;
      }
    }
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_GUI_Widget_emitSignal(Machine_GUI_Widget* self, Machine_String* name, size_t numberOfArguments, const Machine_Value *arguments) {
  if (self->connections) {
    for (size_t i = 0, n = Machine_Collection_getSize((Machine_Collection*)self->connections); i < n; ++i) {
      Machine_Value temporary = Machine_List_getAt(self->connections, i);
      Machine_Signals_Connection* c = (Machine_Signals_Connection*)Machine_Value_getObject(&temporary);
      if (Machine_String_isEqualTo(c->name, name)) {
        MACHINE_ASSERT_NOTNULL(c->callback);
        c->callback(numberOfArguments, arguments);
      }
    }
  }
}

void Machine_GUI_Widget_emitPositionChangedSignal(Machine_GUI_Widget* self) {
  Machine_GUI_Signals_Context* signalsContext = self->context->signalsContext;
  size_t numberOfArguments = 1;
  Machine_Value arguments[1];
  Machine_Value_setObject(&arguments[0], (Machine_Object *)self);
  Machine_GUI_Widget_emitSignal(self, signalsContext->PositionChanged, numberOfArguments, arguments);
}

void Machine_GUI_Widget_emitSizeChangedSignal(Machine_GUI_Widget* self) {
  Machine_GUI_Signals_Context* signalsContext = self->context->signalsContext;
  size_t numberOfArguments = 1;
  Machine_Value arguments[1];
  Machine_Value_setObject(&arguments[0], (Machine_Object*)self);
  Machine_GUI_Widget_emitSignal(self, signalsContext->SizeChanged, numberOfArguments, arguments);
}

