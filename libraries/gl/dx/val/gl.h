#if !defined(DX_VAL_GL_H_INCLUDED)
#define DX_VAL_GL_H_INCLUDED

#include "dx/core.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem
  #include "dx/val/gl/wgl/system_factory.h"
#else
  #error("environment not (yet) supported")
#endif

#endif // DX_VAL_GL_H_INCLUDED
