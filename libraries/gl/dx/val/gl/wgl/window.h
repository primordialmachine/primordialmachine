#if !defined(DX_VAL_GL_WGL_WINDOW_H_INCLUDED)
#define DX_VAL_GL_WGL_WINDOW_H_INCLUDED

#include "dx/val/gl/window.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
typedef struct dx_gl_wgl_application dx_gl_wgl_application;

Core_declareObjectType("dx.val.gl.wgl.window",
                        dx_val_gl_wgl_window,
                        dx_val_gl_window);

static dx_val_gl_wgl_window* DX_VAL_GL_WGL_WINDOW(void* p) {
  return (dx_val_gl_wgl_window*)p;
}

struct dx_val_gl_wgl_window {
  dx_val_gl_window _parent;
  HWND wnd;
  HDC dc;
  HGLRC glrc;
  dx_gl_wgl_application* application;
};

struct dx_val_gl_wgl_window_Dispatch {
  dx_val_gl_window_Dispatch _parent;
};

/// @internal
/// @brief Construct this VAL GL/WGL window.
/// @param SELF A pointer to this VAL GL/WGL window.
/// @param application A pointer to the VAL GL/WGL application.
/// @method-call
Core_Result dx_val_gl_wgl_window_construct(dx_val_gl_wgl_window* SELF, dx_gl_wgl_application* application);

/// @internal
/// @brief Create a VAL GL/WGL window.
/// @param [out] RETURN A pointer to a <code>dx_val_gl_wgl_window*</code> variable.
/// @param application A pointer to the VAL GL/WGL application.
/// @success <code>*RETURN</code> was assigned a pointer to the VAL GL/WGL window.
/// @method-call
Core_Result dx_gl_wgl_window_create(dx_val_gl_wgl_window** RETURN, dx_gl_wgl_application* application);

#endif // DX_VAL_GL_WGL_WINDOW_H_INCLUDED
