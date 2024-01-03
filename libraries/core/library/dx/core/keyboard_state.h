#if !defined(DX_KEYBOARD_STATE_H_INCLUDED)
#define DX_KEYBOARD_STATE_H_INCLUDED

#include "dx/core/object.h"
#include "dx/core/hapticals.h"

/// @warning Keep this synchronized with keyboard_keys.i.
#define dx_keyboard_state_configuration_number_of_keyboard_keys (255 + 1)

DX_DECLARE_OBJECT_TYPE("dx.keyboard_state",
                       dx_keyboard_state,
                       Core_Object);

static inline dx_keyboard_state* DX_KEYBOARD_STATE(void* p) {
  return (dx_keyboard_state*)p;
}

struct dx_keyboard_state {
  Core_Object _parent;
  Core_Boolean state[dx_keyboard_state_configuration_number_of_keyboard_keys];
};

static inline dx_keyboard_state_dispatch* DX_KEYBOARD_STATE_DISPATCH(void* p) {
  return (dx_keyboard_state_dispatch*)p;
}

struct dx_keyboard_state_dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Construct this keyboard state.
/// @param SELF A pointer to this keyboard state.
/// @success The state of all keys is @a false.
/// @default-runtime-calling-convent
Core_Result dx_keyboard_state_construct(dx_keyboard_state* SELF);

Core_Result dx_keyboard_state_create(dx_keyboard_state** RETURN);

/// @brief Get the state of a keyboard key.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @param SELF A pointer to this keyboard state.
/// @param key The keyboard key.
/// @success <code>*RETURN</code> was assigned the state of the keyboard key.
/// @default-runtime-calling-convent
Core_Result dx_keyboard_state_get_state(Core_Boolean* RETURN, dx_keyboard_state* SELF, Core_KeyboardKey key);

/// @brief Set the state of a keyboard key.
/// @param SELF A pointer to this keyboard state.
/// @param key The keyboard key.
/// @param state The state.
/// @default-runtime-calling-convent
Core_Result dx_keyboard_state_set_state(dx_keyboard_state* SELF, Core_KeyboardKey key, Core_Boolean state);

/// @brief Set the state of all keys to @a false.
/// @param SELF A pointer to this keyboard state.
/// @method-call
Core_Result dx_keyboard_state_clear(dx_keyboard_state* SELF);

#endif // DX_KEYBOARD_STATE_H_INCLUDED
