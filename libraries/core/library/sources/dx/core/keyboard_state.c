#include "dx/core/keyboard_state.h"

Core_defineObjectType("Core.KeyboardState",
                      Core_KeyboardState,
                      Core_Object);

static void Core_KeyboardState_destruct(Core_KeyboardState* SELF)
{/*Intentionally empty.*/}

static void Core_KeyboardState_constructDispatch(Core_KeyboardState_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_KeyboardState_construct(Core_KeyboardState* SELF) {
  Core_BeginConstructor(Core_KeyboardState);
  for (Core_Size i = 0; i < Core_KeyboardState_NumberOfKeyboardKeys; ++i) {
    SELF->state[i] = false;
  }
  Core_EndConstructor(Core_KeyboardState);
}

Core_Result Core_KeyboardState_create(Core_KeyboardState** RETURN) {
  DX_CREATE_PREFIX(Core_KeyboardState);
  if (Core_KeyboardState_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_KeyboardState_getState(Core_Boolean* RETURN, Core_KeyboardState* SELF, Core_KeyboardKey key) {
  if (!RETURN || !SELF || key < 0 || key >= Core_KeyboardState_NumberOfKeyboardKeys) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->state[key];
  return Core_Success;
}

Core_Result Core_KeyboardState_setState(Core_KeyboardState* SELF, Core_KeyboardKey key, Core_Boolean state) {
  if (!SELF || key < 0 || key >= Core_KeyboardState_NumberOfKeyboardKeys) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->state[key] = state;
  return Core_Success;
}

Core_Result Core_KeyboardState_clear(Core_KeyboardState* SELF) {
  for (Core_Size i = 0; i < Core_KeyboardState_NumberOfKeyboardKeys; ++i) {
    SELF->state[i] = false;
  }
  return Core_Success;
}
