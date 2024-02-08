#if !defined(CORE_VAL_GL_WGL_SYSTEM_H_INCLUDED)
#define CORE_VAL_GL_WGL_SYSTEM_H_INCLUDED

#include "dx/val/gl/system.h"
#include "dx/val/gl/wgl/context.h"

Core_declareObjectType("Core.Val.Gl.Wgl.System",
                       Core_Val_Gl_Wgl_System,
                       Core_Val_Gl_System);

static inline Core_Val_Gl_Wgl_System* CORE_VAL_GL_WGL_SYSTEM(void* p) {
  return (Core_Val_Gl_Wgl_System*)p;
}

struct Core_Val_Gl_Wgl_System {
  Core_Val_Gl_System _parent;
  /// @brief The instance handle.
  HINSTANCE instance_handle;
  /// @brief The class name.
  char* class_name;
  /// @brief The class handle.
  ATOM class_handle;
};

static inline Core_Val_Gl_Wgl_System_Dispatch* CORE_VAL_GL_WGL_SYSTEM_DISPATCH(void* p) {
  return (Core_Val_Gl_Wgl_System_Dispatch*)p;
}

struct Core_Val_Gl_Wgl_System_Dispatch {
  Core_Val_Gl_System_Dispatch _parent;
};

/// @param msq_queue A pointer to the message queue.
/// @constructor{Core_Val_Gl_Wgl_System}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result Core_Val_Gl_Wgl_System_construct(Core_Val_Gl_Wgl_System* SELF, Core_MessageQueue* msg_queue);

/// @param msq_queue A pointer to the message queue.
/// @create-operator{Core_Val_Gl_Wgl_System}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result Core_Val_Gl_Wgl_System_create(Core_Val_Gl_Wgl_System** RETURN, Core_MessageQueue* msg_queue);

/// @brief Handle WM_(L|M|R|X)BUTTON(DOWN|UP) messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{Core_Val_Gl_Wgl_System}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result Core_Val_Gl_Wgl_System_onMouseButtonMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @brief Handle WM_MOUSEMOVE messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{Core_Val_Gl_Wgl_System}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result Core_Val_Gl_Wgl_System_onMousePointerMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @brief Handle WM_(KEY|SYSKEY)(DOWN|UP) messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{Core_Val_Gl_Wgl_System}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result Core_Val_Gl_Wgl_System_onKeyboardKeyMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @brief Handle WM_SIZE|WM_ACTIVATE messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{Core_Val_Gl_Wgl_System}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result Core_Val_Gl_Wgl_System_onWindowMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

#endif // CORE_VAL_GL_WGL_SYSTEM_H_INCLUDED
