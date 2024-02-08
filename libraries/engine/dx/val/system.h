#if !defined(DX_VAL_SYSTEM_H_INCLUDED)
#define DX_VAL_SYSTEM_H_INCLUDED

#include "dx/core.h"
#include "dx/val/context.h"

Core_declareObjectType("dx.val.system",
                       dx_val_system,
                       dx_system);

static inline dx_val_system* DX_VAL_SYSTEM(void* p) {
  return (dx_val_system*)p;
}

struct dx_val_system {
  dx_system _parent;
  /// @brief A weak reference to the message queue.
  Core_MessageQueue* msg_queue;
  /// @brief The state of the keyboard.
  /// @todo This should be window-specific.
  dx_keyboard_state* keyboard_state;
  /// @brief The state of the mouse.
  /// @todo This should be window-specific.
  dx_mouse_state* mouse_state;
};

static inline dx_val_system_Dispatch* DX_VAL_SYSTEM_DISPATCH(void* p) {
  return (dx_val_system_Dispatch*)p;
}

struct dx_val_system_Dispatch {
  dx_system_Dispatch _parent;
  Core_Result (*get_context)(dx_val_context**, dx_val_system*);
};

/// @param msg_queue A pointer to the message queue.
/// @constructor{dx_val_system}
Core_Result dx_val_system_construct(dx_val_system* SELF, Core_MessageQueue* msg_queue);

/// @brief Get the VAL context.
/// @param RETURN A pointer to a <code>dx_val_context*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the VAL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this VAL system is not started.
/// @method{dx_val_system}
static inline Core_Result dx_val_system_get_context(dx_val_context** RETURN, dx_val_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_system, get_context, RETURN, SELF); 
}

/// @internal
/// @brief Emit a "keyboard key pressed" message.
/// @param SELF A pointer to this VAL system object.
/// @param key The keyboard key.
/// @param modifierKeys The modifier keys.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_keyboard_key_pressed_msg(dx_val_system* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "keyboard key released" message.
/// @param SELF A pointer to this VAL system object.
/// @param key The keyboard key.
/// @param modifierKeys The modifier keys.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_keyboard_key_released_msg(dx_val_system* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "mouse button pressed" message.
/// @param SELF A pointer to this VAL system object.
/// @param button The mouse button.
/// @param x, y The position.
/// @param modifierKeys The modifier keys.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_mouse_button_pressed_msg(dx_val_system* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "mouse button released" message.
/// @param button The mouse button.
/// @param x, y The position.
/// @param modifierKeys The modifier keys.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_mouse_button_released_msg(dx_val_system* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "mouse pointer moved" message.
/// @param x, y The position.
/// @param modifierKeys The modifier keys.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_mouse_pointer_moved_msg(dx_val_system* SELF, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "canvas size changed" message.
/// @param width, height The size.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_canvas_size_changed_msg(dx_val_system* SELF, Core_Real32 width, Core_Real32 height);

/// @internal
/// @brief Emit a "canvas activated" message.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_canvas_activated_msg(dx_val_system* SELF);

/// @internal
/// @brief Emit a "canvas deactivated" message.
/// @method{dx_val_system}
Core_Result dx_val_system_emit_canvas_deactivated_msg(dx_val_system* SELF);

#endif // DX_VAL_SYSTEM_H_INCLUDED
