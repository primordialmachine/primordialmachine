#if !defined(DX_GL_APPLICATION_H_INCLUDED)
#define DX_GL_APPLICATION_H_INCLUDED

#include "dx/application.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DX_DECLARE_OBJECT_TYPE("dx.gl.application",
                       dx_gl_application,
                       dx_gl_application)

static dx_gl_application* DX_GL_APPLICATION(void* p) {
  return (dx_gl_application*)p;
}

struct dx_gl_application {
  dx_application _parent;
  /// @brief The message queue.
  dx_msg_queue* msg_queue;
  /// @brief The state of the keyboard.
  dx_keyboard_state* keyboard_state;
  /// @brief The state of the mouse.
  dx_mouse_state* mouse_state;
};

static dx_gl_application_dispatch* DX_GL_APPLICATION_DISPATCH(void* p) {
  return (dx_gl_application_dispatch*)p;
}

struct dx_gl_application_dispatch {
  dx_application_dispatch _parent;
};

/// @internal
dx_result dx_gl_application_construct(dx_gl_application* SELF, dx_msg_queue* msg_queue);

#endif // DX_GL_APPLICATION_H_INCLUDED
