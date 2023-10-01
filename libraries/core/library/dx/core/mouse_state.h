#if !defined(DX_MOUSE_STATE_H_INCLUDED)
#define DX_MOUSE_STATE_H_INCLUDED

#include "dx/core/object.h"
#include "dx/core/hapticals.h"

/// @warning Keep this synchronized with mouse_buttons.i.
#define dx_mouse_state_configuration_number_of_mouse_buttons (7)

DX_DECLARE_OBJECT_TYPE("dx.mouse_state",
                       dx_mouse_state,
                       dx_object);

static inline dx_mouse_state* DX_MOUSE_STATE(void* p) {
  return (dx_mouse_state*)p;
}

struct dx_mouse_state {
  dx_object _parent;
  dx_f32 x, y;
  dx_bool state[dx_mouse_state_configuration_number_of_mouse_buttons];
};

static inline dx_mouse_state_dispatch* DX_MOUSE_STATE_DISPATCH(void* p) {
  return (dx_mouse_state_dispatch*)p;
}

struct dx_mouse_state_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_mouse_state_construct(dx_mouse_state* SELF);

dx_result dx_mouse_state_create(dx_mouse_state** RETURN);

/// @brief Get the state of a mouse button.
/// @param RETURN A pointer to a <code>dx_bool</code> variable.
/// @param SELF A pointer to this mouse state.
/// @param button The mouse button.
/// @success <code>*RETURN</code> was assigned the state of the mouse button.
/// @method-call
dx_result dx_mouse_state_get_button_state(dx_bool* RETURN, dx_mouse_state* SELF, dx_mouse_button button);

/// @brief Set the state of a mouse button.
/// @param SELF A pointer to this mouse state.
/// @param button The mouse button.
/// @param state The state of the mouse button.
/// @method-call
dx_result dx_mouse_state_set_button_state(dx_mouse_state* SELF, dx_mouse_button button, dx_bool state);

/// @brief Get the state of a mouse pointer.
/// @param SELF A pointer to this mouse state.
/// @param x A pointer to a <code>dx_f32</code> variable.
/// @param y A pointer to a <code>dx_f32</code> variable.
/// @success
/// <code>*x</code> was assigned the position of the mouse pointer along the x-axis.
/// <code>*y</code> was assigned the position of the mouse pointer along the y-axis.
/// @method-call
dx_result dx_mouse_state_set_pointer_state(dx_mouse_state* SELF, dx_f32 x, dx_f32 y);

/// @brief Set the state of a mouse pointer.
/// @param SELF A pointer to this mouse state.
/// @param x The position of the mouse pointer along the x-axis.
/// @param y The position of the mouse pointer along the y-axis.
/// @method-call
dx_result dx_mouse_state_set_pointer_state(dx_mouse_state* SELF, dx_f32 x, dx_f32 y);

#endif // DX_MOUSE_STATE_H_INCLUDED