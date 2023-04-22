#include "dx/core/keyboard_state.h"

DX_DEFINE_OBJECT_TYPE("dx.keyboard_state",
                      dx_keyboard_state,
                      dx_object);

static void dx_keyboard_state_destruct(dx_keyboard_state* SELF)
{/*Intentionally empty.*/}

static void dx_keyboard_state_dispatch_construct(dx_keyboard_state_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_keyboard_state_construct(dx_keyboard_state* SELF) {
  dx_rti_type* TYPE = dx_keyboard_state_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < dx_keyboard_state_configuration_number_of_keyboard_keys; ++i) {
    SELF->state[i] = false;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_keyboard_state_create(dx_keyboard_state** RETURN) {
  dx_keyboard_state* SELF = DX_KEYBOARD_STATE(dx_object_alloc(sizeof(dx_keyboard_state)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_keyboard_state_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_keyboard_state_get_state(dx_bool* RETURN, dx_keyboard_state* SELF, dx_keyboard_key key) {
  if (!RETURN || !SELF || key < 0 || key >= dx_keyboard_state_configuration_number_of_keyboard_keys) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->state[key];
  return DX_SUCCESS;
}

dx_result dx_keyboard_state_set_state(dx_keyboard_state* SELF, dx_keyboard_key key, dx_bool state) {
  if (!SELF || key < 0 || key >= dx_keyboard_state_configuration_number_of_keyboard_keys) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->state[key] = state;
  return DX_SUCCESS;
}

dx_result dx_keyboard_state_clear(dx_keyboard_state* SELF) {
  for (dx_size i = 0; i < dx_keyboard_state_configuration_number_of_keyboard_keys; ++i) {
    SELF->state[i] = false;
  }
  return DX_SUCCESS;
}
