#if !defined(CORE_VAL_GL_WGL_WINDOW_H_INCLUDED)
#define CORE_VAL_GL_WGL_WINDOW_H_INCLUDED

#include "dx/val/gl/window.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
typedef struct Core_Application Core_Application;

Core_declareObjectType("Core.Val.Gl.Wgl.Window",
                        Core_Val_Gl_Wgl_Window,
                        dx_val_gl_window);

static Core_Val_Gl_Wgl_Window* CORE_VAL_GL_WGL_WINDOW(void* p) {
  return (Core_Val_Gl_Wgl_Window*)p;
}

struct Core_Val_Gl_Wgl_Window {
  dx_val_gl_window _parent;
  HWND wnd;
  HDC dc;
  HGLRC glrc;
  HICON smallIcon;
  HICON bigIcon;
  Core_Application* application;
};

static Core_Val_Gl_Wgl_Window_Dispatch* CORE_VAL_GL_WGL_WINDOW_DISPATCH(void* p) {
  return (Core_Val_Gl_Wgl_Window_Dispatch*)p;
}

struct Core_Val_Gl_Wgl_Window_Dispatch {
  dx_val_gl_window_Dispatch _parent;
};

/// @internal
/// @param application A pointer to the VAL GL/WGL application.
/// @constructor{Core_Val_Gl_Wgl_Window}
Core_Result Core_Val_Gl_Wgl_Window_construct(Core_Val_Gl_Wgl_Window* SELF, Core_Application* application);

/// @internal
/// @param application A pointer to the VAL GL/WGL application.
/// @create-operator{Core_Val_Gl_Wgl_Window}
Core_Result Core_Val_Gl_Wgl_Window_create(Core_Val_Gl_Wgl_Window** RETURN, Core_Application* application);

#endif // CORE_VAL_GL_WGL_WINDOW_H_INCLUDED
