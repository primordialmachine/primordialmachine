#if !defined(DX_GL_WGL_WINDOW_H_INCLUDED)
#define DX_GL_WGL_WINDOW_H_INCLUDED

#include "dx/core.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
typedef struct dx_gl_wgl_application dx_gl_wgl_application;

DX_DECLARE_OBJECT_TYPE("dx.gl.wgl.window",
                        dx_gl_wgl_window,
                        dx_object);

  static dx_gl_wgl_window* DX_GL_WGL_WINDOW(void* p) {
  return (dx_gl_wgl_window*)p;
}

struct dx_gl_wgl_window {
  dx_object _parent;
  HWND wnd;
  HDC dc;
  HGLRC glrc;
  dx_gl_wgl_application* application;
};

struct dx_gl_wgl_window_dispatch {
  dx_object_dispatch _parent;
};

/// @internal
/// @brief Construct this window.
/// @param SELF A pointer to this window.
/// @param application A pointer to the application.
/// @default-return
/// @default-failure
dx_result dx_gl_wgl_window_construct(dx_gl_wgl_window* SELF, dx_gl_wgl_application* application);

/// @internal
/// @brief Create a window.
/// @param [out] RETURN A pointer to the return variable.
/// @default-return
/// @default-failure
dx_result dx_gl_wgl_window_create(dx_gl_wgl_window** RETURN, dx_gl_wgl_application* application);

/// @internal
/// Get the size of the canvas of this window.
/// @param SELF A pointer to this window.
/// @param width A pointer to a <code>dx_i32</code> variable.
/// @param height A pointer to a <code>dx_i32</code> variable.
/// @default-return
/// @default-failure
/// @success
/// <code>*width</code> was assigned the width of the canvas.
/// <code>*height</code> was assigned the height of the canvas.
dx_result dx_gl_wgl_window_get_canvas_size(dx_gl_wgl_window* SELF, dx_i32* width, dx_i32* height);

#endif // DX_GL_WGL_WINDOW_H_INCLUDED
