#if !defined(DX_GL_WGL_WM_H_INCLUDED)
#define DX_GL_WGL_WM_H_INCLUDED

#include "dx/gl/wgl/application.h"
#include "dx/gl/wgl/window.h"
#include "dx/gl/wgl/context.h"

/// @brief Startup the WGL backend.
/// @default-return
/// @default-failure
dx_result dx_val_gl_wgl_startup();

/// @brief Shutdown the WGL backend.
/// @default-return
/// @default-failure
dx_result dx_val_gl_wgl_shutdown();

/// @brief Get the WGL context.
/// @param RETURN A pointer to a <code>dx_gl_wgl_context*</code> variable.
/// @procedure-call
/// @success
/// <code>*RETURN</code> was assigned a pointer to the WGL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if the WGL backend is not started.
dx_result dx_val_gl_wgl_get_context(dx_gl_wgl_context** RETURN);

/// @brief Get the WGL window.
/// @param RETURN A pointer to a <code>dx_gl_wgl_window*</code> variable.
/// @procedure-call
/// @success
/// <code>*RETURN</code> was assigned a pointer to the WGL window object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if the WGL backend is not started.
dx_result dx_val_gl_wgl_get_window(dx_gl_wgl_window** RETURN);

#endif // DX_GL_WGL_WM_H_INCLUDED
