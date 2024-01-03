#include "dx/core/keyboard_state.h"

DX_DEFINE_OBJECT_TYPE("dx.keyboard_state",
                      dx_keyboard_state,
                      Core_Object);

static void dx_keyboard_state_destruct(dx_keyboard_state* SELF)
{/*Intentionally empty.*/}

static void dx_keyboard_state_constructDispatch(dx_keyboard_state_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_keyboard_state_construct(dx_keyboard_state* SELF) {
  DX_CONSTRUCT_PREFIX(dx_keyboard_state);
  for (Core_Size i = 0; i < dx_keyboard_state_configuration_number_of_keyboard_keys; ++i) {
    SELF->state[i] = false;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_keyboard_state_create(dx_keyboard_state** RETURN) {
  DX_CREATE_PREFIX(dx_keyboard_state);
  if (dx_keyboard_state_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_keyboard_state_get_state(Core_Boolean* RETURN, dx_keyboard_state* SELF, Core_KeyboardKey key) {
  if (!RETURN || !SELF || key < 0 || key >= dx_keyboard_state_configuration_number_of_keyboard_keys) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->state[key];
  return Core_Success;
}

Core_Result dx_keyboard_state_set_state(dx_keyboard_state* SELF, Core_KeyboardKey key, Core_Boolean state) {
  if (!SELF || key < 0 || key >= dx_keyboard_state_configuration_number_of_keyboard_keys) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->state[key] = state;
  return Core_Success;
}

Core_Result dx_keyboard_state_clear(dx_keyboard_state* SELF) {
  for (Core_Size i = 0; i < dx_keyboard_state_configuration_number_of_keyboard_keys; ++i) {
    SELF->state[i] = false;
  }
  return Core_Success;
}
