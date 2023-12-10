#if !defined(DX_VAL_GL_WINDOW_H_INCLUDED)
#define DX_VAL_GL_WINDOW_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.val.gl.window",
                       dx_val_gl_window,
                       Core_Object);

static inline dx_val_gl_window* DX_VAL_GL_WINDOW(void* p) {
  return (dx_val_gl_window*)p;
}

struct dx_val_gl_window {
  Core_Object _parent;
};

static inline dx_val_gl_window_dispatch* DX_VAL_GL_WINDOW_DISPATCH(void* p) {
  return (dx_val_gl_window_dispatch*)p;
}

struct dx_val_gl_window_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*get_canvas_size)(dx_val_gl_window*, Core_Integer32*, Core_Integer32*);
};

/// @brief Construct this VAL GL window.
/// @param SELF A pointer to this VAL GL window.
/// @method-call
Core_Result dx_val_gl_window_construct(dx_val_gl_window* SELF);

/// @internal
/// Get the size of the canvas of this VAL GL window.
/// @param SELF A pointer to this VAL GL window.
/// @param width A pointer to a <code>Core_Integer32</code> variable.
/// @param height A pointer to a <code>Core_Integer32</code> variable.
/// @method-call
/// @success
/// <code>*width</code> was assigned the width of the canvas.
/// <code>*height</code> was assigned the height of the canvas.
static inline Core_Result dx_val_gl_window_get_canvas_size(dx_val_gl_window* SELF, Core_Integer32* width, Core_Integer32* height) {
  DX_OBJECT_VIRTUALCALL(dx_val_gl_window, get_canvas_size, SELF, width, height);
}

#endif // DX_VAL_GL_SYSTEM_H_INCLUDED
