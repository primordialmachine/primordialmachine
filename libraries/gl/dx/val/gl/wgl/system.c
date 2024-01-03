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

static Core_Result gl_wgl_open_window_internal(dx_val_gl_wgl_window** window, dx_val_gl_wgl_window* existing, Core_Result(*init_wgl)(dx_val_gl_wgl_window*, dx_val_gl_wgl_window*));

static void gl_wgl_close_window_internal(dx_val_gl_wgl_window* window);

static Core_Result gl_wgl_init_wgl_v1(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window*);

static Core_Result gl_wgl_init_wgl_v2(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window*);

static Core_Result gl_wgl_open_window();

static Core_Result gl_wgl_close_window();

static dx_val_gl_wgl_window* g_window = NULL;

static dx_gl_wgl_context* g_context = NULL;

static void gl_wgl_close_window_internal(dx_val_gl_wgl_window* window) {
  DX_UNREFERENCE(window);
  window = NULL;
}

static Core_Result gl_wgl_open_window_internal(dx_val_gl_wgl_window** window, dx_val_gl_wgl_window* existing, Core_Result(*init_wgl)(dx_val_gl_wgl_window*, dx_val_gl_wgl_window*)) {
  dx_gl_wgl_application* application = NULL;
  if (dx_application_get((dx_application**) & application)) {
    return Core_Failure;
  }
  dx_val_gl_wgl_window* window1 = NULL;
  if (dx_gl_wgl_window_create(&window1, application)) {
    DX_UNREFERENCE(application);
    application = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(application);
  application = NULL;
  if (init_wgl) {
    if (init_wgl(window1, existing)) {
      DX_UNREFERENCE(window1);
      window1 = NULL;
      return Core_Failure;
    }
  }

  ShowWindow(window1->wnd, SW_SHOWNORMAL);
  UpdateWindow(window1->wnd);

  *window = window1;

  return Core_Success;
}

static Core_Result gl_wgl_open_window() {
  dx_val_gl_wgl_window* window_v1;
  if (gl_wgl_open_window_internal(&window_v1, NULL, &gl_wgl_init_wgl_v1)) {
    return Core_Failure;
  }
  dx_val_gl_wgl_window* window_v2;
  if (gl_wgl_open_window_internal(&window_v2, window_v1, &gl_wgl_init_wgl_v2)) {
    return Core_Failure;
  }

  gl_wgl_close_window_internal(window_v1);

  if (!wglMakeCurrent(window_v2->dc, window_v2->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n") - 1);
    gl_wgl_close_window_internal(window_v2);
    window_v2 = NULL;
    return Core_Failure;
  }

  g_window = window_v2;

  return Core_Success;
}

static Core_Result gl_wgl_close_window() {
  gl_wgl_close_window_internal(g_window);
  g_window = NULL;
  return Core_Success;
}

static Core_Result gl_wgl_init_wgl_v2(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window* existing) {
  if (!wglMakeCurrent(existing->dc, existing->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n") - 1);
    return Core_Failure;
  }
  // (1) Acquire pointers to essential OpenGL functions.
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
  if (!wglChoosePixelFormat) {
    dx_log("unable to acquire a pointer to the wglChoosePixelFormat function\n", sizeof("unable to acquire a pointer to the wglChoosePixelFormat function\n") - 1);
    return Core_Failure;
  }
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  if (!wglCreateContextAttribs) {
    dx_log("unable to acquire a pointer to the wglCreateContextAttribs function\n", sizeof("unable to acquire a pointer to the wglCreateContextAttribs function\n") - 1);
    return Core_Failure;
  }
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribiv = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
  if (!wglGetPixelFormatAttribiv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n") - 1);
    return Core_Failure;
  }
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfv = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
  if (!wglGetPixelFormatAttribfv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n") - 1);
    return Core_Failure;
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
      return Core_Failure;
    }
    PIXELFORMATDESCRIPTOR desc;
    if (!DescribePixelFormat(window->dc, i, sizeof(desc), &desc)) {
      dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n") - 1);
      return Core_Failure;
    }
    if (!SetPixelFormat(window->dc, i, &desc)) {
      dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n") - 1);
      return Core_Failure;
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
    return Core_Failure;
  }

  return Core_Success;
}

static Core_Result gl_wgl_init_wgl_v1(dx_val_gl_wgl_window* window, dx_val_gl_wgl_window* existing) {
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
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  if (!DescribePixelFormat(window->dc, format, sizeof(desc), &desc)) {
    dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n") - 1);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  if (!SetPixelFormat(window->dc, format, &desc)) {
    dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n") - 1);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  window->glrc = wglCreateContext(window->dc);
  if (!window->glrc) {
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n") - 1);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.val.gl.wgl.system",
                      dx_val_gl_wgl_system,
                      dx_val_gl_system);

/// @internal
/// @brief Determine the mouse button that was pressed/released given the msg, wparam, and lparam arguments of a WM_(L|M|R|X)BUTTON(DOWN|UP) message.
/// @param RETURN A pointer to a <code>Core_KeyboardKey_A</code> value.
/// @aram msg The msg value of the message.
/// @param wparam The wparam value of the message.
/// @param lparam The lparam value of the message.
/// @procedure
/// @error #Core_Error_NotFound if the button is not is not supported.
static Core_Result map_mouse_button(dx_mouse_button* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam);

/// @internal
/// @brief Determine the keyboard key that was pressed/released given the msg, wparam, and lparam arguments of a WM_(KEY|SYSKEY)(DOWN|UP) message.
/// @param RETURN A pointer to a <code>Core_KeyboardKey_A</code> value.
/// @aram msg The msg value of the message.
/// @param wparam The wparam value of the message.
/// @param lparam The lparam value of the message.
/// @procedure
/// @error #Core_Error_NotFound if the key is not supported.
static Core_Result map_keyboard_key(Core_KeyboardKey* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam);

static Core_Result get_modifiers(uint8_t* RETURN, dx_val_gl_wgl_system* SELF);

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

static Core_Result startup(dx_val_gl_wgl_system* SELF);

static Core_Result shutdown(dx_val_gl_wgl_system* SELF);

static Core_Result get_context(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_system* SELF);

static Core_Result get_window(dx_val_gl_wgl_window** RETURN, dx_val_gl_wgl_system* SELF);

static Core_Result map_mouse_button(dx_mouse_button* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam) {
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
      Core_setError(Core_Error_NotFound);
      return Core_Failure;
    } break;
  };
  return Core_Success;
}

static Core_Result map_keyboard_key(Core_KeyboardKey* RETURN, dx_val_gl_wgl_system* SELF, UINT msg, WPARAM wparam, LPARAM lparam) {
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
    /*dx_log("mapped to ", sizeof("mapped to ") - 1);*/ \
    /*dx_log(dx_keyboard_key_to_string(TARGET), strlen(dx_keyboard_key_to_string(TARGET)));*/ \
    /*dx_log("\n", sizeof("\n") - 1);*/ \
    *RETURN = TARGET; \
  } break;
    DEFINE('0', Core_KeyboardKey_D0);
    DEFINE('1', Core_KeyboardKey_D1);
    DEFINE('2', Core_KeyboardKey_D2);
    DEFINE('3', Core_KeyboardKey_D3);
    DEFINE('4', Core_KeyboardKey_D4);
    DEFINE('5', Core_KeyboardKey_D5);
    DEFINE('6', Core_KeyboardKey_D6);
    DEFINE('7', Core_KeyboardKey_D7);
    DEFINE('8', Core_KeyboardKey_D8);
    DEFINE('9', Core_KeyboardKey_D9);


    DEFINE('A', Core_KeyboardKey_A);
    DEFINE('B', Core_KeyboardKey_B);
    DEFINE('C', Core_KeyboardKey_C);
    DEFINE('D', Core_KeyboardKey_D);
    DEFINE('E', Core_KeyboardKey_E);
    DEFINE('F', Core_KeyboardKey_F);
    DEFINE('G', Core_KeyboardKey_G);
    DEFINE('H', Core_KeyboardKey_H);
    DEFINE('I', Core_KeyboardKey_I);
    DEFINE('J', Core_KeyboardKey_J);
    DEFINE('K', Core_KeyboardKey_K);
    DEFINE('L', Core_KeyboardKey_L);
    DEFINE('M', Core_KeyboardKey_M);
    DEFINE('N', Core_KeyboardKey_N);
    DEFINE('O', Core_KeyboardKey_O);
    DEFINE('P', Core_KeyboardKey_P);
    DEFINE('Q', Core_KeyboardKey_Q);
    DEFINE('R', Core_KeyboardKey_R);
    DEFINE('S', Core_KeyboardKey_S);
    DEFINE('T', Core_KeyboardKey_T);
    DEFINE('U', Core_KeyboardKey_U);
    DEFINE('V', Core_KeyboardKey_V);
    DEFINE('W', Core_KeyboardKey_W);
    DEFINE('X', Core_KeyboardKey_X);
    DEFINE('Y', Core_KeyboardKey_Y);
    DEFINE('Z', Core_KeyboardKey_Z);

    DEFINE(VK_F1, Core_KeyboardKey_F1);
    DEFINE(VK_F2, Core_KeyboardKey_F2);
    DEFINE(VK_F3, Core_KeyboardKey_F3);
    DEFINE(VK_F4, Core_KeyboardKey_F4);
    DEFINE(VK_F5, Core_KeyboardKey_F5);
    DEFINE(VK_F6, Core_KeyboardKey_F6);
    DEFINE(VK_F7, Core_KeyboardKey_F7);
    DEFINE(VK_F8, Core_KeyboardKey_F8);
    DEFINE(VK_F9, Core_KeyboardKey_F9);
    DEFINE(VK_F10, Core_KeyboardKey_F10);
    DEFINE(VK_F11, Core_KeyboardKey_F11);
    DEFINE(VK_F12, Core_KeyboardKey_F12);
    DEFINE(VK_F13, Core_KeyboardKey_F13);
    DEFINE(VK_F14, Core_KeyboardKey_F14);
    DEFINE(VK_F15, Core_KeyboardKey_F15);
    DEFINE(VK_F16, Core_KeyboardKey_F16);
    DEFINE(VK_F17, Core_KeyboardKey_F17);
    DEFINE(VK_F18, Core_KeyboardKey_F18);
    DEFINE(VK_F19, Core_KeyboardKey_F19);
    DEFINE(VK_F20, Core_KeyboardKey_F20);
    DEFINE(VK_F21, Core_KeyboardKey_F21);
    DEFINE(VK_F22, Core_KeyboardKey_F22);
    DEFINE(VK_F23, Core_KeyboardKey_F23);
    DEFINE(VK_F24, Core_KeyboardKey_F24);

    DEFINE(VK_NUMPAD0, Core_KeyboardKey_NumPad0);
    DEFINE(VK_NUMPAD1, Core_KeyboardKey_NumPad1);
    DEFINE(VK_NUMPAD2, Core_KeyboardKey_NumPad2);
    DEFINE(VK_NUMPAD3, Core_KeyboardKey_NumPad3);
    DEFINE(VK_NUMPAD4, Core_KeyboardKey_NumPad4);
    DEFINE(VK_NUMPAD5, Core_KeyboardKey_NumPad5);
    DEFINE(VK_NUMPAD6, Core_KeyboardKey_NumPad6);
    DEFINE(VK_NUMPAD7, Core_KeyboardKey_NumPad7);
    DEFINE(VK_NUMPAD8, Core_KeyboardKey_NumPad8);
    DEFINE(VK_NUMPAD9, Core_KeyboardKey_NumPad9);

    DEFINE(VK_UP, Core_KeyboardKey_UpArrow);
    DEFINE(VK_DOWN, Core_KeyboardKey_DownArrow);
    DEFINE(VK_LEFT, Core_KeyboardKey_LeftArrow);
    DEFINE(VK_RIGHT, Core_KeyboardKey_RightArrow);

    DEFINE(VK_ESCAPE, Core_KeyboardKey_Escape);
    DEFINE(VK_RETURN, Core_KeyboardKey_Return);
    DEFINE(VK_OEM_5, Core_KeyboardKey_DeadCircumflex);
    DEFINE(VK_OEM_6, Core_KeyboardKey_DeadGrave);

    DEFINE(VK_BACK, Core_KeyboardKey_Backspace);

    DEFINE(VK_OEM_PERIOD, Core_KeyboardKey_Period);
    DEFINE(VK_OEM_PLUS, Core_KeyboardKey_Plus);
    DEFINE(VK_OEM_MINUS, Core_KeyboardKey_Minus);
    DEFINE(VK_OEM_COMMA, Core_KeyboardKey_Comma);

    DEFINE(VK_SPACE, Core_KeyboardKey_Space);

    DEFINE(VK_LSHIFT, Core_KeyboardKey_LeftShift);
    DEFINE(VK_RSHIFT, Core_KeyboardKey_RightShift);
    DEFINE(VK_LCONTROL, Core_KeyboardKey_LeftControl);
    DEFINE(VK_RCONTROL, Core_KeyboardKey_RightControl);
    DEFINE(VK_LMENU, Core_KeyboardKey_LeftMenu);
    DEFINE(VK_RMENU, Core_KeyboardKey_RightMenu);

    DEFINE(VK_OEM_2, Core_KeyboardKey_NumberSign);
    DEFINE(VK_OEM_102, Core_KeyboardKey_Less);
  #undef DEFINE
    default: {
      Core_setError(Core_Error_NotFound);
      return Core_Failure;
    }
  };
  return Core_Success;
}

static Core_Result get_modifiers(uint8_t* RETURN, dx_val_gl_wgl_system* SELF) {
  uint8_t modifiers = 0;
  Core_Boolean state;
  dx_keyboard_state* keyboard_state = DX_VAL_SYSTEM(SELF)->keyboard_state;
  //
  if (dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_LeftShift)) {
    return Core_Failure;
  }
  if (state) {
    modifiers |= DX_MODIFIER_LSHIFT;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_RightShift);
  if (state) {
    modifiers |= DX_MODIFIER_RSHIFT;
  }
  //
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_LeftControl);
  if (state) {
    modifiers |= DX_MODIFIER_LCTRL;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_RightControl);
  if (state) {
    modifiers |= DX_MODIFIER_RCTRL;
  }
  //
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_LeftMenu);
  if (state) {
    modifiers |= DX_MODIFIER_LMENU;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_RightMenu);
  if (state) {
    modifiers |= DX_MODIFIER_RMENU;
  }
  *RETURN = modifiers;
  return Core_Success;
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
        Core_setError(Core_Error_NoError); // Ignore the error.
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
        Core_setError(Core_Error_NoError); // Ignore the error.
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
        Core_setError(Core_Error_NoError); // Ignore the error.
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

static Core_Result startup(dx_val_gl_wgl_system* SELF) {
  if (gl_wgl_open_window()) {
    return Core_Failure;
  }
  if (dx_gl_wgl_context_create(&g_context, g_window)) {
    gl_wgl_close_window();
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown(dx_val_gl_wgl_system* SELF) {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  gl_wgl_close_window();
  return Core_Success;
}

static Core_Result get_context(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_system* SELF) {
  if (!g_context) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return Core_Success;
}

static Core_Result get_window(dx_val_gl_wgl_window** RETURN, dx_val_gl_wgl_system* SELF) {
  if (!g_window) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  DX_REFERENCE(g_window);
  *RETURN = g_window;
  return Core_Success;
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

static void dx_val_gl_wgl_system_constructDispatch(dx_val_gl_wgl_system_dispatch* SELF) {
  DX_SYSTEM_DISPATCH(SELF)->startup = (Core_Result(*)(dx_system*)) & startup;
  DX_SYSTEM_DISPATCH(SELF)->shutdown = (Core_Result(*)(dx_system*)) & shutdown;
  DX_VAL_SYSTEM_DISPATCH(SELF)->get_context = (Core_Result(*)(dx_val_context**,dx_val_system*)) & get_context;
  DX_VAL_GL_SYSTEM_DISPATCH(SELF)->get_window = (Core_Result(*)(dx_val_gl_window**,dx_val_gl_system*)) & get_window;
}

Core_Result dx_val_gl_wgl_system_construct(dx_val_gl_wgl_system* SELF, dx_msg_queue* msg_queue) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_wgl_system);
  if (dx_val_gl_system_construct(DX_VAL_GL_SYSTEM(SELF), msg_queue)) {
    return Core_Failure;
  }
  //
  SELF->instance_handle = GetModuleHandle(NULL);
  if (!SELF->instance_handle) {
    Core_setError(Core_Error_EnvironmentFailed);
    dx_log("unable to acquire module handle\n", sizeof("unable to acquire module handle\n") - 1);
    return Core_Failure;
  }
  //
  SELF->class_name = _strdup("dx.gl.wgl.window.class_name");
  if (!SELF->class_name) {
    Core_setError(Core_Error_AllocationFailed);
    dx_log("unable to create window class name\n", sizeof("unable to create window class name\n") - 1);
    SELF->instance_handle = NULL;
    return Core_Failure;
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
    Core_setError(Core_Error_EnvironmentFailed);
    dx_log("unable to register window class\n", sizeof("unable to register window class\n") - 1);
    free(SELF->class_name);
    SELF->class_name = NULL;
    SELF->instance_handle = NULL;
    return Core_Failure;
  }
  //
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_gl_wgl_system_create(dx_val_gl_wgl_system** RETURN, dx_msg_queue* msg_queue) {
  DX_CREATE_PREFIX(dx_val_gl_wgl_system);
  if (dx_val_gl_wgl_system_construct(SELF, msg_queue)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_gl_wgl_system_on_mouse_button_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Core_Real32 x = (Core_Real32)(int)GET_X_LPARAM(lparam);
  Core_Real32 y = (Core_Real32)(int)GET_Y_LPARAM(lparam);
  dx_mouse_button mouse_button;
  if (map_mouse_button(&mouse_button, SELF, msg, wparam, lparam)) {
    return Core_Failure;
  }
  uint8_t modifiers = 0;
  if (get_modifiers(&modifiers, SELF)) {
    return Core_Failure;
  }
  Core_Boolean old;
  if (dx_mouse_state_get_button_state(&old, DX_VAL_SYSTEM(SELF)->mouse_state, mouse_button)) {
    return Core_Failure;
  }
  Core_Boolean new = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_XBUTTONDOWN)
    ? true
    : false;
  dx_mouse_state_set_pointer_state(DX_VAL_SYSTEM(SELF)->mouse_state, x, y);
  dx_mouse_state_set_button_state(DX_VAL_SYSTEM(SELF)->mouse_state, mouse_button, new);
  if (old != new) {
    if (new) {
      if (dx_val_system_emit_mouse_button_pressed_msg(DX_VAL_SYSTEM(SELF), mouse_button, x, y, modifiers)) {
        return Core_Failure;
      }
    } else {
      if (dx_val_system_emit_mouse_button_released_msg(DX_VAL_SYSTEM(SELF), mouse_button, x, y, modifiers)) {
        return Core_Failure;
      }
    }
  }
  return Core_Success;
}

Core_Result dx_val_gl_wgl_system_on_mouse_pointer_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Core_Real32 x = (Core_Real32)(int)GET_X_LPARAM(lparam);
  Core_Real32 y = (Core_Real32)(int)GET_Y_LPARAM(lparam);
  uint8_t modifiers = 0;
  if (get_modifiers(&modifiers, SELF)) {
    return Core_Failure;
  }
  dx_mouse_state_set_pointer_state(DX_VAL_SYSTEM(SELF)->mouse_state, x, y);
  if (dx_val_system_emit_mouse_pointer_moved_msg(DX_VAL_SYSTEM(SELF), x, y, modifiers)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_val_gl_wgl_system_on_keyboard_key_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Core_KeyboardKey keyboard_key;
  if (map_keyboard_key(&keyboard_key, SELF, msg, wparam, lparam)) {
    return Core_Failure;
  }
  uint8_t modifiers = 0;
  if (get_modifiers(&modifiers, SELF)) {
    return Core_Failure;
  }
  Core_Boolean old;
  if (dx_keyboard_state_get_state(&old, DX_VAL_SYSTEM(SELF)->keyboard_state, keyboard_key)) {
    return Core_Failure;
  }
  Core_Boolean new = msg == WM_KEYDOWN ? true : false;
  dx_keyboard_state_set_state(DX_VAL_SYSTEM(SELF)->keyboard_state, keyboard_key, new);
  if (old != new) {
    if (msg == WM_KEYUP) {
      if (dx_val_system_emit_keyboard_key_released_msg(DX_VAL_SYSTEM(SELF), keyboard_key, modifiers)) {
        return Core_Failure;
      }
    } else {
      if (dx_val_system_emit_keyboard_key_pressed_msg(DX_VAL_SYSTEM(SELF), keyboard_key, modifiers)) {
        return Core_Failure;
      }
    }
  }
  return Core_Success;
}

Core_Result dx_val_gl_wgl_system_on_window_message(dx_val_gl_wgl_system* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_ACTIVATE: {
      if (wparam) {
        // canvas was activated
        if (dx_val_system_emit_canvas_activated_msg(DX_VAL_SYSTEM(SELF))) {
          return Core_Failure;
        }
      } else {
        // canvas was deactivated
        if (dx_val_system_emit_canvas_deactivated_msg(DX_VAL_SYSTEM(SELF))) {
          return Core_Failure;
        }
      }
      return Core_Success;
    } break;
    case WM_SIZE: {
      UINT width = LOWORD(lparam);
      UINT height = HIWORD(lparam);
      if (dx_val_system_emit_canvas_size_changed_msg(DX_VAL_SYSTEM(SELF), (Core_Real32)width, (Core_Real32)height)) {
        return Core_Failure;
      }
      return Core_Success;
    } break;
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };
  return Core_Success;
}