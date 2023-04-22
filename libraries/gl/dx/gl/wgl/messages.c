#include "dx/gl/wgl/messages.h"

#include "dx/gl/wgl/application.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(DX_TRACE_QUIT_MESSAGES) || ((DX_TRACE_QUIT_MESSAGES != 1) && (DX_TRACE_QUIT_MESSAGES != 0))
  #error("DX_TRACE_QUIT_MESSAGES must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE_QUIT_MESSAGES
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

dx_result emit_quit_msg() {
  ENTER(__func__);
  // create the "quit" message.
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = DX_MSG(dx_quit_msg_create());
  if (!msg) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

#undef LEAVE
#undef ENTER

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(DX_TRACE_KEYBOARD_MESSAGES) || ((DX_TRACE_KEYBOARD_MESSAGES != 1) && (DX_TRACE_KEYBOARD_MESSAGES != 0))
  #error("DX_TRACE_KEYBOARD_MESSAGES must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE_KEYBOARD_MESSAGES
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

dx_result emit_keyboard_key_pressed_msg(dx_keyboard_key key, uint8_t modifiers) {
  ENTER(__func__);
  // create the "keyboard key" message.
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = DX_MSG(dx_keyboard_key_msg_create(DX_KEYBOARD_KEY_ACTION_PRESSED, key, modifiers));
  if (!msg) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

dx_result emit_keyboard_key_released_msg(dx_keyboard_key key, uint8_t modifiers) {
  ENTER(__func__);
  // create the "keyboard key" message.
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = DX_MSG(dx_keyboard_key_msg_create(DX_KEYBOARD_KEY_ACTION_RELEASED, key, modifiers));
  if (!msg) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

#undef LEAVE
#undef ENTER

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(DX_TRACE_MOUSE_BUTTON_MESSAGES) || ((DX_TRACE_MOUSE_BUTTON_MESSAGES != 1) && (DX_TRACE_MOUSE_BUTTON_MESSAGES != 0))
  #error("DX_TRACE_MOUSE_BUTTON_MESSAGES must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE_MOUSE_BUTTON_MESSAGES
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

dx_result emit_mouse_button_pressed_msg(dx_mouse_button button, dx_f32 x, dx_f32 y, uint8_t modifiers) {
  ENTER(__func__);
  // create the "mouse button" message.
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = DX_MSG(dx_mouse_button_msg_create(DX_MOUSE_BUTTON_ACTION_PRESSED, button, modifiers, x, y));
  if (!msg) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

dx_result emit_mouse_button_released_msg(dx_mouse_button button, dx_f32 x, dx_f32 y, uint8_t modifiers) {
  ENTER(__func__);
  // create the "mouse button" message.
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = DX_MSG(dx_mouse_button_msg_create(DX_MOUSE_BUTTON_ACTION_RELEASED, button, modifiers, x, y));
  if (!msg) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

#undef LEAVE
#undef ENTER

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(DX_TRACE_MOUSE_POINTER_MESSAGES) || ((DX_TRACE_MOUSE_POINTER_MESSAGES != 1) && (DX_TRACE_MOUSE_POINTER_MESSAGES != 0))
  #error("DX_TRACE_MOUSE_POINTER_MESSAGES must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE_MOUSE_POINTER_MESSAGES
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

dx_result emit_mouse_pointer_moved_msg(dx_f32 x, dx_f32 y, uint8_t modifiers) {
  ENTER(__func__);
  // create the "mouse pointer" message.
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = DX_MSG(dx_mouse_pointer_msg_create(DX_MOUSE_POINTER_ACTION_MOVED, modifiers, x, y));
  if (!msg) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

#undef LEAVE
#undef ENTER

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(DX_TRACE_CANVAS_MESSAGES) || ((DX_TRACE_CANVAS_MESSAGES != 1) && (DX_TRACE_CANVAS_MESSAGES != 0))
  #error("DX_TRACE_CANVAS_MESSAGES must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE_CANVAS_MESSAGES
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

dx_result emit_canvas_size_changed_msg(dx_f32 width, dx_f32 height) {
  ENTER(__func__);
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = NULL;
  if (dx_canvas_size_changed_msg_create((dx_canvas_size_changed_msg**)&msg, width, height)) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

dx_result emit_canvas_activated_msg() {
  ENTER(__func__);
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = NULL;
  if (dx_canvas_msg_create((dx_canvas_msg**)&msg, dx_canvas_msg_kind_activated)) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

dx_result emit_canvas_deactivated_msg() {
  ENTER(__func__);
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  dx_msg* msg = NULL;
  if (dx_canvas_msg_create((dx_canvas_msg**) & msg, dx_canvas_msg_kind_deactivated)) {
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(DX_GL_APPLICATION(application)->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    LEAVE(__func__);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  DX_UNREFERENCE(application);
  application = NULL;
  LEAVE(__func__);
  return DX_SUCCESS;
}

#undef LEAVE
#undef ENTER

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
