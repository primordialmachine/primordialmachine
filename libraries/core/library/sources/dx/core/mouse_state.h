#if !defined(CORE_MOUSESTATE_H_INCLUDED)
#define CORE_MOUSESTATE_H_INCLUDED

#include "dx/core/Object.h"
#include "Core/Input/MouseButton.h"

/* WARNING: Must be kept synchronized with `Core/Input/MouseButtons.i`. */
#define Core_MouseState_NumberOfMouseButtons (7)

Core_declareObjectType("Core.MouseState",
                       Core_MouseState,
                       Core_Object);

static inline Core_MouseState* CORE_MOUSESTATE(void* p) {
  return (Core_MouseState*)p;
}

struct Core_MouseState {
  Core_Object _parent;
  Core_Real32 x, y;
  Core_Boolean state[Core_MouseState_NumberOfMouseButtons];
};

static inline Core_MouseState_Dispatch* CORE_MOUSESTATE_DISPATCH(void* p) {
  return (Core_MouseState_Dispatch*)p;
}

struct Core_MouseState_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_MouseState_construct(Core_MouseState* SELF);

Core_Result Core_MouseState_create(Core_MouseState** RETURN);

/// @brief Get the state of a mouse button.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @param SELF A pointer to this mouse state.
/// @param button The mouse button.
/// @success <code>*RETURN</code> was assigned the state of the mouse button.
/// @method{Core_MouseState}
Core_Result Core_MouseState_getButtonState(Core_Boolean* RETURN, Core_MouseState* SELF, Core_MouseButton button);

/// @brief Set the state of a mouse button.
/// @param SELF A pointer to this mouse state.
/// @param button The mouse button.
/// @param state The state of the mouse button.
/// @method{Core_MouseState}
Core_Result Core_MouseState_setButtonState(Core_MouseState* SELF, Core_MouseButton button, Core_Boolean state);

/// @brief Get the state of a mouse pointer.
/// @param SELF A pointer to this mouse state.
/// @param x A pointer to a <code>Core_Real32</code> variable.
/// @param y A pointer to a <code>Core_Real32</code> variable.
/// @success
/// <code>*x</code> was assigned the position of the mouse pointer along the x-axis.
/// <code>*y</code> was assigned the position of the mouse pointer along the y-axis.
/// @method{Core_MouseState}
Core_Result Core_MouseState_getPointerState(Core_MouseState* SELF, Core_Real32 *x, Core_Real32 *y);

/// @brief Set the state of a mouse pointer.
/// @param SELF A pointer to this mouse state.
/// @param x The position of the mouse pointer along the x-axis.
/// @param y The position of the mouse pointer along the y-axis.
/// @method{Core_MouseState}
Core_Result Core_MouseState_setPointerState(Core_MouseState* SELF, Core_Real32 x, Core_Real32 y);

#endif // CORE_MOUSESTATE_H_INCLUDED
