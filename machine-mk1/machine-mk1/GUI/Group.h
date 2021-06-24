#if !defined(MACHINE_GUI_GROUP_H_INCLUDED)
#define MACHINE_GUI_GROUP_H_INCLUDED

#include "./../GUI/Widget.h"
#include "./../GUI/WidgetList.h"

/// @brief A group.
/// @extends Machine.GUI.Widget
MACHINE_DECLARE_CLASSTYPE(Machine_GUI_Group)

struct Machine_GUI_Group {
  Machine_GUI_Widget __parent;
  Machine_GUI_LayoutModel* layoutModel;
  Machine_GUI_WidgetList* children;
};

/// @brief Create a group with default values.
/// @param context The context the group belongs to.
/// @return The group.
Machine_GUI_Group* Machine_GUI_Group_create(Machine_GUI_Context* context);

const Machine_GUI_LayoutModel* Machine_GUI_Group_getLayoutModel(const Machine_GUI_Group* self);

void Machine_GUI_Group_setLayoutModel(Machine_GUI_Group* self, Machine_GUI_LayoutModel* layoutModel);

void Machine_GUI_Group_relayout(Machine_GUI_Group* self, float canvasWidth, float canvasHeight);

#endif // MACHINE_GUI_GROUP_H_INCLUDED