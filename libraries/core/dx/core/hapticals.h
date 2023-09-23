/// @file dx/core/hapticals.h
/// @brief Mouse buttons, mouse pointers, mouse wheels, keyboard keys and messages related to the actions associated with them.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_HAPTICALS_H_INCLUDED)
#define DX_CORE_HAPTICALS_H_INCLUDED

#include "dx/core/msgs.h"

/// @ingroup Core_Hapticals
/// @brief The "lmenu" modifier.
#define DX_MODIFIER_LMENU (1<<0)
/// @ingroup Core_Hapticals
/// @brief The "rmenu" modifier.
#define DX_MODIFIER_RMENU (1<<1)

/// @ingroup Core_Hapticals
/// @brief The "lshift" modifier.
#define DX_MODIFIER_LSHIFT (1<<2)
/// @ingroup Core_Hapticals
/// @brief The "rshift" modifier.
#define DX_MODIFIER_RSHIFT (1<<3)

/// @ingroup Core_Hapticals
/// @brief The "lctrl" modifier.
#define DX_MODIFIER_LCTRL (1<<4)
/// @ingroup Core_Hapticals
/// @brief The "rctrl" modifier.
#define DX_MODIFIER_RCTRL (1<<5)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Hapticals
/// @brief Enumeration of keyboard keys.
DX_DECLARE_ENUMERATION_TYPE("dx.keyboard_key",
                            dx_keyboard_key);

enum dx_keyboard_key {
  #define ALIAS(aliased,alias) dx_keyboard_key_##alias = dx_keyboard_key_##aliased,
  #define DEFINE(name,value,description) dx_keyboard_key_##name = value,
  #include "dx/core/keyboard_keys.i"
  #undef DEFINE
  #undef ALIAS
};

/// @ingroup Core_Hapticals
/// @brief Return a human-readable, static constant C string describing a keyboard key.
/// @param self The keyboard key.
/// @return A pointer to a C string as described above on success. The null pointer on failure.
char const* dx_keyboard_key_to_string(dx_keyboard_key self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Hapticals
/// @brief Enumeration of mouse buttons.
DX_DECLARE_ENUMERATION_TYPE("dx.mouse_button",
                            dx_mouse_button);

enum dx_mouse_button {
  #define ALIAS(aliased,alias) dx_mouse_button_##alias = dx_mouse_button_##aliased,
  #define DEFINE(name,value,description) dx_mouse_button_##name = value,
  #include "dx/core/mouse_buttons.i"
  #undef DEFINE
  #undef ALIAS
};

/// @ingroup Core_Hapticals
/// @brief Return a human-readable, static constant C string describing a mouse button.
/// @param self The mouse button.
/// @return A pointer to a C string as described above on success. The null pointer on failure.
char const* dx_mouse_button_to_string(dx_mouse_button self);

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
/// @param modifiers The modifiers. A bitwise or of
/// - dx_modifier_lmenu
/// - dx_modifier_rmenu
/// - dx_modifier_lshift
/// - dx_modifier_rshift
/// - dx_modifier_lctrl
/// - dx_modifier_lctrl
/// @method-call
dx_result dx_input_msg_construct(dx_input_msg* SELF, dx_input_msg_kind kind, uint8_t modifiers);

/// @brief Get the kind of this "input" message.
/// @param SELF A poiner to this "input" message object.
/// @return The kind of this "input" message.
dx_input_msg_kind dx_input_msg_get_kind(dx_input_msg* SELF);

/// @brief Get the modifiers of this "input" message.
/// @param RETURN A pointer to a <code>uint8_t</code> variable.
/// @param SELF A pointer to this "input" message object.
/// @success <code>*RETURN</code> was assigned the modifiers of this "input" message..
/// @default-return
/// @default-failure
dx_result dx_input_msg_get_modifiers(uint8_t* RETURN, dx_input_msg* SELF);

struct dx_input_msg {
  dx_msg _parent;
  dx_input_msg_kind kind;
  uint8_t modifiers;
};

struct dx_input_msg_dispatch {
  dx_msg_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.keyboard_key_action",
                            dx_keyboard_key_action);

enum dx_keyboard_key_action {
  DX_KEYBOARD_KEY_ACTION_PRESSED = 1,
  DX_KEYBOARD_KEY_ACTION_RELEASED = 2,
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
/// @param SELF A pointer to this "keyboard key" message object.
/// @param action The action of the key. Must be #DX_KEYBOARD_KEY_ACTION_PRESSED or #DX_KEYBOARD_KEY_ACTION_RELEASED.
/// @param key The key.
/// @method-call
dx_result dx_keyboard_key_msg_construct(dx_keyboard_key_msg* SELF, dx_keyboard_key_action action, dx_keyboard_key key, uint8_t modifiers);

/// @brief Get the action.
/// @param RETURN A pointer to a <code>dx_keyboard_key_action</code> variable.
/// @param SELF A pointer to this "keyboard key" message object.
/// @success <code>*RETURN</code> was assigned the action.
/// @method-call
dx_result dx_keyboard_key_msg_get_action(dx_keyboard_key_action* RETURN, dx_keyboard_key_msg* SELF);

/// @brief Get the key.
/// @param RETURN A pointer to a <code>dx_keyboard_key</code> variable.
/// @param SELF A pointer to this "keyboard key" message object.
/// @success <code>*RETURN</code> was assigned the keyboard key.
/// @method-call
dx_result dx_keyboard_key_msg_get_key(dx_keyboard_key* RETURN, dx_keyboard_key_msg* SELF);

/// @brief Create a "keyboard key" message.
/// @param RETURN A pointer to a code <code>dx_keyboard_key_msg*</code> variable.
/// @param action The action of the key. Must be #DX_KEYBOARD_KEY_ACTION_PRESSED or #DX_KEYBOARD_KEY_ACTION_RELEASED.
/// @param key The key.
/// @param modifiers The modifiers.
/// @success <code>*RETURN</code> was assigned a pointer to the "keyboard key" message object.
/// @procedure-call
dx_result dx_keyboard_key_msg_create(dx_keyboard_key_msg** RETURN, dx_keyboard_key_action action, dx_keyboard_key key, uint8_t modifiers);

struct dx_keyboard_key_msg {
  dx_input_msg _parent;
  dx_keyboard_key_action action;
  dx_keyboard_key key;
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
/// @param SELF A pointer to this "mouse button" message object.
/// @param action The action of the button. Must be #DX_MOUSE_BUTTON_ACTION_PRESSED or #DX_MOUSE_BUTTON_ACTION_RELEASED.
/// @param button The button.
/// @param modifiers The modifiers.
/// @param x, y
/// The position of the mouse pointer.
/// The origin is at the left/top of the canvas.
/// The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @method-call
dx_result dx_mouse_button_msg_construct(dx_mouse_button_msg* SELF, dx_mouse_button_action action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y);

dx_mouse_button_action dx_mouse_button_msg_get_action(dx_mouse_button_msg* SELF);

dx_mouse_button dx_mouse_button_msg_get_button(dx_mouse_button_msg* SELF);

/// @brief Create a "mouse button" message.
/// @param RETURN A pointer to a code <code>dx_mouse_button_msg*</code> variable.
/// @param action The action of the button. Must be #DX_MOUSE_BUTTON_ACTION_PRESSED or #DX_MOUSE_BUTTON_ACTION_RELEASED.
/// @param button The button.
/// @param modifiers The modifiers.
/// @param x, y
/// The position of the mouse pointer.
/// The origin is at the left/top of the canvas.
/// The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @success <code>*RETURN</code> was assigned a pointer to the "mouse button" message object.
/// @procedure-call
dx_result dx_mouse_button_msg_create(dx_mouse_button_msg** RETURN, dx_mouse_button_action action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y);

struct dx_mouse_button_msg {
  dx_input_msg _parent;
  uint8_t action;
  dx_mouse_button button;
  dx_f32 x, y;
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
/// @param SELF A pointer to this "mouse pointer" message object.
/// @param action The action of the mouse pointer. Must be #DX_MOUSE_POINTER_ACTION_MOVED, DX_MOUSE_POINTER_ACTION_ENTERED, or DX_MOUSE_POINTER_ACTION_EXITED.
/// @param modifiers The modifiers.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @method-call
dx_result dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* SELF, dx_mouse_pointer_action action, uint8_t modifiers, dx_f32 x, dx_f32 y);

/// @brief Get the action.
/// @param RETURN A pointer to a <code>dx_mouse_pointer_action</code> variable.
/// @param SELF A pointer to this "mouse pointer key" message object.
/// @success <code>*RETURN</code> was assigned the action.
/// @method-call
dx_result dx_mouse_pointer_msg_get_action(dx_mouse_pointer_action* RETURN, dx_mouse_pointer_msg* SELF);

/// @brief Create a "mouse pointer" message.
/// @param RETURN A pointer to a <code>dx_mouse_pointer_msg*</code> variable.
/// @param action The action of the mouse pointer. Must be #DX_MOUSE_POINTER_ACTION_MOVED, DX_MOUSE_POINTER_ACTION_ENTERED, or DX_MOUSE_POINTER_ACTION_EXITED.
/// @param modifiers The modifiers.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @success <code>*RETURN</code> was assigned a pointer to the "mouse pointer" message object.
/// @procedure-call
dx_result dx_mouse_pointer_msg_create(dx_mouse_pointer_msg** RETURN, dx_mouse_pointer_action action, uint8_t modifiers, dx_f32 x, dx_f32 y);

struct dx_mouse_pointer_msg {
  dx_input_msg _parent;
  uint8_t action;
  dx_f32 x, y;
};

struct dx_mouse_pointer_msg_dispatch {
  dx_input_msg_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_HAPTICALS_H_INCLUDED
