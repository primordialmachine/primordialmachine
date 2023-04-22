#include "dx/gl/wgl/wm.h"

#include "dx/core.h"
#include "dx/gl/wgl/context.h"
#include "dx/gl/wgl/window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "opengl32.lib")

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on Nvidia Optimus systems
// with up-to-date drivers
__declspec(dllexport) DWORD NvOptimusEnablement = 1;

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on AMD PowerXpress systems
// with up-to-date drivers
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_gl_wgl_init_wgl_v1(dx_gl_wgl_window* window, dx_gl_wgl_window*);

static dx_result dx_gl_wgl_init_wgl_v2(dx_gl_wgl_window* window, dx_gl_wgl_window*);

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing,
                                          dx_result (*init_wgl)(dx_gl_wgl_window*, dx_gl_wgl_window*));

static void dx_gl_wgl_close_window_internal(dx_gl_wgl_window* window);

dx_result dx_gl_wgl_open_window();

dx_result dx_gl_wgl_close_window();

static dx_gl_wgl_window* g_window = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_gl_wgl_close_window_internal(dx_gl_wgl_window* window) {
  DX_UNREFERENCE(window);
  window = NULL;
}

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing, dx_result (*init_wgl)(dx_gl_wgl_window*, dx_gl_wgl_window*)) {
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  if (!application) {
    return DX_FAILURE;
  }
  dx_gl_wgl_window* window1 = NULL;
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

dx_result dx_gl_wgl_open_window() {
  dx_gl_wgl_window* window_v1;
  if (dx_gl_wgl_open_window_internal(&window_v1, NULL, &dx_gl_wgl_init_wgl_v1)) {
    return DX_FAILURE;
  }
  dx_gl_wgl_window* window_v2;
  if (dx_gl_wgl_open_window_internal(&window_v2, window_v1, &dx_gl_wgl_init_wgl_v2)) {
    return DX_FAILURE;
  }

  dx_gl_wgl_close_window_internal(window_v1);
  
  if (!wglMakeCurrent(window_v2->dc, window_v2->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n"));
    dx_gl_wgl_close_window_internal(window_v2);
    window_v2 = NULL;
    return DX_FAILURE;
  }

  g_window = window_v2;

  return DX_SUCCESS;
}

dx_result dx_gl_wgl_close_window() {
  dx_gl_wgl_close_window_internal(g_window);
  g_window = NULL;
  return DX_SUCCESS;
}

static dx_result dx_gl_wgl_init_wgl_v2(dx_gl_wgl_window* window, dx_gl_wgl_window* existing) {
  if (!wglMakeCurrent(existing->dc, existing->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n"));
    return DX_FAILURE;
  }
  // (1) Acquire pointers to essential OpenGL functions.
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
  if (!wglChoosePixelFormat) {
    dx_log("unable to acquire a pointer to the wglChoosePixelFormat function\n",
           sizeof("unable to acquire a pointer to the wglChoosePixelFormat function\n"));
    return DX_FAILURE;
  }
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  if (!wglCreateContextAttribs) {
    dx_log("unable to acquire a pointer to the wglCreateContextAttribs function\n",
           sizeof("unable to acquire a pointer to the wglCreateContextAttribs function\n"));
    return DX_FAILURE;
  }
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribiv = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
  if (!wglGetPixelFormatAttribiv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n",
           sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribiv function\n"));
    return DX_FAILURE;
  }
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfv = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
  if (!wglGetPixelFormatAttribfv) {
    dx_log("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n",
           sizeof("unable to acquire a pointer to the wglGetPixelFormatAttribfv function\n"));
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
      dx_log("unable to choose pixel format\n", sizeof("unable to choose pixel format\n"));
      return DX_FAILURE;
    }
    PIXELFORMATDESCRIPTOR desc;
    if (!DescribePixelFormat(window->dc, i, sizeof(desc), &desc)) {
      dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n"));
      return DX_FAILURE;
    }
    if (!SetPixelFormat(window->dc, i, &desc)) {
      dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n"));
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
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n"));
    return DX_FAILURE;
  }

  return DX_SUCCESS;
}

static dx_result dx_gl_wgl_init_wgl_v1(dx_gl_wgl_window* window, dx_gl_wgl_window* existing) {
  PIXELFORMATDESCRIPTOR desc = {
    .nSize = sizeof(desc),
    .nVersion = 1,
    .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 24,
  };
  int format = ChoosePixelFormat(window->dc, &desc);
  if (!format) {
    dx_log("unable to choose pixel format\n", sizeof("unable to choose pixel format\n"));
    return 1;
  }
  if (!DescribePixelFormat(window->dc, format, sizeof(desc), &desc)) {
    dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n"));
    return 1;
  }
  if (!SetPixelFormat(window->dc, format, &desc)) {
    dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n"));
    return 1;
  }
  window->glrc = wglCreateContext(window->dc);
  if (!window->glrc) {
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n"));
    return 1;
  }
  return 0;
}

static dx_gl_wgl_context* g_context = NULL;

dx_result dx_val_gl_wgl_startup() {
  if (dx_gl_wgl_open_window()) {
    return DX_FAILURE;
  }
  if (dx_gl_wgl_context_create(&g_context, g_window)) {
    dx_gl_wgl_close_window();
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_shutdown() {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  dx_gl_wgl_close_window();
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_get_context(dx_gl_wgl_context** RETURN) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_get_window(dx_gl_wgl_window** RETURN) {
  if (!g_window) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_window);
  *RETURN = g_window;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
