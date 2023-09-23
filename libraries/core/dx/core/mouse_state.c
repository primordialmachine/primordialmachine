#include "dx/core/mouse_state.h"

DX_DEFINE_OBJECT_TYPE("dx.mouse_state",
                      dx_mouse_state,
                      dx_object);

static void dx_mouse_state_destruct(dx_mouse_state* SELF)
{/*Intentionally empty.*/}

static void dx_mouse_state_dispatch_construct(dx_mouse_state_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_mouse_state_construct(dx_mouse_state* SELF) {
  dx_rti_type* TYPE = dx_mouse_state_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->x = 0.f;
  SELF->y = 0.f;
  for (dx_size i = 0; i < dx_mouse_state_configuration_number_of_mouse_buttons; ++i) {
    SELF->state[i] = false;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_mouse_state_create(dx_mouse_state** RETURN) {
  dx_mouse_state* SELF = DX_MOUSE_STATE(dx_object_alloc(sizeof(dx_mouse_state)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_mouse_state_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_mouse_state_get_button_state(dx_bool* RETURN, dx_mouse_state* SELF, dx_mouse_button button) {
  *RETURN = SELF->state[(dx_size)button];
  return DX_SUCCESS;
}

dx_result dx_mouse_state_set_button_state(dx_mouse_state* SELF, dx_mouse_button button, dx_bool state) {
  SELF->state[(dx_size)button] = state;
  return DX_SUCCESS;
}

dx_result dx_mouse_state_set_pointer_state(dx_mouse_state* SELF, dx_f32 x, dx_f32 y) {
  SELF->x = x;
  SELF->y = y;
  return DX_SUCCESS;
}
