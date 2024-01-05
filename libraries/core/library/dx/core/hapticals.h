/// @file dx/core/hapticals.h
/// @brief Mouse buttons, mouse pointers, mouse wheels, keyboard keys and messages related to the actions associated with them.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_HAPTICALS_H_INCLUDED)
#define DX_CORE_HAPTICALS_H_INCLUDED

#include "dx/core/msgs.h"
#include "Core/KeyboardKey.h"
#include "Core/ModifierKeys.h"
#include "Core/MouseButton.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Hapticals
/// @brief Return a human-readable, static constant C string describing a keyboard key.
/// @param SELF The keyboard key.
/// @return A pointer to a C string as described above on success. The null pointer on failure.
char const* dx_keyboard_key_to_string(Core_KeyboardKey SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Hapticals
/// @brief Return a human-readable, static constant C string describing a mouse button.
/// @param SELF The mouse button.
/// @return A pointer to a C string as described above on success. The null pointer on failure.
char const* dx_mouse_button_to_string(Core_MouseButton SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.input_msg_kind",
                            dx_input_msg_kind);

enum dx_input_msg_kind {
  DX_INPUT_MSG_KIND_KEYBOARD_KEY = 1,
  DX_INPUT_MSG_KIND_MOUSE_BUTTON,
  DX_INPUT_MSG_KIND_MOUSE_POINTER,
};

DX_DECLARE_OBJECT_TYPE("dx.input_msg",
                       dx_input_msg,
                       dx_msg);

static inline dx_input_msg* DX_INPUT_MSG(void* p) {
  return (dx_input_msg*)p;
}

/// @brief Construct this "input" message.
/// @param SELF A pointer to this "input" message object.
/// @param type The type of this input message.
/// Must be one of #DX_INPUT_MSG_KIND_KEYBOARD_KEY, #DX_INPUT_MSG_KIND_MOUSE_BUTTON, or #DX_INPUT_MSG_KIND_MOUSE_POINTER.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @constructor{dx_input_msg}
Core_Result dx_input_msg_construct(dx_input_msg* SELF, dx_input_msg_kind kind, Core_ModifierKeys modifierKeys);

/// @brief Get the kind of this "input" message.
/// @param SELF A poiner to this "input" message object.
/// @return The kind of this "input" message.
/// @method{dx_input_msg}
dx_input_msg_kind dx_input_msg_get_kind(dx_input_msg* SELF);

/// @brief Get the modifier keys of this "input" message.
/// @param RETURN A pointer to a <code>Core_ModifierKeys</code> variable.
/// @param SELF A pointer to this "input" message object.
/// @success <code>*RETURN</code> was assigned the modifier keys of this "input" message.
/// @method{dx_input_msg}
Core_Result dx_input_msg_get_modifier_keys(Core_ModifierKeys* RETURN, dx_input_msg* SELF);

struct dx_input_msg {
  Core_Message _parent;
  dx_input_msg_kind kind;
  Core_ModifierKeys modifierKeys;
};

struct dx_input_msg_dispatch {
  Core_Message_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("Core.KeyboardKeyAction",
                            Core_KeyboardKeyAction);

enum Core_KeyboardKeyAction {
  Core_KeyboardKeyAction_Pressed = 1,
  Core_KeyboardKeyAction_Released = 2,
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The opaque type of a "keyboard key" message.
DX_DECLARE_OBJECT_TYPE("dx.keyboard_key_msg",
                       dx_keyboard_key_msg,
                       dx_input_msg);

static inline dx_keyboard_key_msg* DX_KEYBOARD_KEY_MSG(void* p) {
  return (dx_keyboard_key_msg*)p;
}

/// @brief Construct this "keyboard key" message.
/// @param action The action of the key. An element from the Core_KeyboardKeyAction elements.
/// @param key The key.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @constructor{dx_keyboard_key_msg}
Core_Result dx_keyboard_key_msg_construct(dx_keyboard_key_msg* SELF, Core_KeyboardKeyAction action, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

/// @brief Get the action.
/// @param RETURN A pointer to a <code>Core_KeyboardKeyAction</code> variable.
/// @success <code>*RETURN</code> was assigned the action.
/// @method{dx_keyboard_key_msg}
Core_Result dx_keyboard_key_msg_get_action(Core_KeyboardKeyAction* RETURN, dx_keyboard_key_msg* SELF);

/// @brief Get the key.
/// @param RETURN A pointer to a <code>Core_KeyboardKey</code> variable.
/// @success <code>*RETURN</code> was assigned the keyboard key.
/// @method{dx_keyboard_key_msg}
Core_Result dx_keyboard_key_msg_get_key(Core_KeyboardKey* RETURN, dx_keyboard_key_msg* SELF);

/// @brief Create a "keyboard key" message.
/// @param RETURN A pointer to a code <code>dx_keyboard_key_msg*</code> variable.
/// @param action The action of the key. An element from the Core_KeyboardKeyAction elements.
/// @param key The key.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @success <code>*RETURN</code> was assigned a pointer to the "keyboard key" message object.
/// @create-operator{dx_keyboard_key_msg}
Core_Result dx_keyboard_key_msg_create(dx_keyboard_key_msg** RETURN, Core_KeyboardKeyAction action, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

struct dx_keyboard_key_msg {
  dx_input_msg _parent;
  Core_KeyboardKeyAction action;
  Core_KeyboardKey key;
};

struct dx_keyboard_key_msg_dispatch {
  dx_input_msg_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.mouse_button_action",
                            dx_mouse_button_action);

enum dx_mouse_button_action {
  DX_MOUSE_BUTTON_ACTION_PRESSED = 1,
  DX_MOUSE_BUTTON_ACTION_RELEASED = 2,
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The opaque type of a "mouse button" message.
DX_DECLARE_OBJECT_TYPE("dx.mouse_button_msg",
                       dx_mouse_button_msg,
                       dx_input_msg);

static inline dx_mouse_button_msg* DX_MOUSE_BUTTON_MSG(void* p) {
  return (dx_mouse_button_msg*)p;
}

/// @brief Construct this "mouse button" message.
/// @param action The action of the button. Must be #DX_MOUSE_BUTTON_ACTION_PRESSED or #DX_MOUSE_BUTTON_ACTION_RELEASED.
/// @param button The button.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @param x, y
/// The position of the mouse pointer.
/// The origin is at the left/top of the canvas.
/// The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @constructor{dx_mouse_button_msg}
Core_Result dx_mouse_button_msg_construct(dx_mouse_button_msg* SELF, dx_mouse_button_action action, Core_MouseButton button, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y);

dx_mouse_button_action dx_mouse_button_msg_get_action(dx_mouse_button_msg* SELF);

Core_MouseButton dx_mouse_button_msg_get_button(dx_mouse_button_msg* SELF);

/// @brief Create a "mouse button" message.
/// @param RETURN A pointer to a code <code>dx_mouse_button_msg*</code> variable.
/// @param action The action of the button. Must be #DX_MOUSE_BUTTON_ACTION_PRESSED or #DX_MOUSE_BUTTON_ACTION_RELEASED.
/// @param button The button.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @param x, y
/// The position of the mouse pointer.
/// The origin is at the left/top of the canvas.
/// The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @success <code>*RETURN</code> was assigned a pointer to the "mouse button" message object.
/// @create-operator{dx_mouse_button_msg}
Core_Result dx_mouse_button_msg_create(dx_mouse_button_msg** RETURN, dx_mouse_button_action action, Core_MouseButton button, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y);

struct dx_mouse_button_msg {
  dx_input_msg _parent;
  uint8_t action;
  Core_MouseButton button;
  Core_Real32 x, y;
};

struct dx_mouse_button_msg_dispatch {
  dx_input_msg_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.mouse_pointer_action",
                            dx_mouse_pointer_action);

enum dx_mouse_pointer_action {
  DX_MOUSE_POINTER_ACTION_MOVED = 1,
  DX_MOUSE_POINTER_ACTION_ENTERED = 2,
  DX_MOUSE_POINTER_ACTION_EXITED = 3,
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The opaque type of a "mouse pointer" message.
DX_DECLARE_OBJECT_TYPE("dx.mouse_pointer_msg",
                       dx_mouse_pointer_msg,
                       dx_input_msg);

static inline dx_mouse_pointer_msg* DX_MOUSE_POINTER_MSG(void* p) {
  return (dx_mouse_pointer_msg*)p;
}

/// @brief Construct this "mouse pointer" message.
/// @param action The action of the mouse pointer. Must be #DX_MOUSE_POINTER_ACTION_MOVED, DX_MOUSE_POINTER_ACTION_ENTERED, or DX_MOUSE_POINTER_ACTION_EXITED.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @param x, y
/// The position of the mouse pointer.
/// The origin is at the left/top of the canvas. 
/// The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @constructor{dx_mouse_pointer_msg}
Core_Result dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* SELF, dx_mouse_pointer_action action, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y);

/// @brief Get the action.
/// @param RETURN A pointer to a <code>dx_mouse_pointer_action</code> variable.
/// @success <code>*RETURN</code> was assigned the action.
/// @method{dx_mouse_pointer_action}
Core_Result dx_mouse_pointer_msg_get_action(dx_mouse_pointer_action* RETURN, dx_mouse_pointer_msg* SELF);

/// @brief Create a "mouse pointer" message.
/// @param RETURN A pointer to a <code>dx_mouse_pointer_msg*</code> variable.
/// @param action The action of the mouse pointer. Must be #DX_MOUSE_POINTER_ACTION_MOVED, DX_MOUSE_POINTER_ACTION_ENTERED, or DX_MOUSE_POINTER_ACTION_EXITED.
/// @param modifierKeys The modifier keys. A bitwise or of Core_ModifierKeys enumeration elements.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @success <code>*RETURN</code> was assigned a pointer to the "mouse pointer" message object.
/// @create-operator{dx_mouse_pointer_msg}
Core_Result dx_mouse_pointer_msg_create(dx_mouse_pointer_msg** RETURN, dx_mouse_pointer_action action, Core_ModifierKeys modifierKeys, Core_Real32 x, Core_Real32 y);

struct dx_mouse_pointer_msg {
  dx_input_msg _parent;
  uint8_t action;
  Core_Real32 x, y;
};

struct dx_mouse_pointer_msg_dispatch {
  dx_input_msg_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_HAPTICALS_H_INCLUDED
