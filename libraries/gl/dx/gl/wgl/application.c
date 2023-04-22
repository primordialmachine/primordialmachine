#include "dx/gl/wgl/application.h"

#include "dx/gl/wgl/messages.h"

// GET_X_LPARAM, GET_Y_LPARAM
#include <windowsx.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_gl_wgl_application* g_application = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result map_keyboard_key(WPARAM source, dx_keyboard_key* target);

static dx_result get_modifiers(uint8_t* RETURN);

/// @details
/// According to MSDN:
/// - VK_LSHIFT
/// - VK_RSHIFT
/// - VK_LCONTROL
/// - VK_RCONTROL
/// - VK_LMENU
/// - VK_RMENU
/// These left- and right- distinguishing constants are available to an application only through the
/// GetKeyboardState, SetKeyboardState, GetAsyncKeyState, GetKeyState, and MapVirtualKey functions.
static WPARAM map_left_right_keys(WPARAM vk, LPARAM lParam);

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

static dx_result update(dx_gl_wgl_application* application);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result map_keyboard_key(WPARAM source, dx_keyboard_key* target) {
  switch (source) {
  #define DEFINE(SOURCE, TARGET) \
  case SOURCE: { \
    *target = TARGET; \
  } break;
    DEFINE('0', dx_keyboard_key__0);
    DEFINE('1', dx_keyboard_key__1);
    DEFINE('2', dx_keyboard_key__2);
    DEFINE('3', dx_keyboard_key__3);
    DEFINE('4', dx_keyboard_key__4);
    DEFINE('5', dx_keyboard_key__5);
    DEFINE('6', dx_keyboard_key__6);
    DEFINE('7', dx_keyboard_key__7);
    DEFINE('8', dx_keyboard_key__8);
    DEFINE('9', dx_keyboard_key__9);


    DEFINE('A', dx_keyboard_key_a);
    DEFINE('B', dx_keyboard_key_b);
    DEFINE('C', dx_keyboard_key_c);
    DEFINE('D', dx_keyboard_key_d);
    DEFINE('E', dx_keyboard_key_e);
    DEFINE('F', dx_keyboard_key_f);
    DEFINE('G', dx_keyboard_key_g);
    DEFINE('H', dx_keyboard_key_h);
    DEFINE('I', dx_keyboard_key_i);
    DEFINE('J', dx_keyboard_key_j);
    DEFINE('K', dx_keyboard_key_k);
    DEFINE('L', dx_keyboard_key_l);
    DEFINE('M', dx_keyboard_key_m);
    DEFINE('N', dx_keyboard_key_n);
    DEFINE('O', dx_keyboard_key_o);
    DEFINE('P', dx_keyboard_key_p);
    DEFINE('Q', dx_keyboard_key_q);
    DEFINE('R', dx_keyboard_key_r);
    DEFINE('S', dx_keyboard_key_s);
    DEFINE('T', dx_keyboard_key_t);
    DEFINE('U', dx_keyboard_key_u);
    DEFINE('V', dx_keyboard_key_v);
    DEFINE('W', dx_keyboard_key_w);
    DEFINE('X', dx_keyboard_key_x);
    DEFINE('Y', dx_keyboard_key_y);
    DEFINE('Z', dx_keyboard_key_z);

    DEFINE(VK_F1, dx_keyboard_key_f1);
    DEFINE(VK_F2, dx_keyboard_key_f2);
    DEFINE(VK_F3, dx_keyboard_key_f3);
    DEFINE(VK_F4, dx_keyboard_key_f4);
    DEFINE(VK_F5, dx_keyboard_key_f5);
    DEFINE(VK_F6, dx_keyboard_key_f6);
    DEFINE(VK_F7, dx_keyboard_key_f7);
    DEFINE(VK_F8, dx_keyboard_key_f8);
    DEFINE(VK_F9,  dx_keyboard_key_f9);
    DEFINE(VK_F10, dx_keyboard_key_f10);
    DEFINE(VK_F11, dx_keyboard_key_f11);
    DEFINE(VK_F12, dx_keyboard_key_f12);
    DEFINE(VK_F13, dx_keyboard_key_f13);
    DEFINE(VK_F14, dx_keyboard_key_f14);
    DEFINE(VK_F15, dx_keyboard_key_f15);
    DEFINE(VK_F16, dx_keyboard_key_f16);
    DEFINE(VK_F17, dx_keyboard_key_f17);
    DEFINE(VK_F18, dx_keyboard_key_f18);
    DEFINE(VK_F19, dx_keyboard_key_f19);
    DEFINE(VK_F20, dx_keyboard_key_f20);
    DEFINE(VK_F21, dx_keyboard_key_f21);
    DEFINE(VK_F22, dx_keyboard_key_f22);
    DEFINE(VK_F23, dx_keyboard_key_f23);
    DEFINE(VK_F24, dx_keyboard_key_f24);

    DEFINE(VK_NUMPAD0, dx_keyboard_key_numpad_0);
    DEFINE(VK_NUMPAD1, dx_keyboard_key_numpad_1);
    DEFINE(VK_NUMPAD2, dx_keyboard_key_numpad_2);
    DEFINE(VK_NUMPAD3, dx_keyboard_key_numpad_3);
    DEFINE(VK_NUMPAD4, dx_keyboard_key_numpad_4);
    DEFINE(VK_NUMPAD5, dx_keyboard_key_numpad_5);
    DEFINE(VK_NUMPAD6, dx_keyboard_key_numpad_6);
    DEFINE(VK_NUMPAD7, dx_keyboard_key_numpad_7);
    DEFINE(VK_NUMPAD8, dx_keyboard_key_numpad_8);
    DEFINE(VK_NUMPAD9, dx_keyboard_key_numpad_9);

    DEFINE(VK_UP, dx_keyboard_key_up);
    DEFINE(VK_DOWN, dx_keyboard_key_down);
    DEFINE(VK_LEFT, dx_keyboard_key_left);
    DEFINE(VK_RIGHT, dx_keyboard_key_right);

    DEFINE(VK_ESCAPE, dx_keyboard_key_escape);
    DEFINE(VK_RETURN, dx_keyboard_key_return);
    DEFINE(VK_OEM_5, dx_keyboard_key_dead_circumflex);

    DEFINE(VK_BACK, dx_keyboard_key_backspace);

    DEFINE(VK_OEM_PERIOD, dx_keyboard_key_period);
    DEFINE(VK_OEM_PLUS, dx_keyboard_key_plus);
    DEFINE(VK_OEM_MINUS, dx_keyboard_key_minus);
    DEFINE(VK_OEM_COMMA, dx_keyboard_key_comma);

    DEFINE(VK_SPACE, dx_keyboard_key_space);

    DEFINE(VK_LSHIFT, dx_keyboard_key_left_shift);
    DEFINE(VK_RSHIFT, dx_keyboard_key_right_shift);
    DEFINE(VK_LCONTROL, dx_keyboard_key_left_control);
    DEFINE(VK_RCONTROL, dx_keyboard_key_right_control);
    DEFINE(VK_LMENU, dx_keyboard_key_left_menu);
    DEFINE(VK_RMENU, dx_keyboard_key_right_menu);

    DEFINE(VK_OEM_2, dx_keyboard_key_number_sign)
    DEFINE(VK_OEM_102, dx_keyboard_key_less);

  #undef DEFINE
    default: {
      dx_set_error(DX_ERROR_NOT_FOUND);
      return DX_FAILURE;
    }
  };
  return DX_SUCCESS;
}

static dx_result get_modifiers(uint8_t* RETURN) {
  uint8_t modifiers = 0;
  dx_bool state;
  dx_keyboard_state* keyboard_state = DX_GL_APPLICATION(dx_gl_wgl_application_get())->keyboard_state;
  //
  dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_left_shift);
  if (state) {
    modifiers |= DX_MODIFIER_LSHIFT;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_right_shift);
  if (state) {
    modifiers |= DX_MODIFIER_RSHIFT;
  }
  //
  dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_left_control);
  if (state) {
    modifiers |= DX_MODIFIER_LCTRL;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_right_control);
  if (state) {
    modifiers |= DX_MODIFIER_RCTRL;
  }
  //
  dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_left_menu);
  if (state) {
    modifiers |= DX_MODIFIER_LMENU;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_right_menu);
  if (state) {
    modifiers |= DX_MODIFIER_RMENU;
  }
  *RETURN = modifiers;
  return DX_SUCCESS;
}

/// @details
/// According to MSDN:
/// - VK_LSHIFT
/// - VK_RSHIFT
/// - VK_LCONTROL
/// - VK_RCONTROL
/// - VK_LMENU
/// - VK_RMENU
/// These left- and right- distinguishing constants are available to an application only through the
/// GetKeyboardState, SetKeyboardState, GetAsyncKeyState, GetKeyState, and MapVirtualKey functions.
static WPARAM map_left_right_keys(WPARAM vk, LPARAM lParam) {
  WPARAM new_vk = vk;
  UINT scancode = (lParam & 0x00ff0000) >> 16;
  int extended = (lParam & 0x01000000) != 0;
  switch (vk) {
    case VK_SHIFT:
      new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
      break;
    case VK_CONTROL:
      new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
      break;
    case VK_MENU:
      new_vk = extended ? VK_RMENU : VK_LMENU;
      break;
    default:
        // not a key we map from generic to left/right specialized
        //  just return it.
      new_vk = vk;
      break;
  }
  return new_vk;
}

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    // keyboard keys
    case WM_KEYUP: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_keyboard_key keyboard_key;
      dx_error old_error = dx_get_error();
      wparam = map_left_right_keys(wparam, lparam);
      if (map_keyboard_key(wparam, &keyboard_key)) {
        if (dx_get_error() == DX_ERROR_NOT_FOUND) {
          dx_set_error(old_error);
          return 0;
        }
        return 0;
      }
      dx_keyboard_state_set_state(DX_GL_APPLICATION(dx_gl_wgl_application_get())->keyboard_state, keyboard_key, false);
      emit_keyboard_key_released_msg(keyboard_key, modifiers);
      return 0;
    } break;
    case WM_KEYDOWN: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_keyboard_key keyboard_key;
      dx_error old_error = dx_get_error();
      wparam = map_left_right_keys(wparam, lparam);
      if (map_keyboard_key(wparam, &keyboard_key)) {
        if (dx_get_error() == DX_ERROR_NOT_FOUND) {
          dx_set_error(old_error);
          return 0;
        }
        return 0;
      }
      dx_keyboard_state_set_state(DX_GL_APPLICATION(dx_gl_wgl_application_get())->keyboard_state, keyboard_key, true);
      emit_keyboard_key_pressed_msg(keyboard_key, modifiers);
      return 0;
    } break;
    // left mouse button
    case WM_LBUTTONDOWN: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      emit_mouse_button_pressed_msg(dx_mouse_button_button_0, x, y, modifiers);
      return 0;
    } break;
    case WM_LBUTTONUP: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_released_msg(dx_mouse_button_button_0, x, y, modifiers);
      return 0;
    } break;
    // middle mouse button
    case WM_MBUTTONDOWN: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      emit_mouse_button_pressed_msg(dx_mouse_button_button_1, x, y, modifiers);
      return 0;
    } break;
    case WM_MBUTTONUP: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_released_msg(dx_mouse_button_button_1, x, y, modifiers);
      return 0;
    } break;
    // right mouse button
    case WM_RBUTTONDOWN: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_pressed_msg(dx_mouse_button_button_2, x, y, modifiers);
      return 0;
    } break;
    case WM_RBUTTONUP: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_released_msg(dx_mouse_button_button_2, x, y, modifiers);
      return 0;
    } break;
    // mouse pointer
    case WM_MOUSEMOVE: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      emit_mouse_pointer_moved_msg(x, y, modifiers);
      return 0;
    } break;
    // application/window
    case WM_SIZE: {
      UINT width = LOWORD(lparam);
      UINT height = HIWORD(lparam);
      emit_canvas_size_changed_msg((dx_f32)width, (dx_f32)height);
      return 0;
    } break;
    case WM_ACTIVATE: {
      if (wparam) {
        // canvas was activated
        emit_canvas_activated_msg();
      } else {
        // canvas was deactivated
        emit_canvas_deactivated_msg();
      }
      return 0;
    } break;
    case WM_SYSKEYUP: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_keyboard_key keyboard_key;
      dx_error old_error = dx_get_error();
      wparam = map_left_right_keys(wparam, lparam);
      if (map_keyboard_key(wparam, &keyboard_key)) {
        if (dx_get_error() == DX_ERROR_NOT_FOUND) {
          dx_set_error(old_error);
          return 0;
        }
        return 0;
      }
      dx_keyboard_state_set_state(DX_GL_APPLICATION(dx_gl_wgl_application_get())->keyboard_state, keyboard_key, false);
      emit_keyboard_key_released_msg(keyboard_key, modifiers);
      return 0;
    } break;
    case WM_SYSKEYDOWN: {
      uint8_t modifiers = 0;
      if (get_modifiers(&modifiers)) {
        return 0;
      }
      dx_keyboard_key keyboard_key;
      dx_error old_error = dx_get_error();
      wparam = map_left_right_keys(wparam, lparam);
      if (map_keyboard_key(wparam, &keyboard_key)) {
        if (dx_get_error() == DX_ERROR_NOT_FOUND) {
          dx_set_error(old_error);
          return 0;
        }
        return 0;
      }
      dx_keyboard_state_set_state(DX_GL_APPLICATION(dx_gl_wgl_application_get())->keyboard_state, keyboard_key, true);
      emit_keyboard_key_pressed_msg(keyboard_key, modifiers);
      return 0;
    } break;
    case WM_CLOSE: {
      emit_quit_msg();
      return 0;
    } break;
    default: {
      return DefWindowProc(wnd, msg, wparam, lparam);
    } break;
  };
}

static dx_result update(dx_gl_wgl_application* application) {
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      emit_quit_msg();
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  // If an error is pending, then this function fails.
  return DX_NO_ERROR != dx_get_error() ? DX_FAILURE : DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.application",
                      dx_gl_wgl_application,
                      dx_gl_application);

static void dx_gl_wgl_application_destruct(dx_gl_wgl_application* self) {
  if (self->class_handle) {
    if (!UnregisterClass(self->class_name, self->instance_handle)) {
      dx_log("unable to unregister window class\n", sizeof("unable to unregister window class\n"));
    }
    self->class_handle = 0;
  }
  if (self->instance_handle) {
    self->instance_handle = 0;
  }
  if (self->class_name) {
    dx_memory_deallocate(self->class_name);
    self->class_name = NULL;
  }
  // >>>
  g_application = NULL;
  // <<<
}

static void dx_gl_wgl_application_dispatch_construct(dx_gl_wgl_application_dispatch* SELF) {
  DX_APPLICATION_DISPATCH(SELF)->update = (dx_result(*)(dx_application*)) & update;
}

dx_result dx_gl_wgl_application_construct(dx_gl_wgl_application* SELF, dx_msg_queue* msg_queue) {
  dx_rti_type* TYPE = dx_gl_wgl_application_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  //
  if (dx_gl_application_construct(DX_GL_APPLICATION(SELF), msg_queue)) {
    return DX_FAILURE;
  }
  //
  SELF->class_name = _strdup("dx.gl.wgl.window.class_name");
  if (!SELF->class_name) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  //
  SELF->instance_handle = GetModuleHandle(NULL);
  if (!SELF->instance_handle) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    dx_memory_deallocate(SELF->class_name);
    SELF->class_name = NULL;
    dx_log("unable to acquire module handle\n", sizeof("unable to acquire module handle\n") - 1);
    return DX_FAILURE;
  }
  //
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
  //
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wcex.lpfnWndProc = &window_procedure;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = SELF->instance_handle;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hIconSm = NULL;
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = NULL;// (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = SELF->class_name;
  SELF->class_handle = RegisterClassEx(&wcex);
  if (!SELF->class_handle) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    SELF->instance_handle = 0;
    dx_memory_deallocate(SELF->class_name);
    SELF->class_name = NULL;
    dx_log("unable to register window class\n", sizeof("unable to register window class\n") - 1);
    return DX_FAILURE;
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  /// >>>
  g_application = SELF;
  /// <<<
  return DX_SUCCESS;
}

dx_result dx_gl_wgl_application_create(dx_gl_wgl_application** RETURN, dx_msg_queue* msg_queue) {
  dx_gl_wgl_application* SELF = DX_GL_WGL_APPLICATION(dx_object_alloc(sizeof(dx_gl_wgl_application)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_gl_wgl_application_construct(SELF, msg_queue)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_gl_wgl_application* dx_gl_wgl_application_get() {
  if (!g_application) {
    return NULL;
  }
  DX_REFERENCE(g_application);
  return g_application;
}
