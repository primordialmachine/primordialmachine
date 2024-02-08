#include "dx/core/keyboard_state.h"

Core_defineObjectType("dx.keyboard_state",
                      dx_keyboard_state,
                      Core_Object);

static void dx_keyboard_state_destruct(dx_keyboard_state* SELF)
{/*Intentionally empty.*/}

static void dx_keyboard_state_constructDispatch(dx_keyboard_state_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_keyboard_state_construct(dx_keyboard_state* SELF) {
  Core_BeginConstructor(dx_keyboard_state);
  for (Core_Size i = 0; i < dx_keyboard_state_configuration_number_of_keyboard_keys; ++i) {
    SELF->state[i] = false;
  }
  Core_EndConstructor(dx_keyboard_state);
}

Core_Result dx_keyboard_state_create(dx_keyboard_state** RETURN) {
  DX_CREATE_PREFIX(dx_keyboard_state);
  if (dx_keyboard_state_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
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
