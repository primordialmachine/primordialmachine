/// @file GUI/TextLabel.h
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#if !defined(MACHINE_GUI_TEXTLABEL_H_INCLUDED)
#define MACHINE_GUI_TEXTLABEL_H_INCLUDED

#include "./../GUI/Widget.h"
#include "./../Fonts.h"

/// @brief A label with text.
/// @extends Machine.GUI.Widget
MACHINE_DECLARE_CLASSTYPE(Machine_GUI_TextLabel)

/// @brief Create a text label with default values.
/// @param context The GUI context the text label belongs to.
/// @return The text label.
Machine_GUI_TextLabel* Machine_GUI_TextLabel_create(Machine_GUI_Context *context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the text.
/// @param self This text label.
/// @param text The text.
void Machine_GUI_TextLabel_setText(Machine_GUI_TextLabel* self, Machine_String* text);

/// @brief Get the text.
/// @param self This text label.
/// @return The text.
Machine_String* Machine_GUI_TextLabel_getText(const Machine_GUI_TextLabel* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the background color.
/// @param self This text label.
/// @param backgroundColor The background color.
void Machine_GUI_TextLabel_setBackgroundColor(Machine_GUI_TextLabel* self, const Machine_Math_Vector4* backgroundColor);

/// @brief Get the background color.
/// @param self This text label.
/// @return The background color.
const Machine_Math_Vector4* Machine_GUI_TextLabel_getBackgroundColor(const Machine_GUI_TextLabel *self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the foreground color.
/// @param self This text label.
/// @param foregroundColor The foreground color.
void Machine_GUI_TextLabel_setForegroundColor(Machine_GUI_TextLabel* self, const Machine_Math_Vector3* foregroundColor);

/// @brief Set the foreground color.
/// @param self This text label.
/// @return The foreground color.
const Machine_Math_Vector3* Machine_GUI_TextLabel_getForegroundColor(const Machine_GUI_TextLabel* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // MACHINE_GUI_TEXTLABEL_H_INCLUDED
