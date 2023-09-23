#if !defined(DX_VAL_SYSTEM_H_INCLUDED)
#define DX_VAL_SYSTEM_H_INCLUDED

#include "dx/system.h"
#include "dx/val/context.h"

DX_DECLARE_OBJECT_TYPE("dx.val.system",
                       dx_val_system,
                       dx_system);

static inline dx_val_system* DX_VAL_SYSTEM(void* p) {
  return (dx_val_system*)p;
}

struct dx_val_system {
  dx_system _parent;
  /// @brief A weak reference to the message queue.
  dx_msg_queue* msg_queue;
  /// @brief The state of the keyboard.
  /// @todo This should be window-specific.
  dx_keyboard_state* keyboard_state;
  /// @brief The state of the mouse.
  /// @todo This should be window-specific.
  dx_mouse_state* mouse_state;
};

static inline dx_val_system_dispatch* DX_VAL_SYSTEM_DISPATCH(void* p) {
  return (dx_val_system_dispatch*)p;
}

struct dx_val_system_dispatch {
  dx_system_dispatch _parent;
  dx_result (*get_context)(dx_val_context**, dx_val_system*);
};

/// @brief Construct this VAL system.
/// @param SELF A pointer to this VAL system object.
/// @param msg_queue A pointer to the message queue.
/// @method-call
dx_result dx_val_system_construct(dx_val_system* SELF, dx_msg_queue* msg_queue);

/// @brief Get the VAL context.
/// @param RETURN A pointer to a <code>dx_val_context*</code> variable.
/// @param SELF A pointer to this VAL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the VAL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this VAL system is not started.
/// @method-call
static inline dx_result dx_val_system_get_context(dx_val_context** RETURN, dx_val_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_system, get_context, RETURN, SELF); 
}

/// @internal
/// @brief Emit a "keyboard key pressed" message.
/// @param SELF A pointer to this VAL system object.
/// @param key The keyboard key.
/// @param modifiers The modifiers.
/// @method-call
dx_result dx_val_system_emit_keyboard_key_pressed_msg(dx_val_system* SELF, dx_keyboard_key key, uint8_t modifiers);

/// @internal
/// @brief Emit a "keyboard key released" message.
/// @param SELF A pointer to this VAL system object.
/// @param key The keyboard key.
/// @param modifiers The modifiers.
/// @method-call
dx_result dx_val_system_emit_keyboard_key_released_msg(dx_val_system* SELF, dx_keyboard_key key, uint8_t modifiers);

/// @internal
/// @brief Emit a "mouse button pressed" message.
/// @param SELF A pointer to this VAL system object.
/// @param button The mouse button.
/// @param x, y The position.
/// @param modifiers The modifiers.
/// @method-call
dx_result dx_val_system_emit_mouse_button_pressed_msg(dx_val_system* SELF, dx_mouse_button button, dx_f32 x, dx_f32 y, uint8_t modifiers);

/// @internal
/// @brief Emit a "mouse button released" message.
/// @param SELF A pointer to this VAL system object.
/// @param button The mouse button.
/// @param x, y The position.
/// @param modifiers The modifiers.
/// @method-call
dx_result dx_val_system_emit_mouse_button_released_msg(dx_val_system* SELF, dx_mouse_button button, dx_f32 x, dx_f32 y, uint8_t modifiers);

/// @internal
/// @brief Emit a "mouse pointer moved" message.
/// @param SELF A pointer to this VAL system object.
/// @param x, y The position.
/// @param modifiers The modifiers.
/// @method-call
dx_result dx_val_system_emit_mouse_pointer_moved_msg(dx_val_system* SELF, dx_f32 x, dx_f32 y, uint8_t modifiers);

/// @internal
/// @brief Emit a "canvas size changed" message.
/// @param SELF A pointer to this VAL system object.
/// @param width, height The size.
/// @method-call
dx_result dx_val_system_emit_canvas_size_changed_msg(dx_val_system* SELF, dx_f32 width, dx_f32 height);

/// @internal
/// @brief Emit a "canvas activated" message.
/// @param SELF A pointer to this VAL system object.
/// @method-call
dx_result dx_val_system_emit_canvas_activated_msg(dx_val_system* SELF);

/// @internal
/// @brief Emit a "canvas deactivated" message.
/// @param SELF A pointer to this VAL system object.
/// @method-call
dx_result dx_val_system_emit_canvas_deactivated_msg(dx_val_system* SELF);

#endif // DX_VAL_SYSTEM_H_INCLUDED
