#if !defined(DX_VAL_SYSTEM_H_INCLUDED)
#define DX_VAL_SYSTEM_H_INCLUDED

#include "dx/core.h"
#include "dx/val/context.h"

Core_declareObjectType("Core.Visuals.System",
                       Core_Visuals_System,
                       Core_System);

static inline Core_Visuals_System* CORE_VISUALS_SYSTEM(void* p) {
  return (Core_Visuals_System*)p;
}

struct Core_Visuals_System {
  Core_System _parent;
  /// @brief A weak reference to the message queue.
  Core_MessageQueue* messageQueue;
  /// @brief The state of the keyboard.
  /// @todo This should be window-specific.
  dx_keyboard_state* keyboard_state;
  /// @brief The state of the mouse.
  /// @todo This should be window-specific.
  dx_mouse_state* mouse_state;
};

static inline Core_Visuals_System_Dispatch* CORE_VISUALS_SYSTEM_DISPATCH(void* p) {
  return (Core_Visuals_System_Dispatch*)p;
}

struct Core_Visuals_System_Dispatch {
  Core_System_Dispatch _parent;
  Core_Result (*getContext)(Core_Visuals_Context**, Core_Visuals_System*);
};

/// @param messageQueue A pointer to the message queue.
/// @constructor{Core_Visuals_System}
Core_Result Core_Visuals_System_construct(Core_Visuals_System* SELF, Core_MessageQueue* messageQueue);

/// @brief Get the context.
/// @param RETURN A pointer to a <code>Core_Visuals_Context*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this system is not started.
/// @method{Core_Visuals_System}
static inline Core_Result Core_Visuals_System_getContext(Core_Visuals_Context** RETURN, Core_Visuals_System* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_System, getContext, RETURN, SELF);
}

/// @internal
/// @brief Emit a "keyboard key pressed" message.
/// @param SELF A pointer to this system object.
/// @param key The keyboard key.
/// @param modifierKeys The modifier keys.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitKeyboardKeyPressedMessage(Core_Visuals_System* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "keyboard key released" message.
/// @param SELF A pointer to this system object.
/// @param key The keyboard key.
/// @param modifierKeys The modifier keys.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitKeyboardKeyReleasedMessage(Core_Visuals_System* SELF, Core_KeyboardKey key, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "mouse button pressed" message.
/// @param SELF A pointer to this system object.
/// @param button The mouse button.
/// @param x, y The position.
/// @param modifierKeys The modifier keys.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitMouseButtonPressedMessage(Core_Visuals_System* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "mouse button released" message.
/// @param button The mouse button.
/// @param x, y The position.
/// @param modifierKeys The modifier keys.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitMouseButtonReleasedMessage(Core_Visuals_System* SELF, Core_MouseButton button, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "mouse pointer moved" message.
/// @param x, y The position.
/// @param modifierKeys The modifier keys.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitMousePointerMovedMessage(Core_Visuals_System* SELF, Core_Real32 x, Core_Real32 y, Core_ModifierKeys modifierKeys);

/// @internal
/// @brief Emit a "canvas size changed" message.
/// @param width, height The size.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitCanvasSizeChangedMessage(Core_Visuals_System* SELF, Core_Real32 width, Core_Real32 height);

/// @internal
/// @brief Emit a "canvas activated" message.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitCanvasActivatedMessage(Core_Visuals_System* SELF);

/// @internal
/// @brief Emit a "canvas deactivated" message.
/// @method{Core_Visuals_System}
Core_Result Core_Visuals_System_emitCanvasDeactivatedMessage(Core_Visuals_System* SELF);

#endif // CORE_VISUALS_SYSTEM_H_INCLUDED
