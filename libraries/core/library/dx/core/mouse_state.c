#include "dx/core/mouse_state.h"

Core_defineObjectType("Core.MouseState",
                      Core_MouseState,
                      Core_Object);

static void Core_MouseState_destruct(Core_MouseState* SELF)
{/*Intentionally empty.*/}

static void Core_MouseState_constructDispatch(Core_MouseState_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_MouseState_construct(Core_MouseState* SELF) {
  Core_BeginConstructor(Core_MouseState);
  SELF->x = 0.f;
  SELF->y = 0.f;
  for (Core_Size i = 0; i < Core_MouseState_NumberOfMouseButtons; ++i) {
    SELF->state[i] = false;
  }
  Core_EndConstructor(Core_MouseState);
}

Core_Result Core_MouseState_create(Core_MouseState** RETURN) {
  DX_CREATE_PREFIX(Core_MouseState);
  if (Core_MouseState_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_MouseState_getButtonState(Core_Boolean* RETURN, Core_MouseState* SELF, Core_MouseButton button) {
  *RETURN = SELF->state[(Core_Size)button];
  return Core_Success;
}

Core_Result Core_MouseState_setButtonState(Core_MouseState* SELF, Core_MouseButton button, Core_Boolean state) {
  SELF->state[(Core_Size)button] = state;
  return Core_Success;
}

Core_Result Core_MouseState_getPointerState(Core_MouseState* SELF, Core_Real32 *x, Core_Real32 *y) {
  *x = SELF->x;
  *y = SELF->y;
  return Core_Success;
}

Core_Result Core_MouseState_setPointerState(Core_MouseState* SELF, Core_Real32 x, Core_Real32 y) {
  SELF->x = x;
  SELF->y = y;
  return Core_Success;
}
