#if !defined(DX_KEYBOARD_STATE_H_INCLUDED)
#define DX_KEYBOARD_STATE_H_INCLUDED

#include "dx/core/object.h"
#include "dx/core/hapticals.h"

/// @warning Keep this synchronized with keyboard_keys.i.
#define dx_keyboard_state_configuration_number_of_keyboard_keys (255)

DX_DECLARE_OBJECT_TYPE("dx.keyboard_state",
                       dx_keyboard_state,
                       dx_object);

static inline dx_keyboard_state* DX_KEYBOARD_STATE(void* p) {
  return (dx_keyboard_state*)p;
}

struct dx_keyboard_state {
  dx_object _parent;
  dx_bool state[dx_keyboard_state_configuration_number_of_keyboard_keys + 1];
};

static inline dx_keyboard_state_dispatch* DX_KEYBOARD_STATE_DISPATCH(void* p) {
  return (dx_keyboard_state_dispatch*)p;
}

struct dx_keyboard_state_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this keyboard state.
/// @param SELF A pointer to this keyboard state.
/// @success The state of all keys is @a false.
/// @default-runtime-calling-convent
dx_result dx_keyboard_state_construct(dx_keyboard_state* SELF);

dx_result dx_keyboard_state_create(dx_keyboard_state** RETURN);

/// @brief Get the state of a keyboard key.
/// @param RETURN A pointer to a <code>dx_bool</code> variable.
/// @param SELF A pointer to this keyboard state.
/// @param key The keyboard key.
/// @success <code>*RETURN</code> was assigned the state of the keyboard key.
/// @default-runtime-calling-convent
dx_result dx_keyboard_state_get_state(dx_bool* RETURN, dx_keyboard_state* SELF, dx_keyboard_key key);

/// @brief Set the state of a keyboard key.
/// @param SELF A pointer to this keyboard state.
/// @param key The keyboard key.
/// @param state The state.
/// @default-runtime-calling-convent
dx_result dx_keyboard_state_set_state(dx_keyboard_state* SELF, dx_keyboard_key key, dx_bool state);

/// @brief Set the state of all keys to @a false.
/// @param SELF A pointer to this keyboard state.
/// @default-runtime-calling-convention
dx_result dx_keyboard_state_clear(dx_keyboard_state* SELF);

#endif // DX_KEYBOARD_STATE_H_INCLUDED
