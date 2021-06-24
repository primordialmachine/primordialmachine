#include "./../GUI/Group.h"



#include "./../GUI/Context.h"



static void Machine_GUI_Group_visit(Machine_GUI_Group* self) {
  if (self->layoutModel) {
    Machine_visit(self->layoutModel);
  }
  if (self->children) {
    Machine_visit(self->children);
  }
}

static const Machine_Math_Vector2* Machine_GUI_Group_getPreferredSize(const Machine_GUI_Group* self) {
  return Machine_Math_Rectangle2_getSize(Machine_GUI_Widget_getRectangle((Machine_GUI_Widget *)self));
}

static void Machine_GUI_Group_render(Machine_GUI_Group* self, float width, float height) {
  const Machine_Math_Vector2* size = Machine_GUI_Widget_getSize((Machine_GUI_Widget *)self);
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self->children); i < n; ++i) {
    Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(self->children, i);
    Machine_GUI_Widget_render(widget, Machine_Math_Vector2_getX(size), Machine_Math_Vector2_getY(size));
  }
}

void Machine_GUI_Group_construct(Machine_GUI_Group* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_GUI_Widget_construct((Machine_GUI_Widget*)self, numberOfArguments, arguments);
  self->layoutModel = Machine_GUI_LayoutModel_create();
  self->children = Machine_GUI_WidgetList_create();
  ((Machine_GUI_Widget*)self)->render = (void (*)(Machine_GUI_Widget*, float, float)) & Machine_GUI_Group_render;
  ((Machine_GUI_Widget*)self)->getPreferredSize = (const Machine_Math_Vector2 * (*)(const Machine_GUI_Widget*)) & Machine_GUI_Group_getPreferredSize;
  Machine_setClassType((Machine_Object*)self, Machine_GUI_Group_getClassType());
}

MACHINE_DEFINE_CLASSTYPE_EX(Machine_GUI_Group, Machine_GUI_Widget, &Machine_GUI_Group_visit, &Machine_GUI_Group_construct, NULL)

Machine_GUI_Group* Machine_GUI_Group_create(Machine_GUI_Context* context) {
  Machine_ClassType* ty = Machine_GUI_Group_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 1;
  Machine_Value ARGUMENTS[1];
  Machine_Value_setObject(&ARGUMENTS[0], (Machine_Object*)context);
  Machine_GUI_Group* self = (Machine_GUI_Group*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

const Machine_GUI_LayoutModel* Machine_GUI_Group_getLayoutModel(const Machine_GUI_Group* self) {
  return self->layoutModel;
}

void Machine_GUI_Group_setLayoutModel(Machine_GUI_Group* self, Machine_GUI_LayoutModel* layoutModel) {
  self->layoutModel = layoutModel;
}

void Machine_GUI_Group_relayout(Machine_GUI_Group* self, float canvasWidth, float canvasHeight) {
  // Set the size of all children to the same value: That value is the component-wise maxima vector of the preferred sizes of all children.
  Machine_Math_Vector2* preferredSize = Machine_Math_Vector2_clone(Machine_GUI_Widget_getPreferredSize(Machine_GUI_WidgetList_getAt(self->children, 0)));
  for (size_t i = 1, n = Machine_GUI_WidgetList_getSize(self->children); i < n; ++i) {
    Machine_Math_Vector2_maxima(preferredSize, preferredSize, Machine_GUI_Widget_getPreferredSize(Machine_GUI_WidgetList_getAt(self->children, i)));
  }
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self->children); i < n; ++i) {
    Machine_GUI_Widget_setSize(Machine_GUI_WidgetList_getAt(self->children, i), preferredSize);
  }
  // Layout the children.
  const Machine_Math_Vector2* size = Machine_GUI_Widget_getSize((Machine_GUI_Widget*)self);
  Machine_GUI_WidgetList_layout(self->children, Machine_Math_Vector2_getX(size), Machine_Math_Vector2_getY(size), canvasWidth, canvasHeight, self->layoutModel);
}
