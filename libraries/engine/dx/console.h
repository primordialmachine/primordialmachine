#if !defined(DX_CONSOLE_H_INCLUDED)
#define DX_CONSOLE_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.console",
                       dx_console,
                       Core_Object);

static inline dx_console* DX_CONSOLE(void* p) {
  return (dx_console*)p;
}

struct dx_console {
  Core_Object _parent;
};

static inline dx_console_dispatch* DX_CONSOLE_DISPATCH(void* p) {
  return (dx_console_dispatch*)p;
}

struct dx_console_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*on_keyboard_key_message)(dx_console*, dx_keyboard_key_msg*);
  Core_Result (*render)(dx_console*, Core_Real32, Core_Integer32, Core_Integer32, Core_Integer32, Core_Integer32);
  Core_Result (*open)(dx_console*);
  Core_Result (*close)(dx_console*);
  Core_Result (*toggle)(dx_console*);
  Core_Result (*is_open)(Core_Boolean* RETURN, dx_console* SELF);
  Core_Result (*is_closed)(Core_Boolean* RETURN, dx_console* SELF);
  Core_Result(*append_output_text)(dx_console*, Core_String*);
};

Core_Result dx_console_construct(dx_console* SELF);

/// @brief Handle a keyboard key message.
/// @method{dx_console}
static inline Core_Result dx_console_on_keyboard_key_message(dx_console* SELF, dx_keyboard_key_msg* keyboard_key_message) {
  DX_OBJECT_VIRTUALCALL(dx_console, on_keyboard_key_message, SELF, keyboard_key_message);
}

/// @brief Render this console.
/// @method{dx_console}
static inline Core_Result dx_console_render(dx_console* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_size_horizontal, Core_Integer32 canvas_size_vertical, Core_Integer32 dpi_horizontal, Core_Integer32 dpi_vertical) {
  DX_OBJECT_VIRTUALCALL(dx_console, render, SELF, delta_seconds, canvas_size_horizontal, canvas_size_vertical, dpi_horizontal, dpi_vertical);
}

/// @brief Open this console.
/// @method{dx_console}
static inline Core_Result dx_console_open(dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, open, SELF);
}

/// @brief Open this console.
/// @method{dx_console}
static inline Core_Result dx_console_close(dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, close, SELF);
}

/// @brief Toggle this console.
/// @method{dx_console}
static inline Core_Result dx_console_toggle(dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, toggle, SELF);
}

/// @brief Get if this console is open.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @success
/// <code>*RETURN</code> was assigned @a Core_True if the console is open.
/// <code>*RETURN</code> was assigned @a Core_False if the console is closed.
/// @method{dx_console}
static inline Core_Result dx_console_is_open(Core_Boolean* RETURN, dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, is_open, RETURN, SELF);
}

/// @brief Get if this console is close.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @success
/// <code>*RETURN</code> was assigned @a Core_True if the console is closed.
/// <code>*RETURN</code> was assigned @a Core_False if the console is open.
/// @method{dx_console}
static Core_Result dx_console_is_closed(Core_Boolean* RETURN, dx_console* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_console, is_closed, RETURN, SELF);
}

/// @brief Append text to the output text of this console.
/// @param text The text.
/// @method{dx_console}
static Core_Result dx_console_append_output_text(dx_console* SELF, Core_String* text) {
  DX_OBJECT_VIRTUALCALL(dx_console, append_output_text, SELF, text);
}

#endif // DX_CONSOLE_H_INCLUDED
