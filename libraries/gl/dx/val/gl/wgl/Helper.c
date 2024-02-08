#include "dx/val/gl/wgl/Helper.h"

#include <malloc.h>
#include <stdio.h>

static HINSTANCE g_instanceHandle = NULL;
static char *g_className = NULL;
static ATOM g_classHandle = 0;
static HWND g_windowHandle = NULL;
static HDC g_deviceContext = NULL;
static int g_pixelFormat = 0;
static HGLRC g_openglContext = NULL;

static LRESULT CALLBACK windowProcedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(wnd, msg, wparam, lparam);
}
static Core_Result StartupOpenGl() {
  PIXELFORMATDESCRIPTOR descriptor;
  memset(&descriptor, 0, sizeof(descriptor));
  descriptor.nSize = sizeof(descriptor);
  descriptor.nVersion = 1;
  descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  descriptor.iPixelType = PFD_TYPE_RGBA;
  descriptor.cColorBits = 24;
  g_pixelFormat = ChoosePixelFormat(g_deviceContext, &descriptor);
  if (!g_pixelFormat) {
    fprintf(stderr, "%s:%d: unable to choose pixel format\n", __FILE__, __LINE__);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  if (!DescribePixelFormat(g_deviceContext, g_pixelFormat, sizeof(descriptor), &descriptor)) {
    fprintf(stderr, "%s:%d: unable to describe pixel format\n", __FILE__, __LINE__);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  if (!SetPixelFormat(g_deviceContext, g_pixelFormat, &descriptor)) {
    fprintf(stderr, "%s:%d: unable to set pixel format\n", __FILE__, __LINE__);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_openglContext = wglCreateContext(g_deviceContext);
  if (!g_openglContext) {
    fprintf(stderr, "%s:%d: unable to create wgl context\n", __FILE__, __LINE__);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  if (!wglMakeCurrent(g_deviceContext, g_openglContext)) {
    wglDeleteContext(g_openglContext);
    g_openglContext = NULL;
    fprintf(stderr, "%s:%d: unable to make wgl context current\n", __FILE__, __LINE__);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result ShutdownOpenGl() {
  if (g_openglContext) {
    wglDeleteContext(g_openglContext);
    g_openglContext = NULL;
  }
  g_pixelFormat = 0;
  return Core_Success;
}

Core_Result OpenHelperWindow() {
  // (1)
  g_instanceHandle = GetModuleHandle(NULL);
  if (!g_instanceHandle) {
    fprintf(stderr, "%s:%d: unable to acquire instance handle\n", __FILE__, __LINE__);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  // (2)
  g_className = _strdup("Core.Gl.Wgl.HelperWindowClass");
  if (!g_className) {
    Core_setError(Core_Error_AllocationFailed);
    fprintf(stderr, "%s:%d: unable to create window class name\n", __FILE__, __LINE__);
    CloseHelperWindow();
    return Core_Failure;
  }
  //
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
  //
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wcex.lpfnWndProc = &windowProcedure;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = g_instanceHandle;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hIconSm = NULL;
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = g_className;
  g_classHandle = RegisterClassEx(&wcex);
  if (!g_classHandle) {
    Core_setError(Core_Error_EnvironmentFailed);
    fprintf(stderr, "%s:%d: unable to register window class\n", __FILE__, __LINE__);
    CloseHelperWindow();
    return Core_Failure;
  }
  g_windowHandle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                                  g_className,
                                  "Primordial Machine Helper Window",
                                  WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  NULL,
                                  NULL,
                                  g_instanceHandle,
                                  NULL);
  if (!g_windowHandle) {
    Core_setError(Core_Error_EnvironmentFailed);
    fprintf(stderr, "%s:%d: unable to create window\n", __FILE__, __LINE__);
    CloseHelperWindow();
    return Core_Failure;
  }
  g_deviceContext = GetDC(g_windowHandle);
  if (!g_deviceContext) {
    Core_setError(Core_Error_EnvironmentFailed);
    fprintf(stderr, "%s:%d: unable to acquire device context\n", __FILE__, __LINE__);
    CloseHelperWindow();
    return Core_Failure;
  }
  if (StartupOpenGl()) {
    CloseHelperWindow();
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result CloseHelperWindow() {
  ShutdownOpenGl();
  g_pixelFormat = 0;
  if (g_deviceContext) {
    ReleaseDC(g_windowHandle, g_deviceContext);
    g_deviceContext = NULL;
  }
  if (g_windowHandle) {
    DestroyWindow(g_windowHandle);
    g_windowHandle = NULL;
  }
  if (g_classHandle) {
    if (!UnregisterClass(g_className, g_instanceHandle)) {
      fprintf(stderr, "%s:%d: unable to unregister window class\n", __FILE__, __LINE__);
    }
    g_classHandle = 0;
  }
  if (g_className) {
    free(g_className);
    g_className = NULL;
  }
  g_instanceHandle = 0;
  return Core_Success;
}

Core_Result GetHelperWindowHandle(HWND* wnd) {
  *wnd = g_windowHandle;
  return Core_Success;
}

Core_Result GetHelperDeviceContext(HDC* dc) {
  *dc = g_deviceContext;
  return Core_Success;
}
