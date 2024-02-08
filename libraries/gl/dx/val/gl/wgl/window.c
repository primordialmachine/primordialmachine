#include "dx/val/gl/wgl/window.h"

#include "dx/application.h"
#include "dx/val/gl/wgl/system.h"

#include "dx/val/gl/GlFunctions.h"
#include "dx/val/gl/wgl/WglFunctions.h"

#include <stdio.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static PFNGLGETINTEGERVPROC g_glGetIntegerv = NULL;

static PFNGLGETSTRINGIPROC g_glGetStringi = NULL;

static Core_Result link(void** RETURN, char const* name);

static Core_Result uninitOpenGl(Core_Val_Gl_Wgl_Window* window);

static Core_Result initOpenGl(Core_Val_Gl_Wgl_Window* window);

// Create an icon. The icon is filed with the specified color and is of the specified width and height.
static Core_Result createIcon(HICON* RETURN, COLORREF color, int width, int height);

static Core_Result createIcons(HWND wnd, HICON* bigIcon, HICON* smallIcon, COLORREF color);

static Core_Result link(void** RETURN, char const* name) {
  void* p = (void*)wglGetProcAddress(name);
  if (!p) {
    // This technique is required for OpenGL 1.0 core functions.
    HMODULE module = LoadLibraryA("opengl32.dll");
    if (!module) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    p = (void*)GetProcAddress(module, name);
  }
  if (!p) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = p;
  return Core_Success;
}

static _WglFunctions* g_wglFunctions = NULL;

static _GlFunctions* g_glFunctions = NULL;

static Core_Result uninitOpenGl(Core_Val_Gl_Wgl_Window* window) {
  _GlFunctions_shutdown();
  g_glFunctions = NULL;
  _WglFunctions_shutdown();
  g_wglFunctions = NULL;
  if (window->glrc) {
    wglDeleteContext(window->glrc);
    window->glrc = NULL;
  }
  return Core_Success;
}

static Core_Result initOpenGl(Core_Val_Gl_Wgl_Window* window) {
  // (1) Startup WGL functions.
  if (_WglFunctions_startup(window->dc)) {
    return Core_Failure;
  }
  if (_WglFunctions_getInstance(&g_wglFunctions)) {
    _WglFunctions_shutdown();
    return Core_Failure;
  }
  // (2) Set the pixel format.
  const int samples = 0;
  const int pixelFormatAttribs[] = {
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
    if (!g_wglFunctions->_wglChoosePixelFormatARB(window->dc, pixelFormatAttribs, 0, 1, &i, &n)) {
      fprintf(stderr, "%s:%d: unable to choose pixel format\n", __FILE__, __LINE__);
      _WglFunctions_shutdown();
      g_wglFunctions = NULL;
      return Core_Failure;
    }
    PIXELFORMATDESCRIPTOR desc;
    if (!DescribePixelFormat(window->dc, i, sizeof(desc), &desc)) {
      fprintf(stderr, "%s:%d: unable to describe pixel format\n", __FILE__, __LINE__);
      _WglFunctions_shutdown();
      g_wglFunctions = NULL;
      return Core_Failure;
    }
    if (!SetPixelFormat(window->dc, i, &desc)) {
      fprintf(stderr, "%s:%d: unable to set pixel format\n", __FILE__, __LINE__);
      _WglFunctions_shutdown();
      g_wglFunctions = NULL;
      return Core_Failure;
    }
  }
  // (3) Create the context.
  const int contextAttribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 1,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  window->glrc = g_wglFunctions->_wglCreateContextAttribsARB(window->dc, NULL, contextAttribs);
  if (!window->glrc) {
    fprintf(stderr, "%s:%d: unable to create wgl context\n", __FILE__, __LINE__);
    _WglFunctions_shutdown();
    g_wglFunctions = NULL;
    return Core_Failure;
  }
  // (4) Startup GL functions.
  if (_GlFunctions_startup()) {
    _WglFunctions_shutdown();
    g_wglFunctions = NULL;
    return Core_Failure;
  }

  return Core_Success;
}

static Core_Result createIcon(HICON *RETURN, COLORREF color, int width, int height) {
  // Obtain a handle to the screen device context.
  HDC hdcScreen = GetDC(NULL);
  if (!hdcScreen) {
    return Core_Failure;
  }

  // Create a memory device context, which we will draw into.
  HDC hdcMem = CreateCompatibleDC(hdcScreen);
  if (!hdcMem) {
    ReleaseDC(NULL, hdcScreen);
    hdcScreen = NULL;
    return Core_Failure;
  }

  // Create the bitmap, and select it into the device context for drawing.
  HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
  if (!hbmp) {
    DeleteDC(hdcMem);
    hdcMem = NULL;
    ReleaseDC(NULL, hdcScreen);
    hdcScreen = NULL;
    return Core_Failure;
  }
  HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);

  // Draw your icon.
  // 
  // For this simple example, we're just drawing a solid color rectangle
  // in the specified color with the specified dimensions.
  HPEN hpen = CreatePen(PS_SOLID, 1, color);
  HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen);
  HBRUSH hbrush = CreateSolidBrush(color);
  HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush);
  Rectangle(hdcMem, 0, 0, width, height);
  SelectObject(hdcMem, hbrushOld);
  SelectObject(hdcMem, hpenOld);
  DeleteObject(hbrush);
  DeleteObject(hpen);

  // Create an icon from the bitmap.
  // 
  // Icons require masks to indicate transparent and opaque areas. Since this
  // simple example has no transparent areas, we use a fully opaque mask.
  HBITMAP hbmpMask = CreateCompatibleBitmap(hdcScreen, width, height);
  ICONINFO ii;
  ii.fIcon = TRUE;
  ii.hbmMask = hbmpMask;
  ii.hbmColor = hbmp;
  HICON hIcon = CreateIconIndirect(&ii);
  DeleteObject(hbmpMask);

  // Clean-up.
  SelectObject(hdcMem, hbmpOld);
  DeleteObject(hbmp);
  DeleteDC(hdcMem);
  ReleaseDC(NULL, hdcScreen);

  *RETURN = hIcon;

  return Core_Success;
}

static Core_Result createIcons(HWND wnd, HICON* bigIcon, HICON* smallIcon, COLORREF color) {
  int size;
  HICON _bigIcon = NULL,
        _smallIcon = NULL;

  // create big icon
  size = GetSystemMetrics(SM_CXICON);
  if (createIcon(&_bigIcon, color, size, size)) {
    return Core_Failure;
  }

  // create and assign small icon
  size = GetSystemMetrics(SM_CXSMICON);
  if (createIcon(&_smallIcon, color, size, size)) {
    DestroyIcon(_bigIcon);
    _bigIcon = NULL;
    return Core_Failure;
  }

  *bigIcon = _bigIcon;
  *smallIcon = _smallIcon;

  return Core_Success;

}

Core_defineObjectType("dx.val.gl.wgl.window",
                      Core_Val_Gl_Wgl_Window,
                      dx_val_gl_window);

static Core_Result getCanvasSize(Core_Val_Gl_Wgl_Window* SELF, Core_Integer32* width, Core_Integer32* height);

static void Core_Val_Gl_Wgl_Window_destruct(Core_Val_Gl_Wgl_Window* SELF) {
  uninitOpenGl(SELF);
  if (SELF->dc) {
    ReleaseDC(SELF->wnd, SELF->dc);
    SELF->dc = NULL;
  }
  if (SELF->wnd) {
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
  }
  if (SELF->smallIcon) {
    DestroyIcon(SELF->smallIcon);
    SELF->smallIcon = NULL;
  }
  if (SELF->bigIcon) {
    DestroyIcon(SELF->bigIcon);
    SELF->bigIcon = NULL;
  }
  DX_DEBUG_ASSERT(NULL != SELF->application);
  CORE_UNREFERENCE(SELF->application);
  SELF->application = NULL;
}

static void Core_Val_Gl_Wgl_Window_constructDispatch(Core_Val_Gl_Wgl_Window_Dispatch* SELF) {
  DX_VAL_GL_WINDOW_DISPATCH(SELF)->getCanvasSize = (Core_Result(*)(dx_val_gl_window*,Core_Integer32*,Core_Integer32*)) & getCanvasSize;
}

Core_Result Core_Val_Gl_Wgl_Window_construct(Core_Val_Gl_Wgl_Window* SELF, dx_gl_wgl_application* application) {
  Core_BeginConstructor(Core_Val_Gl_Wgl_Window);
  if (!application) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (dx_val_gl_window_construct(DX_VAL_GL_WINDOW(SELF))) {
    return Core_Failure;
  }
  SELF->application = application;
  CORE_REFERENCE(application);
  SELF->wnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                             CORE_VAL_GL_WGL_SYSTEM(DX_APPLICATION(application)->val_system)->class_name,
                             "Media Evaluation Framework",
                             WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             NULL,
                             NULL,
                             CORE_VAL_GL_WGL_SYSTEM(DX_APPLICATION(application)->val_system)->instance_handle,
                             NULL);
  if (!SELF->wnd) {
    CORE_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create window\n", sizeof("unable to create window\n") - 1);
    return Core_Failure;
  }
  SELF->dc = GetDC(SELF->wnd);
  if (!SELF->dc) {
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
    CORE_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n") - 1);
    return Core_Failure;
  }
  COLORREF color = RGB(0, 0, 0);
  if (createIcons(SELF->wnd, &SELF->bigIcon, &SELF->smallIcon, color)) {
    ReleaseDC(SELF->wnd, SELF->dc);
    SELF->dc = NULL;
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
    CORE_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create icons\n", sizeof("unable to create icons\n") - 1);
    return Core_Failure;
  }
  SendMessage(SELF->wnd, WM_SETICON, ICON_BIG, (LPARAM)SELF->bigIcon);
  SendMessage(SELF->wnd, WM_SETICON, ICON_SMALL, (LPARAM)SELF->smallIcon);

  if (initOpenGl(SELF)) {
    ReleaseDC(SELF->wnd, SELF->dc);
    SELF->dc = NULL;
    DestroyWindow(SELF->wnd);
    SELF->wnd = NULL;
    CORE_UNREFERENCE(application);
    application = NULL;
    return Core_Failure;
  }
  Core_EndConstructor(Core_Val_Gl_Wgl_Window);
}

Core_Result Core_Val_Gl_Wgl_Window_create(Core_Val_Gl_Wgl_Window** RETURN, dx_gl_wgl_application* application) {
  DX_CREATE_PREFIX(Core_Val_Gl_Wgl_Window);
  if (Core_Val_Gl_Wgl_Window_construct(SELF, application)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result getCanvasSize(Core_Val_Gl_Wgl_Window* SELF, Core_Integer32* width, Core_Integer32* height) {
  if (!SELF || !width || !height) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  RECT rect;
  GetClientRect(SELF->wnd, &rect);
  *width = rect.right - rect.left;
  *height = rect.bottom - rect.top;
  return Core_Success;
}
