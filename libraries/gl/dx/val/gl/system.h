#if !defined(DX_VAL_GL_SYSTEM_H_INCLUDED)
#define DX_VAL_GL_SYSTEM_H_INCLUDED

#include "dx/val/system.h"
#include "dx/val/gl/context.h"
#include "dx/val/gl/window.h"

DX_DECLARE_OBJECT_TYPE("dx.val.gl.system",
                       dx_val_gl_system,
                       dx_val_system);

static inline dx_val_gl_system* DX_VAL_GL_SYSTEM(void* p) {
  return (dx_val_gl_system*)p;
}

struct dx_val_gl_system {
  dx_val_system _parent;
};

static inline dx_val_gl_system_dispatch* DX_VAL_GL_SYSTEM_DISPATCH(void* p) {
  return (dx_val_gl_system_dispatch*)p;
}

struct dx_val_gl_system_dispatch {
  dx_val_system_dispatch _parent;
  Core_Result(*get_window)(dx_val_gl_window**, dx_val_gl_system*);
};

/// @param msg_queue A pointer to the message queue.
/// @constructor{dx_val_gl_system}
Core_Result dx_val_gl_system_construct(dx_val_gl_system* SELF, dx_msg_queue* msg_queue);

/// @brief Get the VAL GL window.
/// @param RETURN A pointer to a <code>dx_val_gl_window*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the VAL GL window object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this VAL GL system is not started.
/// @method{dx_val_gl_system}
static inline Core_Result dx_val_gl_system_get_window(dx_val_gl_window** RETURN, dx_val_gl_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_gl_system, get_window, RETURN, SELF);
}

#endif // DX_VAL_GL_SYSTEM_H_INCLUDED
