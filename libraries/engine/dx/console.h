#if !defined(DX_CONSOLE_H_INCLUDED)
#define DX_CONSOLE_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.console",
                       dx_console,
                       dx_object);

static inline dx_console* DX_CONSOLE(void* p) {
  return (dx_console*)p;
}

struct dx_console {
  dx_object _parent;
};

static inline dx_console_dispatch* DX_CONSOLE_DISPATCH(void* p) {
  return (dx_console_dispatch*)p;
}

struct dx_console_dispatch {
  dx_object_dispatch _parent;
  dx_result (*on_keyboard_key_message)(dx_console*, dx_keyboard_key_msg*);
  dx_result (*render)(dx_console*, dx_f32, dx_i32, dx_i32);
  dx_result (*open)(dx_console*);
  dx_result (*close)(dx_console*);
  dx_result (*toggle)(dx_console*);
  dx_result (*is_open)(dx_bool* RETURN, dx_console* SELF);
  dx_result (*is_closed)(dx_bool* RETURN, dx_console* SELF);
  dx_result(*append_text)(dx_console*, dx_string*);
};

dx_result dx_console_construct(dx_console* SELF);

/// @brief Handle a keyboard key message.
/// @param self A pointer to this console.
/// @method-call
static inline dx_result dx_console_on_keyboard_key_message(dx_console* SELF, dx_keyboard_key_msg* keyboard_key_message) {
  DX_OBJECT_VIRTUALCALL(dx_console, on_keyboard_key_message, SELF, keyboard_key_message);
}

/// @brief Render this console.
/// @param SELF A pointer to this console.
/// @method-call
static inline dx_result dx_console_render(dx_console* SELF, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  DX_OBJECT_VIRTUALCALL(dx_console, render, SELF, delta_seconds, canvas_width, canvas_height);
}

/// @brief Open this console.
/// @param SELF A pointer to this console.
/// @method-call
static inline dx_result dx_console_open(dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, open, SELF);
}

/// @brief Open this console.
/// @param SELF A pointer to this console.
/// @method-call
static inline dx_result dx_console_close(dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, close, SELF);
}

/// @brief Toggle this console.
/// @param SELF A pointer to this console.
/// @method-call
static inline dx_result dx_console_toggle(dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, toggle, SELF);
}

/// @brief Get if this console is open.
/// @param RETURN A pointer to a <code>dx_bool</code> variable.
/// @param SELF A pointer to this console.
/// @success
/// <code>*RETURN</code> was assigned @a DX_TRUE if the console is open.
/// <code>*RETURN</code> was assigned @a DX_FALSE if the console is closed.
/// @method-call
static inline dx_result dx_console_is_open(dx_bool* RETURN, dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, is_open, RETURN, SELF);
}

/// @brief Get if this console is close.
/// @param RETURN A pointer to a <code>dx_bool</code> variable.
/// @param SELF A pointer to this console.
/// @success
/// <code>*RETURN</code> was assigned @a DX_TRUE if the console is closed.
/// <code>*RETURN</code> was assigned @a DX_FALSE if the console is open.
/// @method-call
static dx_result dx_console_is_closed(dx_bool* RETURN, dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, is_closed, RETURN, SELF);
}

/// @brief Append this text to this console.
/// @param SELF a pointer to this console.
/// @param text The text.
/// @method-call
static dx_result dx_console_append_text(dx_console* SELF, dx_string* text) {
  DX_OBJECT_VIRTUALCALL(dx_console, append_text, SELF, text);
}

#endif // DX_CONSOLE_H_INCLUDED
