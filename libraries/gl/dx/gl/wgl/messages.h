#if !defined(DX_GL_WGL_MESSAGES_H_INCLUDED)

#include "dx/core.h"

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: quit message emission is traced.
/// - @a 0: quit message emission is not traced.
#define DX_TRACE_QUIT_MESSAGES (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: keyboard key message emission is traced.
/// - @a 0: keyboard key message emission is not traced.
#define DX_TRACE_KEYBOARD_MESSAGES (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: mouse button message emission is traced.
/// - @a 0: mouse button message emission is not traced.
#define DX_TRACE_MOUSE_BUTTON_MESSAGES (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: mouse pointer message emission is traced.
/// - @a 0: mouse pointer message emission is not traced.
#define DX_TRACE_MOUSE_POINTER_MESSAGES (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: canvas message emission is traced.
/// - @a 0: canvas message emission is not traced.
#define DX_TRACE_CANVAS_MESSAGES (0)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result emit_quit_msg();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result emit_keyboard_key_pressed_msg(dx_keyboard_key key, uint8_t modifiers);

dx_result emit_keyboard_key_released_msg(dx_keyboard_key key, uint8_t modifiers);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result emit_mouse_button_pressed_msg(dx_mouse_button button, dx_f32 x, dx_f32 y, uint8_t modifiers);

dx_result emit_mouse_button_released_msg(dx_mouse_button button, dx_f32 x, dx_f32 y, uint8_t modifiers);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result emit_mouse_pointer_moved_msg(dx_f32 x, dx_f32 y, uint8_t modifiers);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result emit_canvas_size_changed_msg(dx_f32 width, dx_f32 height);

dx_result emit_canvas_activated_msg();

dx_result emit_canvas_deactivated_msg();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_GL_WGL_MESSAGES_H_INCLUDED
