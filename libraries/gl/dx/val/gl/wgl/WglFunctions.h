#if !defined(CORE_VAL_GL_WGL_WGLFUNCTIONS_H_INCLUDED)
#define CORE_VAL_GL_WGL_WGLFUNCTIONS_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#include "dx/core.h"

typedef struct _WglFunctions {

  PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB;

  PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormatARB;
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC _wglGetPixelFormatAttribivARB;
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC _wglGetPixelFormatAttribfvARB;

  PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB;

  // Set of extension strings.
  Core_InlineHashMapPP extensions;

  // Target device context.
  HDC dc;

} _WglFunctions;
 
/// @param dc The target device context. Must remain valid until the functions are uninitialized.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @error #Core_Error_Initialized the functions are initialized
/// @error #Core_Error_ArgumentInvalid dc is null
Core_Result _WglFunctions_startup(HDC dc);

/// @return #Core_Success on success. #Core_Failure on failure.
/// @error #Core_Error_NotInitialized the functions are not initialized
Core_Result _WglFunctions_shutdown();

/// @return #Core_Success on success. #Core_Failure on failure.
/// @error #Core_Error_NotInitialized the functions are not initialized
/// @error #Core_Error_ArgumentInvalid RETURN is null
Core_Result _WglFunctions_getInstance(_WglFunctions** RETURN);

#endif // CORE_VAL_GL_WGL_WGLFUNCTIONS_H_INCLUDED
