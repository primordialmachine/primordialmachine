#include "dx/core/mouse_state.h"

Core_defineObjectType("dx.mouse_state",
                      dx_mouse_state,
                      Core_Object);

static void dx_mouse_state_destruct(dx_mouse_state* SELF)
{/*Intentionally empty.*/}

static void dx_mouse_state_constructDispatch(dx_mouse_state_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_mouse_state_construct(dx_mouse_state* SELF) {
  DX_CONSTRUCT_PREFIX(dx_mouse_state);
  SELF->x = 0.f;
  SELF->y = 0.f;
  for (Core_Size i = 0; i < dx_mouse_state_configuration_number_of_mouse_buttons; ++i) {
    SELF->state[i] = false;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_mouse_state_create(dx_mouse_state** RETURN) {
  DX_CREATE_PREFIX(dx_mouse_state);
  if (dx_mouse_state_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_mouse_state_get_button_state(Core_Boolean* RETURN, dx_mouse_state* SELF, Core_MouseButton button) {
  *RETURN = SELF->state[(Core_Size)button];
  return Core_Success;
}

Core_Result dx_mouse_state_set_button_state(dx_mouse_state* SELF, Core_MouseButton button, Core_Boolean state) {
  SELF->state[(Core_Size)button] = state;
  return Core_Success;
}

Core_Result dx_mouse_state_set_pointer_state(dx_mouse_state* SELF, Core_Real32 x, Core_Real32 y) {
  SELF->x = x;
  SELF->y = y;
  return Core_Success;
}
