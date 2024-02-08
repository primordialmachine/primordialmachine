#if !defined(CORE_VAL_GL_SYSTEM_H_INCLUDED)
#define CORE_VAL_GL_SYSTEM_H_INCLUDED

#include "dx/val/system.h"
#include "dx/val/gl/context.h"
#include "dx/val/gl/window.h"

Core_declareObjectType("Core.Val.Gl.System",
                       Core_Val_Gl_System,
                       dx_val_system);

static inline Core_Val_Gl_System* CORE_VAL_GL_SYSTEM(void* p) {
  return (Core_Val_Gl_System*)p;
}

struct Core_Val_Gl_System {
  dx_val_system _parent;
};

static inline Core_Val_Gl_System_Dispatch* CORE_VAL_GL_SYSTEM_DISPATCH(void* p) {
  return (Core_Val_Gl_System_Dispatch*)p;
}

struct Core_Val_Gl_System_Dispatch {
  dx_val_system_Dispatch _parent;
  Core_Result(*getWindow)(dx_val_gl_window**, Core_Val_Gl_System*);
};

/// @param msg_queue A pointer to the message queue.
/// @constructor{Core_Val_Gl_System}
Core_Result Core_Val_Gl_System_construct(Core_Val_Gl_System* SELF, Core_MessageQueue* msg_queue);

/// @brief Get the VAL GL window.
/// @param RETURN A pointer to a <code>dx_val_gl_window*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the VAL GL window object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this VAL GL system is not started.
/// @method{Core_Val_Gl_System}
static inline Core_Result Core_Val_Gl_System_get_window(dx_val_gl_window** RETURN, Core_Val_Gl_System* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Val_Gl_System, getWindow, RETURN, SELF);
}

#endif // CORE_VAL_GL_SYSTEM_H_INCLUDED
