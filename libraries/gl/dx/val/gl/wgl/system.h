#if !defined(DX_VAL_GL_WGL_SYSTEM_H_INCLUDED)
#define DX_VAL_GL_WGL_SYSTEM_H_INCLUDED

#include "dx/val/gl/system.h"
#include "dx/val/gl/wgl/context.h"

Core_declareObjectType("dx.val.gl.wgl.system",
                       dx_val_gl_wgl_system,
                       dx_val_gl_system);

static inline dx_val_gl_wgl_system* DX_VAL_GL_WGL_SYSTEM(void* p) {
  return (dx_val_gl_wgl_system*)p;
}

struct dx_val_gl_wgl_system {
  dx_val_gl_system _parent;
  /// @brief The instance handle.
  HINSTANCE instance_handle;
  /// @brief The class name.
  char* class_name;
  /// @brief The class handle.
  ATOM class_handle;
};

static inline dx_val_gl_wgl_system_Dispatch* DX_VAL_GL_WGL_SYSTEM_DISPATCH(void* p) {
  return (dx_val_gl_wgl_system_Dispatch*)p;
}

struct dx_val_gl_wgl_system_Dispatch {
  dx_val_gl_system_Dispatch _parent;
};

/// @param msq_queue A pointer to the message queue.
/// @constructor{dx_val_gl_wgl_system}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result dx_val_gl_wgl_system_construct(dx_val_gl_wgl_system* SELF, dx_msg_queue* msg_queue);

/// @param msq_queue A pointer to the message queue.
/// @create-operator{dx_val_gl_wgl_system}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result dx_val_gl_wgl_system_create(dx_val_gl_wgl_system** RETURN, dx_msg_queue* msg_queue);

/// @brief Handle WM_(L|M|R|X)BUTTON(DOWN|UP) messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{dx_val_gl_wgl_system}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result dx_val_gl_wgl_system_on_mouse_button_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @brief Handle WM_MOUSEMOVE messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{dx_val_gl_wgl_system}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result dx_val_gl_wgl_system_on_mouse_pointer_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @brief Handle WM_(KEY|SYSKEY)(DOWN|UP) messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{dx_val_gl_wgl_system}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result dx_val_gl_wgl_system_on_keyboard_key_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @brief Handle WM_SIZE|WM_ACTIVATE messages.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method{dx_val_gl_wgl_system}
/// @todo Scope to package. That is, move to "system.package.h".
Core_Result dx_val_gl_wgl_system_on_window_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

#endif // DX_VAL_GL_WGL_SYSTEM_H_INCLUDED
