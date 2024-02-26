#if !defined(DX_KEYBOARD_STATE_H_INCLUDED)
#define DX_KEYBOARD_STATE_H_INCLUDED

#include "dx/core/Object.h"
#include "Core/Input/KeyboardKey.h"

/* WARNING: Must be kept synchronized with `Core/Input/KeyboardKeys.i`. */
#define Core_KeyboardState_NumberOfKeyboardKeys (255 + 1)

Core_declareObjectType("Core.KeyboardState",
                       Core_KeyboardState,
                       Core_Object);

static inline Core_KeyboardState* CORE_KEYBOARD_STATE(void* p) {
  return (Core_KeyboardState*)p;
}

struct Core_KeyboardState {
  Core_Object _parent;
  Core_Boolean state[Core_KeyboardState_NumberOfKeyboardKeys];
};

static inline Core_KeyboardState_Dispatch* CORE_KEYBOARDSTATE_DISPATCH(void* p) {
  return (Core_KeyboardState_Dispatch*)p;
}

struct Core_KeyboardState_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Construct this keyboard state.
/// @success The state of all keys is @a false.
/// @constructor{Core_KeyboardState}
Core_Result Core_KeyboardState_construct(Core_KeyboardState* SELF);

/// @create-operator{Core_KeyboardState}
Core_Result Core_KeyboardState_create(Core_KeyboardState** RETURN);

/// @brief Get the state of a keyboard key.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @param key The keyboard key.
/// @success <code>*RETURN</code> was assigned the state of the keyboard key.
/// @method{Core_KeyboardState}
Core_Result Core_KeyboardState_getState(Core_Boolean* RETURN, Core_KeyboardState* SELF, Core_KeyboardKey key);

/// @brief Set the state of a keyboard key.
/// @param key The keyboard key.
/// @param state The state.
/// @method{Core_KeyboardState}
Core_Result Core_KeyboardState_setState(Core_KeyboardState* SELF, Core_KeyboardKey key, Core_Boolean state);

/// @brief Set the state of all keys to @a false.
/// @method{Core_KeyboardState}
Core_Result Core_KeyboardState_clear(Core_KeyboardState* SELF);

#endif // DX_KEYBOARD_STATE_H_INCLUDED
