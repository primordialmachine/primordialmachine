#if !defined(CORE_VAL_GL_WGL_HELPER_H_INCLUDED)
#define CORE_VAL_GL_WGL_HELPER_H_INCLUDED

#include "dx/core.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/// Open the helper window.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result OpenHelperWindow();

/// Close the helper window.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result CloseHelperWindow();

/// Get the helper window's window handle.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result GetHelperWindowHandle(HWND* wnd);

/// Get the helper window's device context.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result GetHelperDeviceContext(HDC* dc);

#endif // CORE_VAL_GL_WGL_HELPER_H_INCLUDED
