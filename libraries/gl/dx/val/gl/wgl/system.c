#include "dx/val/gl/wgl/system.h"

#include "dx/val/gl/wgl/Helper.h"

#include <stdio.h>

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

#include "dx/application.h"
#include "dx/val/gl/wgl/window.h"
#include "dx/val/gl/wgl/context.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on Nvidia Optimus systems
// with up-to-date drivers
__declspec(dllexport) DWORD NvOptimusEnablement = 1;

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on AMD PowerXpress systems
// with up-to-date drivers
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

static Core_Result gl_wgl_open_window_internal(Core_Val_Gl_Wgl_Window** window, Core_Result(*init_wgl)(Core_Val_Gl_Wgl_Window*));

static void gl_wgl_close_window_internal(Core_Val_Gl_Wgl_Window* window);

static Core_Result gl_wgl_init_wgl(Core_Val_Gl_Wgl_Window* window);

static Core_Result gl_wgl_open_window();

static Core_Result gl_wgl_close_window();

static Core_Val_Gl_Wgl_Window* g_window = NULL;

static dx_gl_wgl_context* g_context = NULL;

static void gl_wgl_close_window_internal(Core_Val_Gl_Wgl_Window* window) {
  CORE_UNREFERENCE(window);
  window = NULL;
}

static Core_Result gl_wgl_open_window_internal(Core_Val_Gl_Wgl_Window** window, Core_Result(*init_wgl)(Core_Val_Gl_Wgl_Window*)) {
  dx_gl_wgl_application* application = NULL;
  if (dx_application_get((dx_application**) & application)) {
    return Core_Failure;
  }
  Core_Val_Gl_Wgl_Window* window1 = NULL;
  if (Core_Val_Gl_Wgl_Window_create(&window1, application)) {
    CORE_UNREFERENCE(application);
    application = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(application);
  application = NULL;
  if (init_wgl) {
    if (init_wgl(window1)) {
      CORE_UNREFERENCE(window1);
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
  if (OpenHelperWindow()) {
    return Core_Failure;
  }
  Core_Val_Gl_Wgl_Window* window;
  if (gl_wgl_open_window_internal(&window, &gl_wgl_init_wgl)) {
    return Core_Failure;
  }
  CloseHelperWindow();
  if (!wglMakeCurrent(window->dc, window->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n") - 1);
    gl_wgl_close_window_internal(window);
    window = NULL;
    return Core_Failure;
  }
  g_window = window;
  return Core_Success;
}

static Core_Result gl_wgl_close_window() {
  gl_wgl_close_window_internal(g_window);
  g_window = NULL;
  CloseHelperWindow();
  return Core_Success;
}

static Core_Result getPixelFormatAttribI(int* RETURN, Core_Val_Gl_Wgl_Window* window, int attribute, Core_Size pixelFormatIndex);

static Core_Result getPixelFormatAttribF(float* RETURN, Core_Val_Gl_Wgl_Window* window, int attribute, Core_Size pixelFormatIndex);

static Core_Result getPixelFormat(Core_Val_Gl_Wgl_Window* window, Core_Size index, Core_Configuration* cfg);

static Core_Result getPixelFormatAttribI(int* RETURN, Core_Val_Gl_Wgl_Window* window, int attribute, Core_Size pixelFormatIndex) {
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribiv = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
  if (!wglGetPixelFormatAttribiv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n") - 1);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  int value;
  if (!wglGetPixelFormatAttribiv(window->dc, pixelFormatIndex, 0, 1, &attribute, &value)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = value;
  return Core_Success;
}

static Core_Result getPixelFormatAttribF(float* RETURN, Core_Val_Gl_Wgl_Window* window, int attribute, Core_Size pixelFormatIndex) {
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfv = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
  if (!wglGetPixelFormatAttribfv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n") - 1);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  float value;
  if (!wglGetPixelFormatAttribfv(window->dc, pixelFormatIndex, 0, 1, &attribute, &value)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = value;
  return Core_Success;
}

static Core_Result getPixelFormat(Core_Val_Gl_Wgl_Window* window, Core_Size index, Core_Configuration* cfg) {
  int i;
  // reject if pixel format does not support draw to window
  if (getPixelFormatAttribI(&i, window, WGL_DRAW_TO_WINDOW_ARB, index)) {
    return Core_Failure;
  }
  if (i != 1) {
    Core_setError(Core_Error_NotSupported);
    return Core_Failure;
  }
  // reject if pixel format does not support full accelaration
  if (getPixelFormatAttribI(&i, window, WGL_ACCELERATION_ARB, index)) {
    return Core_Failure;
  }
  if (i != WGL_FULL_ACCELERATION_ARB) {
    Core_setError(Core_Error_NotSupported);
    return Core_Failure;
  }
  // reject if pixel format does not support opengl
  if (getPixelFormatAttribI(&i, window, WGL_SUPPORT_OPENGL_ARB, index)) {
    return Core_Failure;
  }
  if (i != 1) {
    Core_setError(Core_Error_NotSupported);
    return Core_Failure;
  }
  // reject if pixel format does not support non-palettized
  if (getPixelFormatAttribI(&i, window, WGL_PIXEL_TYPE_ARB, index)) {
    return Core_Failure;
  }
  if (i != WGL_TYPE_RGBA_ARB) {
    Core_setError(Core_Error_NotSupported);
    return Core_Failure;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_ACCUM_RED_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.accumulationBuffer.redBits", sizeof("visuals.accumulationBuffer.redBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_ACCUM_GREEN_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.accumulationBuffer.greenBits", sizeof("visuals.accumulationBuffer.greenBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_ACCUM_BLUE_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.accumulationBuffer.blueBits", sizeof("visuals.accumulationBuffer.blueBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_ACCUM_ALPHA_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.accumulationBuffer.alphaBits", sizeof("visuals.accumulationBuffer.alphaBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_RED_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.colorBuffer.redBits", sizeof("visuals.colorBuffer.redBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_GREEN_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.colorBuffer.greenBits", sizeof("visuals.colorBuffer.greenBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_BLUE_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.colorBuffer.blueBits", sizeof("visuals.colorBuffer.blueBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_ALPHA_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.colorBuffer.alphaBits", sizeof("visuals.colorBuffer.alphaBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_STENCIL_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.stencilBuffer.stencilBits", sizeof("visuals.stencilBuffer.stencilBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  {
    if (getPixelFormatAttribI(&i, window, WGL_DEPTH_BITS_ARB, index)) {
      return Core_Failure;
    }
    if (i < Core_Natural8_Least || i > Core_Natural8_Greatest) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String* key;
    if (Core_String_create(&key, "visuals.depthBuffer.depthBits", sizeof("visuals.depthBuffer.depthBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_setN8(cfg, key, i)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
  }
  //
  return Core_Success;
}

static Core_Result getNumberOfPixelFormats(Core_Size* RETURN, Core_Val_Gl_Wgl_Window* window) {
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribiv = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
  if (!wglGetPixelFormatAttribiv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n", sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n") - 1);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  static const int attribute = WGL_NUMBER_PIXEL_FORMATS_ARB;
  int value = { 0.f };
  if (!wglGetPixelFormatAttribiv(window->dc, 0, 0, 1, &attribute, &value)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  if (value < Core_Size_Least || value > Core_Size_Greatest) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = value;
  return Core_Success;
}

static Core_Result dumpVisualsConfiguration(Core_Configuration* configuration) {
  {
    Core_Natural8 r, g, b, a;
    Core_String* key;
    //
    if (Core_String_create(&key, "visuals.colorBuffer.redBits", sizeof("visuals.colorBuffer.redBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&r, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.colorBuffer.greenBits", sizeof("visuals.colorBuffer.greenBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&g, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.colorBuffer.blueBits", sizeof("visuals.colorBuffer.blueBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&b, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.colorBuffer.alphaBits", sizeof("visuals.colorBuffer.alphaBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&a, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    fprintf(stdout, "  color buffer bit depths (r/g/b/a): %d/%d/%d/%d\n", r, g, b, a);
  }
  {
    Core_Natural8 r, g, b, a;
    Core_String* key;
    //
    if (Core_String_create(&key, "visuals.accumulationBuffer.redBits", sizeof("visuals.accumulationBuffer.redBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&r, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.accumulationBuffer.greenBits", sizeof("visuals.accumulationBuffer.greenBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&g, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.accumulationBuffer.blueBits", sizeof("visuals.accumulationBuffer.blueBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&b, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.accumulationBuffer.alphaBits", sizeof("visuals.accumulationBuffer.alphaBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&a, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    fprintf(stdout, "  accumulation buffer bit depths (r/g/b/a): %d/%d/%d/%d\n", r, g, b, a);
  }
  {
    Core_Natural8 s, d;
    Core_String* key;
    //
    if (Core_String_create(&key, "visuals.stencilBuffer.stencilBits", sizeof("visuals.stencilBuffer.stencilBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&s, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    if (Core_String_create(&key, "visuals.depthBuffer.depthBits", sizeof("visuals.depthBuffer.depthBits") - 1)) {
      return Core_Failure;
    }
    if (Core_Configuration_getN8(&d, configuration, key)) {
      CORE_UNREFERENCE(key);
      key = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(key);
    key = NULL;
    //
    fprintf(stdout, "  stencil buffer bit depths (s): %d\n", s);
    fprintf(stdout, "  depth buffer bit depths (d): %d\n", d);
  }
  return Core_Success;
}

static Core_Result enumeratePixelFormats(Core_Val_Gl_Wgl_Window* window) {
  Core_Size numberOfPixelFormats;
  if (getNumberOfPixelFormats(&numberOfPixelFormats, window)) {
    return Core_Failure;
  }
  Core_Configuration* configuration;
  if (Core_Configuration_create(&configuration)) {
    return Core_Failure;
  }
  for (int i = 1, j = 1, n = numberOfPixelFormats; i < n; ++i) {
    if (getPixelFormat(window, i, configuration)) {
      if (Core_Error_NotSupported != Core_getError()) {
        CORE_UNREFERENCE(configuration);
        configuration = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
        continue;
      }
    }
    fprintf(stdout, "format %d, %d\n", i, j);
    j++;
    {
      dumpVisualsConfiguration(configuration);
    }
  }
  return Core_Success;
}

static Core_Result gl_wgl_init_wgl(Core_Val_Gl_Wgl_Window* window) {
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("Core.Val.Gl.Wgl.System",
                      Core_Val_Gl_Wgl_System,
                      Core_Val_Gl_System);

/// @internal
/// @brief Determine the mouse button that was pressed/released given the msg, wparam, and lparam arguments of a WM_(L|M|R|X)BUTTON(DOWN|UP) message.
/// @param RETURN A pointer to a <code>Core_MouseButton</code> value.
/// @aram msg The msg value of the message.
/// @param wparam The wparam value of the message.
/// @param lparam The lparam value of the message.
/// @procedure
/// @error #Core_Error_NotFound if the button is not is not supported.
static Core_Result map_mouse_button(Core_MouseButton* RETURN, Core_Val_Gl_Wgl_System* SELF, UINT msg, WPARAM wparam, LPARAM lparam);

/// @internal
/// @brief Determine the keyboard key that was pressed/released given the msg, wparam, and lparam arguments of a WM_(KEY|SYSKEY)(DOWN|UP) message.
/// @param RETURN A pointer to a <code>Core_KeyboardKey_A</code> value.
/// @aram msg The msg value of the message.
/// @param wparam The wparam value of the message.
/// @param lparam The lparam value of the message.
/// @procedure
/// @error #Core_Error_NotFound if the key is not supported.
static Core_Result map_keyboard_key(Core_KeyboardKey* RETURN, Core_Val_Gl_Wgl_System* SELF, UINT msg, WPARAM wparam, LPARAM lparam);

static Core_Result get_modifiers(uint8_t* RETURN, Core_Val_Gl_Wgl_System* SELF);

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

static Core_Result startup(Core_Val_Gl_Wgl_System* SELF);

static Core_Result shutdown(Core_Val_Gl_Wgl_System* SELF);

static Core_Result get_context(dx_gl_wgl_context** RETURN, Core_Val_Gl_Wgl_System* SELF);

static Core_Result get_window(Core_Val_Gl_Wgl_Window** RETURN, Core_Val_Gl_Wgl_System* SELF);

static Core_Result map_mouse_button(Core_MouseButton* RETURN, Core_Val_Gl_Wgl_System* SELF, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_LBUTTONDOWN:
      *RETURN = Core_MouseButton_Button0;
      break;
    case WM_LBUTTONUP:
      *RETURN = Core_MouseButton_Button0;
      break;
    case WM_MBUTTONDOWN:
      *RETURN = Core_MouseButton_Button1;
      break;
    case WM_MBUTTONUP:
      *RETURN = Core_MouseButton_Button1;
      break;
    case WM_RBUTTONDOWN:
      *RETURN = Core_MouseButton_Button2;
      break;
    case WM_RBUTTONUP:
      *RETURN = Core_MouseButton_Button2;
      break;
    case WM_XBUTTONDOWN:
      *RETURN = Core_MouseButton_Button3;
      break;
    case WM_XBUTTONUP:
      *RETURN = Core_MouseButton_Button3;
      break;
    default: {
      Core_setError(Core_Error_NotFound);
      return Core_Failure;
    } break;
  };
  return Core_Success;
}

static Core_Result map_keyboard_key(Core_KeyboardKey* RETURN, Core_Val_Gl_Wgl_System* SELF, UINT msg, WPARAM wparam, LPARAM lparam) {
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

static Core_Result get_modifiers(uint8_t* RETURN, Core_Val_Gl_Wgl_System* SELF) {
  uint8_t modifiers = 0;
  Core_Boolean state;
  dx_keyboard_state* keyboard_state = DX_VAL_SYSTEM(SELF)->keyboard_state;
  //
  if (dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_LeftShift)) {
    return Core_Failure;
  }
  if (state) {
    modifiers |= Core_ModifierKeys_LeftShift;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_RightShift);
  if (state) {
    modifiers |= Core_ModifierKeys_RightShift;
  }
  //
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_LeftControl);
  if (state) {
    modifiers |= Core_ModifierKeys_LeftControl;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_RightControl);
  if (state) {
    modifiers |= Core_ModifierKeys_RightControl;
  }
  //
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_LeftMenu);
  if (state) {
    modifiers |= Core_ModifierKeys_LeftMenu;
  }
  dx_keyboard_state_get_state(&state, keyboard_state, Core_KeyboardKey_RightMenu);
  if (state) {
    modifiers |= Core_ModifierKeys_RightMenu;
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
      if (Core_Val_Gl_Wgl_System_onKeyboardKeyMessage(CORE_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        Core_setError(Core_Error_NoError); // Ignore the error.
        CORE_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      CORE_UNREFERENCE(app);
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
      if (Core_Val_Gl_Wgl_System_onMouseButtonMessage(CORE_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        Core_setError(Core_Error_NoError); // Ignore the error.
        CORE_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      CORE_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    // mouse pointer messages
    case WM_MOUSEMOVE: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (Core_Val_Gl_Wgl_System_onMousePointerMessage(CORE_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        Core_setError(Core_Error_NoError); // Ignore the error.
        CORE_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      CORE_UNREFERENCE(app);
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
      if (Core_Val_Gl_Wgl_System_onWindowMessage(CORE_VAL_GL_WGL_SYSTEM(app->val_system), wnd, msg, wparam, lparam)) {
        CORE_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      CORE_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    case WM_CLOSE: {
      dx_application* app = NULL;
      if (dx_application_get(&app)) {
        return 0;
      }
      if (dx_application_emit_quit_msg(DX_APPLICATION(app))) {
        CORE_UNREFERENCE(app);
        app = NULL;
        return 0;
      }
      CORE_UNREFERENCE(app);
      app = NULL;
      return 0;
    } break;
    default: {
      return DefWindowProc(wnd, msg, wparam, lparam);
    } break;
  };
}

static Core_Result startup(Core_Val_Gl_Wgl_System* SELF) {
  if (gl_wgl_open_window()) {
    return Core_Failure;
  }
  if (dx_gl_wgl_context_create(&g_context, g_window)) {
    gl_wgl_close_window();
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown(Core_Val_Gl_Wgl_System* SELF) {
  CORE_UNREFERENCE(g_context);
  g_context = NULL;
  gl_wgl_close_window();
  return Core_Success;
}

static Core_Result get_context(dx_gl_wgl_context** RETURN, Core_Val_Gl_Wgl_System* SELF) {
  if (!g_context) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  CORE_REFERENCE(g_context);
  *RETURN = g_context;
  return Core_Success;
}

static Core_Result getWindow(Core_Val_Gl_Wgl_Window** RETURN, Core_Val_Gl_Wgl_System* SELF) {
  if (!g_window) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  CORE_REFERENCE(g_window);
  *RETURN = g_window;
  return Core_Success;
}

static void Core_Val_Gl_Wgl_System_destruct(Core_Val_Gl_Wgl_System* SELF) {
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

static void Core_Val_Gl_Wgl_System_constructDispatch(Core_Val_Gl_Wgl_System_Dispatch* SELF) {
  DX_SYSTEM_DISPATCH(SELF)->startup = (Core_Result(*)(dx_system*)) & startup;
  DX_SYSTEM_DISPATCH(SELF)->shutdown = (Core_Result(*)(dx_system*)) & shutdown;
  DX_VAL_SYSTEM_DISPATCH(SELF)->get_context = (Core_Result(*)(dx_val_context**,dx_val_system*)) & get_context;
  CORE_VAL_GL_SYSTEM_DISPATCH(SELF)->getWindow = (Core_Result(*)(dx_val_gl_window**,Core_Val_Gl_System*)) & getWindow;
}

Core_Result Core_Val_Gl_Wgl_System_construct(Core_Val_Gl_Wgl_System* SELF, Core_MessageQueue* msg_queue) {
  Core_BeginConstructor(Core_Val_Gl_Wgl_System);
  if (Core_Val_Gl_System_construct(CORE_VAL_GL_SYSTEM(SELF), msg_queue)) {
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
  Core_EndConstructor(Core_Val_Gl_Wgl_System);
}

Core_Result Core_Val_Gl_Wgl_System_create(Core_Val_Gl_Wgl_System** RETURN, Core_MessageQueue* msg_queue) {
  DX_CREATE_PREFIX(Core_Val_Gl_Wgl_System);
  if (Core_Val_Gl_Wgl_System_construct(SELF, msg_queue)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Val_Gl_Wgl_System_onMouseButtonMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Core_Real32 x = (Core_Real32)(int)GET_X_LPARAM(lparam);
  Core_Real32 y = (Core_Real32)(int)GET_Y_LPARAM(lparam);
  Core_MouseButton mouse_button;
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

Core_Result Core_Val_Gl_Wgl_System_onMousePointerMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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

Core_Result Core_Val_Gl_Wgl_System_onKeyboardKeyMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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

Core_Result Core_Val_Gl_Wgl_System_onWindowMessage(Core_Val_Gl_Wgl_System* SELF, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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
