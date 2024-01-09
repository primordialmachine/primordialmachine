#include "dx/val/gl/window.h"

Core_defineObjectType("dx.val.gl.window",
                      dx_val_gl_window,
                      Core_Object);

static void dx_val_gl_window_destruct(dx_val_gl_window* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_window_constructDispatch(dx_val_gl_window_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_gl_window_construct(dx_val_gl_window* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_window);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
