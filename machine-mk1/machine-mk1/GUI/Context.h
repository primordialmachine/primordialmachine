/// @file GUI/Context.h
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#if !defined(MACHINE_GUI_CONTEXT_H_INCLUDED)
#define MACHINE_GUI_CONTEXT_H_INCLUDED

#include "./../GUI/Widget.h"
typedef struct Machine_GUI_WidgetList Machine_GUI_WidgetList;
typedef struct Machine_GUI_Widget Machine_GUI_Widget;

/// @brief A GUI context.
/// @extends Machine.Object
MACHINE_DECLARE_CLASSTYPE(Machine_GUI_Context)

struct Machine_GUI_Context {
  Machine_Object parent;
  /// @brief The width of the canvas.
  float canvasWidth;
  /// @brief The height of the canvas.
  float canvasHeight;
  /// @brief List of widgets.
  Machine_GUI_WidgetList* widgets;
};

/// @brief Create a GUI context.
/// @return The GUI context.
Machine_GUI_Context* Machine_GUI_Context_create();

void Machine_GUI_Context_appendChild(Machine_GUI_Context* self, Machine_GUI_Widget* widget);

Machine_GUI_Widget* Machine_GUI_Context_getChildAt(const Machine_GUI_Context* self, size_t index);

size_t Machine_GUI_Context_getNumberOfChildren(const Machine_GUI_Context* self);

void Machine_GUI_Context_render(Machine_GUI_Context* self, float width, float height);

void Machine_GUI_Context_setCanvasSize(Machine_GUI_Context* self, float width, float height);

/// @brief Get the canvas width.
/// @param self This context.
/// @return The canvas width.
/// @default The default is @a 0.
float Machine_GUI_Context_getCanvasWidth(const Machine_GUI_Context* self);

/// @brief Get the canvas height.
/// @param self This context.
/// @return The canvas height.
/// @default The default is @a 0.
float Machine_GUI_Context_getCanvasHeight(const Machine_GUI_Context* self);

/// @brief Notify the context that a mouse button was pressed.
/// @param self This GUI context.
/// @param button The index of the button.
/// @param x The position of the mouse pointer along the x-axis.
/// @param y The position of the mouse pointer along the y-axis.
void Machine_GUI_Context_onMouseButtonPressed(Machine_GUI_Context* self, int button, float x, float y);

/// @brief Notify the context that a mouse button was pressed.
/// @param self This GUI context.
/// @param button The index of the button.
/// @param x The position of the mouse pointer along the x-axis.
/// @param y The position of the mouse pointer along the y-axis.
void Machine_GUI_Context_onMouseButtonRelease(Machine_GUI_Context* self, int button, float x, float y);

/// @brief Notify the context that a mouse pointer was moved.
/// @param self This GUI context.
/// @param x The position of the mouse pointer along the x-axis.
/// @param y The position of the mouse pointer along the y-axis.
void Machine_GUI_Context_onMousePointerMoved(Machine_GUI_Context* self, float x, float y);

#endif // MACHINE_GUI_CONTEXT_H_INCLUDED
