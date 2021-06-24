#if !defined(MACHINE_GUI_CONTEXT_H_INCLUDED)
#define MACHINE_GUI_CONTEXT_H_INCLUDED

#include "./../GUI/GDL/Context.h"
#include "./../GUI/Signals/Context.h"
#include "_Video.h"
#include "./../GUI/Group.h"

/// @brief A GUI context.
/// @extends Machine.Object
MACHINE_DECLARE_CLASSTYPE(Machine_GUI_Context)

struct Machine_GUI_Context {
  Machine_Object parent;
  float canvasWidth;
  float canvasHeight;
  Machine_GUI_Group* rootGroup;
  Machine_GUI_Signals_Context* signalsContext;
  Machine_GUI_GDL_Context* gdlContext;
};

/// @brief Create a GUI ontext.
/// @param context The underlying GDL context.
/// @return The GUI context.
Machine_GUI_Context* Machine_GUI_Context_create(Machine_GDL_Context* context);

void Machine_GUI_Context_onCanvasSizechanged(Machine_GUI_Context* self, Machine_CanvasSizeChangedEvent* event);

/// @brief Set the root group.
/// @param self This GUI context.
/// @param rootGroup The root group or null.
void Machine_GUI_Context_setRootGroup(Machine_GUI_Context* self, Machine_GUI_Group* rootGroup);

/// @brief Get the root group.
/// @param self This GUI context.
/// @return The root group or null.
Machine_GUI_Group* Machine_GUI_Context_getRootGroup(const Machine_GUI_Context* self);

void Machine_GUI_Context_onRender(Machine_GUI_Context* self);

#endif // MACHINE_GUI_CONTEXT_H_INCLUDED
