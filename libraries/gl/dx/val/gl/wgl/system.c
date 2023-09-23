#include "dx/val/gl/wgl/system.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <windowsx.h>

#include <malloc.h>

#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")

#include <GL/gl.h>
#include <GL/glext.h>

#include <GL/wglext.h>

#pragma comment (lib, "opengl32.lib")

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on Nvidia Optimus systems
// with up-to-date drivers
__declspec(dllexport) DWORD NvOptimusEnablement = 1;

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on AMD PowerXpress systems
// with up-to-date drivers
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include "dx/application.h"
#include "dx/val/gl/wgl/window.h"
#include "dx/val/gl/wgl/context.h"

static dx_result gl_wgl_open_window_internal(dx_val_gl_wgl_window** window, dx_val_gl_wgl_window* existing, dx_result(*init_wgl)(dx_val_gl_wgl_window*, dx_val_gl_wgl_window*));

static void gl_wgl_close_window_internal(dx_val_gl_wgl_window* window);

static dx_result gl_wgl_init_wgl_v1(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window*);

static dx_result gl_wgl_init_wgl_v2(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window*);

static dx_result gl_wgl_open_window();

static dx_result gl_wgl_close_window();

static dx_result gl_wgl_startup();

static dx_result gl_wgl_shutdown();

static dx_result gl_wgl_get_context(dx_gl_wgl_context** RETURN);

static dx_result gl_wgl_get_window(dx_val_gl_wgl_window** RETURN);

static dx_val_gl_wgl_window* g_window = NULL;

static dx_gl_wgl_context* g_context = NULL;

static void gl_wgl_close_window_internal(dx_val_gl_wgl_window* window) {
  DX_UNREFERENCE(window);
  window = NULL;
}

static dx_result gl_wgl_open_window_internal(dx_val_gl_wgl_window** window, dx_val_gl_wgl_window* existing, dx_result(*init_wgl)(dx_val_gl_wgl_window*, dx_val_gl_wgl_window*)) {
  dx_gl_wgl_application* application = NULL;
  if (dx_application_get((dx_application**) & application)) {
    return DX_FAILURE;
  }
  dx_val_gl_wgl_window* window1 = NULL;
  if (dx_gl_wgl_window_create(&window1, application)) {
    DX_UNREFERENCE(application);
    application = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(application);
  application = NULL;
  if (init_wgl) {
    if (init_wgl(window1, existing)) {
      DX_UNREFERENCE(window1);
      window1 = NULL;
      return DX_FAILURE;
    }
  }

  ShowWindow(window1->wnd, SW_SHOWNORMAL);
  UpdateWindow(window1->wnd);

  *window = window1;

  return DX_SUCCESS;
}

static dx_result gl_wgl_open_window() {
  dx_val_gl_wgl_window* window_v1;
  if (gl_wgl_open_window_internal(&window_v1, NULL, &gl_wgl_init_wgl_v1)) {
    return DX_FAILURE;
  }
  dx_val_gl_wgl_window* window_v2;
  if (gl_wgl_open_window_internal(&window_v2, window_v1, &gl_wgl_init_wgl_v2)) {
    return DX_FAILURE;
  }

  gl_wgl_close_window_internal(window_v1);

  if (!wglMakeCurrent(window_v2->dc, window_v2->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n") - 1);
    gl_wgl_close_window_internal(window_v2);
    window_v2 = NULL;
    return DX_FAILURE;
  }

  g_window = window_v2;

  return DX_SUCCESS;
}

static dx_result gl_wgl_close_window() {
  gl_wgl_close_window_internal(g_window);
  g_window = NULL;
  return DX_SUCCESS;
}

static dx_result gl_wgl_init_wgl_v2(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window* existing) {
  if (!wglMakeCurrent(existing->dc, existing->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n") - 1);
    return DX_FAILURE;
  }
  // (1) Acquire pointers to essential OpenGL functions.
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
  if (!wglChoosePixelFormat) {
    dx_log("unable to acquire a pointer to the wglChoosePixelFormat function\n", sizeof("unable to acquire a pointer to the wglChoosePixelFormat function\n") - 1);
    return DX_FAILURE;
  }
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  if (!wglCreateContextAttribs) {
    dx_log("unable to acquire a pointer to the wglCreateContextAttribs function\n", sizeof("unable to acquire a pointer to the wglCreateContextAttribs function\n") - 1);
    return DX_FAILURE;
  }
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribiv = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
  if (!wglGetPixelFormatAttribiv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n") - 1);
    return DX_FAILURE;
  }
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfv = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
  if (!wglGetPixelFormatAttribfv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n") - 1);
    return DX_FAILURE;
  }
  // (2) Choose a pixel format.
  const int samples = 0;
  const int pixel_format_attribs[] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 24,
    WGL_ALPHA_BITS_ARB, 8,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_SAMPLE_BUFFERS_ARB, samples > 0 ? 1 : 0,
    WGL_SAMPLES_ARB, samples,
    WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
    0, 0
  };
  {
    int i, n;
    if (!wglChoosePixelFormat(window->dc, pixel_format_attribs, 0, 1, &i, &n)) {
      dx_log("unable to choose pixel format\n", sizeof("unable to choose pixel format\n") - 1);
      return DX_FAILURE;
    }
    PIXELFORMATDESCRIPTOR desc;
    if (!DescribePixelFormat(window->dc, i, sizeof(desc), &desc)) {
      dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n") - 1);
      return DX_FAILURE;
    }
    if (!SetPixelFormat(window->dc, i, &desc)) {
      dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n") - 1);
      return DX_FAILURE;
    }
  }
  const int context_attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 1,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  window->glrc = wglCreateContextAttribs(window->dc, NULL, context_attribs);
  if (!window->glrc) {
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n") - 1);
    return DX_FAILURE;
  }

  return DX_SUCCESS;
}

static dx_result gl_wgl_init_wgl_v1(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window* existing) {
  PIXELFORMATDESCRIPTOR desc = {
    .nSize = sizeof(desc),
    .nVersion = 1,
    .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 24,
  };
  int format = ChoosePixelFormat(window->dc, &desc);
  if (!format) {
    dx_log("unable to choose pixel format\n", sizeof("unable to choose pixel format\n") - 1);
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  if (!DescribePixelFormat(window->dc, format, sizeof(desc), &desc)) {
    dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n") - 1);
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  if (!SetPixelFormat(window->dc, format, &desc)) {
    dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n") - 1);
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  window->glrc = wglCreateContext(window->dc);
  if (!window->glrc) {
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n") - 1);
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result gl_wgl_startup() {
  if (gl_wgl_open_window()) {
    return DX_FAILURE;
  }
  if (dx_gl_wgl_context_create(&g_context, g_window)) {
    gl_wgl_close_window();
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result gl_wgl_shutdown() {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  gl_wgl_close_window();
  return DX_SUCCESS;
}

static dx_result gl_wgl_get_context(dx_gl_wgl_context** RETURN) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}

static dx_result gl_wgl_get_window(dx_val_gl_wgl_window** RETURN) {
  if (!g_window) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_window);
  *RETURN = g_window;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.val.gl.wgl.system",
                      dx_val_gl_wgl_system,
                      dx_val_gl_system);

/// @internal
/// @brief Determine the mouse button that was pressed/released given the msg, wparam, and lparam arguments of a WM_(L|M|R|X)BUTTON(DOWN|UP) message.
/// @param RETURN A pointer to a <code>dx_keyboard_key</code> value.
/// @aram msg The msg value of the message.
/// @param wparam The wparam value of the message.
/// @param lparam The lparam value of the message.
/// @procedure-call
/// @error #DX_ERROR_NOT_FOUND if the button is not is not supported.
static dx_result map_mouse_button(dx_mouse_button* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam);

/// @internal
/// @brief Determine the keyboard key that was pressed/released given the msg, wparam, and lparam arguments of a WM_(KEY|SYSKEY)(DOWN|UP) message.
/// @param RETURN A pointer to a <code>dx_keyboard_key</code> value.
/// @aram msg The msg value of the message.
/// @param wparam The wparam value of the message.
/// @param lparam The lparam value of the message.
/// @procedure-call
/// @error #DX_ERROR_NOT_FOUND if the key is not supported.
static dx_result map_keyboard_key(dx_keyboard_key* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam);

static dx_result get_modifiers(uint8_t* RETURN, dx_val_gl_wgl_system* SELF);

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

static dx_result startup(dx_val_gl_wgl_system* SELF);

static dx_result shutdown(dx_val_gl_wgl_system* SELF);

static dx_result get_context(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_system* SELF);

static dx_result get_window(dx_val_gl_wgl_window** RETURN, dx_val_gl_wgl_system* SELF);

static dx_result map_mouse_button(dx_mouse_button* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_LBUTTONDOWN:
      *RETURN = dx_mouse_button_button_0;
      break;
    case WM_LBUTTONUP:
      *RETURN = dx_mouse_button_button_0;
      break;
    case WM_MBUTTONDOWN:
      *RETURN = dx_mouse_button_button_1;
      break;
    case WM_MBUTTONUP:
      *RETURN = dx_mouse_button_button_1;
      break;
    case WM_RBUTTONDOWN:
      *RETURN = dx_mouse_button_button_2;
      break;
    case WM_RBUTTONUP:
      *RETURN = dx_mouse_button_button_2;
      break;
    case WM_XBUTTONDOWN:
      *RETURN = dx_mouse_button_button_3;
      break;
    case WM_XBUTTONUP:
      *RETURN = dx_mouse_button_button_3;
      break;
    default: {
      dx_set_error(DX_ERROR_NOT_FOUND);
      return DX_FAILURE;
    } break;
  };
  return DX_SUCCESS;
}

static dx_result map_keyboard_key(dx_keyboard_key* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam) {
  // Windows simply sends shift, control, and menu messages.
  // We want to distinguish between the left and the right variants of those keys.
  // The method is awkward.
  UINT scancode = (lparam & 0x00ff0000) >> 16;
  int extended = (lparam & 0x01000000) != 0;
  switch (wparam) {
    case VK_SHIFT:
      wparam = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
      break;
    case VK_CONTROL:
      wparam = extended ? VK_RCONTROL : VK_LCONTROL;
      break;
    case VK_MENU:
      wparam = extended ? VK_RMENU : VK_LMENU;
      break;
    default:
      // Not a key we map from generic to left/right specialized. Just return it.
      wparam = wparam;
      break;
  }
  // We can now map the key.
  switch (wparam) {
  #define DEFINE(SOURCE, TARGET) \
  case SOURCE: { \
    *RETURN = TARGET; \
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
    DEFINE(VK_F9, dx_keyboard_key_f9);
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

static dx_result get_modifiers(uint8_t* RETURN, dx_val_gl_wgl_system* SELF) {
  uint8_t modifiers = 0;
  dx_bool state;
  dx_keyboard_state* keyboard_state = DX_VAL_SYSTEM(SELF)->keyboard_state;
  //
  if (dx_keyboard_state_get_state(&state, keyboard_state, dx_keyboard_key_left_shift)) {
    return DX_FAILURE;
  }
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

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    // keyboard key messages
    case WM_KEYUP:
    case WM_KEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (dx_val_gl_wgl_system_on_keyboard_key_message(DX_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        dx_set_error(DX_NO_ERROR); // Ignore the error.
        DX_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      DX_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    // mouse button messages
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (dx_val_gl_wgl_system_on_mouse_button_message(DX_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        dx_set_error(DX_NO_ERROR); // Ignore the error.
        DX_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      DX_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    // mouse pointer messages
    case WM_MOUSEMOVE: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (dx_val_gl_wgl_system_on_mouse_pointer_message(DX_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        dx_set_error(DX_NO_ERROR); // Ignore the error.
        DX_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      DX_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    // application/window
    case WM_ACTIVATE:
    case WM_SIZE: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (dx_val_gl_wgl_system_on_window_message(DX_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        DX_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      DX_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    case WM_CLOSE: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (dx_application_emit_quit_msg(DX_APPLICATION(app))) {
        DX_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      DX_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    default: {
      return DefWindowProc(wnd, msg, wparam, lparam);
    } break;
  };
}

static dx_result startup(dx_val_gl_wgl_system* SELF) {
  if (gl_wgl_open_window()) {
    return DX_FAILURE;
  }
  if (dx_gl_wgl_context_create(&g_context, g_window)) {
    gl_wgl_close_window();
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result shutdown(dx_val_gl_wgl_system* SELF) {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  gl_wgl_close_window();
  return DX_SUCCESS;
}

static dx_result get_context(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_system* SELF) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}

static dx_result get_window(dx_val_gl_wgl_window** RETURN, dx_val_gl_wgl_system* SELF) {
  if (!g_window) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_window);
  *RETURN = g_window;
  return DX_SUCCESS;
}

static void dx_val_gl_wgl_system_destruct(dx_val_gl_wgl_system* SELF) {
  if (SELF->class_handle) {
    if (!UnregisterClass(SELF->class_name, SELF->instance_handle)) {
      dx_log("unable to unregister window class\n", sizeof("unable to unregister window class\n") - 1);
    }
    SELF->class_handle = 0;
  }
  if (SELF->class_name) {
    free(SELF->class_name);
    SELF->class_name = NULL;
  }
  SELF->instance_handle = NULL;
}

static void dx_val_gl_wgl_system_dispatch_construct(dx_val_gl_wgl_system_dispatch* SELF) {
  DX_SYSTEM_DISPATCH(SELF)->startup = (dx_result(*)(dx_system*)) & startup;
  DX_SYSTEM_DISPATCH(SELF)->shutdown = (dx_result(*)(dx_system*)) & shutdown;
  DX_VAL_SYSTEM_DISPATCH(SELF)->get_context = (dx_result(*)(dx_val_context**,dx_val_system*)) & get_context;
  DX_VAL_GL_SYSTEM_DISPATCH(SELF)->get_window = (dx_result(*)(dx_val_gl_window**,dx_val_gl_system*)) & get_window;
}

dx_result dx_val_gl_wgl_system_construct(dx_val_gl_wgl_system* SELF, dx_msg_queue* msg_queue) {
  dx_rti_type* TYPE = dx_val_gl_wgl_system_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_gl_system_construct(DX_VAL_GL_SYSTEM(SELF), msg_queue)) {
    return DX_FAILURE;
  }
  //
  SELF->instance_handle = GetModuleHandle(NULL);
  if (!SELF->instance_handle) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    dx_log("unable to acquire module handle\n", sizeof("unable to acquire module handle\n") - 1);
    return DX_FAILURE;
  }
  //
  SELF->class_name = _strdup("dx.gl.wgl.window.class_name");
  if (!SELF->class_name) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    dx_log("unable to create window class name\n", sizeof("unable to create window class name\n") - 1);
    SELF->instance_handle = NULL;
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
    dx_log("unable to register window class\n", sizeof("unable to register window class\n") - 1);
    free(SELF->class_name);
    SELF->class_name = NULL;
    SELF->instance_handle = NULL;
    return DX_FAILURE;
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_create(dx_val_gl_wgl_system** RETURN, dx_msg_queue* msg_queue) {
  dx_val_gl_wgl_system* SELF = DX_VAL_GL_WGL_SYSTEM(dx_object_alloc(sizeof(dx_val_gl_wgl_system)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_val_gl_wgl_system_construct(SELF, msg_queue)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_get_context(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_system* SELF) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_get_window(dx_val_gl_wgl_window** RETURN, dx_val_gl_wgl_system* SELF) {
  if (!g_window) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_window);
  *RETURN = g_window;
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_on_mouse_button_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
  dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
  dx_mouse_button mouse_button;
  if (map_mouse_button(&mouse_button, SELF, msg, wparam, lparam)) {
    return DX_FAILURE;
  }
  uint8_t modifiers = 0;
  if (get_modifiers(&modifiers, SELF)) {
    return DX_FAILURE;
  }
  dx_bool old;
  if (dx_mouse_state_get_button_state(&old, DX_VAL_SYSTEM(SELF)->mouse_state, mouse_button)) {
    return DX_FAILURE;
  }
  dx_bool new = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_XBUTTONDOWN)
    ? true
    : false;
  dx_mouse_state_set_pointer_state(DX_VAL_SYSTEM(SELF)->mouse_state, x, y);
  dx_mouse_state_set_button_state(DX_VAL_SYSTEM(SELF)->mouse_state, mouse_button, new);
  if (old != new) {
    if (new) {
      if (dx_val_system_emit_mouse_button_pressed_msg(DX_VAL_SYSTEM(SELF), mouse_button, x, y, modifiers)) {
        return DX_FAILURE;
      }
    } else {
      if (dx_val_system_emit_mouse_button_released_msg(DX_VAL_SYSTEM(SELF), mouse_button, x, y, modifiers)) {
        return DX_FAILURE;
      }
    }
  }
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_on_mouse_pointer_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
  dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
  uint8_t modifiers = 0;
  if (get_modifiers(&modifiers, SELF)) {
    return DX_FAILURE;
  }
  dx_mouse_state_set_pointer_state(DX_VAL_SYSTEM(SELF)->mouse_state, x, y);
  if (dx_val_system_emit_mouse_pointer_moved_msg(DX_VAL_SYSTEM(SELF), x, y, modifiers)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_on_keyboard_key_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  dx_keyboard_key keyboard_key;
  if (map_keyboard_key(&keyboard_key, SELF, msg, wparam, lparam)) {
    return DX_FAILURE;
  }
  uint8_t modifiers = 0;
  if (get_modifiers(&modifiers, SELF)) {
    return DX_FAILURE;
  }
  dx_bool old;
  if (dx_keyboard_state_get_state(&old, DX_VAL_SYSTEM(SELF)->keyboard_state, keyboard_key)) {
    return DX_FAILURE;
  }
  dx_bool new = msg == WM_KEYDOWN ? true : false;
  dx_keyboard_state_set_state(DX_VAL_SYSTEM(SELF)->keyboard_state, keyboard_key, new);
  if (old != new) {
    if (msg == WM_KEYUP) {
      if (dx_val_system_emit_keyboard_key_released_msg(DX_VAL_SYSTEM(SELF), keyboard_key, modifiers)) {
        return DX_FAILURE;
      }
    } else {
      if (dx_val_system_emit_keyboard_key_pressed_msg(DX_VAL_SYSTEM(SELF), keyboard_key, modifiers)) {
        return DX_FAILURE;
      }
    }
  }
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_on_window_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_ACTIVATE: {
      if (wparam) {
        // canvas was activated
        if (dx_val_system_emit_canvas_activated_msg(DX_VAL_SYSTEM(SELF))) {
          return DX_FAILURE;
        }
      } else {
        // canvas was deactivated
        if (dx_val_system_emit_canvas_deactivated_msg(DX_VAL_SYSTEM(SELF))) {
          return DX_FAILURE;
        }
      }
      return DX_SUCCESS;
    } break;
    case WM_SIZE: {
      UINT width = LOWORD(lparam);
      UINT height = HIWORD(lparam);
      if (dx_val_system_emit_canvas_size_changed_msg(DX_VAL_SYSTEM(SELF), (dx_f32)width, (dx_f32)height)) {
        return DX_FAILURE;
      }
      return DX_SUCCESS;
    } break;
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    } break;
  };
  return DX_SUCCESS;
}