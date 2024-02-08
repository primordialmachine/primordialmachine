#if !defined(CORE_VAL_GL_FUNCTIONS_H_INCLUDED)
#define CORE_VAL_GL_FUNCTIONS_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "dx/core.h"

typedef void (APIENTRY* PFNGLGETINTEGERVPROC)(GLenum, GLint*);

typedef const GLubyte* (APIENTRY* PFNGLGETSTRINIGPROC)(GLenum, GLint);

typedef struct _GlFunctions {

  PFNGLGETINTEGERVPROC _glGetIntegerv;
  PFNGLGETSTRINIGPROC _glGetStringi;

  // Set of extension strings.
  Core_InlineHashMapPP extensions;

} _GlFunctions;

/// @return #Core_Success on success. #Core_Failure on failure.
/// @error #Core_Error_Initialized the functions are initialized
/// @error #Core_Error_ArgumentInvalid dc is null
Core_Result _GlFunctions_startup();

/// @return #Core_Success on success. #Core_Failure on failure.
/// @error #Core_Error_NotInitialized the functions are not initialized
Core_Result _GlFunctions_shutdown();

/// @return #Core_Success on success. #Core_Failure on failure.
/// @error #Core_Error_NotInitialized the functions are not initialized
/// @error #Core_Error_ArgumentInvalid RETURN is null
Core_Result _GlFunctions_getInstance(_GlFunctions** RETURN);

#endif // CORE_VAL_GL_FUNCTIONS_H_INCLUDED
