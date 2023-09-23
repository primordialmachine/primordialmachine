#if !defined(DX_VAL_GL_WGL_SYSTEM_H_INCLUDED)
#define DX_VAL_GL_WGL_SYSTEM_H_INCLUDED

#include "dx/val/gl/system.h"
#include "dx/val/gl/wgl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.val.gl.wgl.system",
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

static inline dx_val_gl_wgl_system_dispatch* DX_VAL_GL_WGL_SYSTEM_DISPATCH(void* p) {
  return (dx_val_gl_wgl_system_dispatch*)p;
}

struct dx_val_gl_wgl_system_dispatch {
  dx_val_gl_system_dispatch _parent;
};

/// @brief Construct this VAL GL/WGL system.
/// @param SELF A pointer to this system.
/// @param msq_queue A pointer to the message queue.
/// @method-call
dx_result dx_val_gl_wgl_system_construct(dx_val_gl_wgl_system* SELF, dx_msg_queue* msg_queue);

dx_result dx_val_gl_wgl_system_create(dx_val_gl_wgl_system** RETURN, dx_msg_queue* msg_queue);

/// @brief Get the VAL GL/WGL context.
/// @param RETURN A pointer to a <code>dx_gl_wgl_context*</code> variable.
/// @param SELF A pointer to this VAL GL/WGL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the VAL GL/WGL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this VAL GL/WGL system is not started.
/// @method-call
dx_result dx_val_gl_wgl_system_get_context(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_system* SELF);

/// @brief Get the VAL GL/WGL window.
/// @param RETURN A pointer to a <code>dx_val_gl_wgl_window*</code> variable.
/// @param SELF A pointer to this VAL GL/WGL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the VAL GL/WGL window object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this VAL GL/WGL system is not started.
/// @method-call
dx_result dx_val_gl_wgl_system_get_window(dx_val_gl_wgl_window** RETURN, dx_val_gl_wgl_system* SELF);

/// @internal
/// @brief Handle WM_(L|M|R|X)BUTTON(DOWN|UP) messages.
/// @param SELF A pointer to this VAL GL/WGL system.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method-call
dx_result dx_val_gl_wgl_system_on_mouse_button_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @internal
/// @brief Handle WM_MOUSEMOVE messages.
/// @param SELF A pointer to this VAL GL/WGL system.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method-call
dx_result dx_val_gl_wgl_system_on_mouse_pointer_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @internal
/// @brief Handle WM_(KEY|SYSKEY)(DOWN|UP) messages.
/// @param SELF A pointer to this VAL GL/WGL system.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method-call
dx_result dx_val_gl_wgl_system_on_keyboard_key_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// @internal
/// @brief Handle WM_SIZE|WM_ACTIVATE messages.
/// @param SELF A pointer to this VAL GL/WGL system.
/// @param wnd The window handle.
/// @param msg The message.
/// @param wparam The WPARAM parameter.
/// @param lparam The LPARAM parameter.
/// @method-call
dx_result dx_val_gl_wgl_system_on_window_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

#endif // DX_VAL_GL_WGL_SYSTEM_H_INCLUDED
