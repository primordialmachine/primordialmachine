#if !defined(DX_VAL_GL_H_INCLUDED)
#define DX_VAL_GL_H_INCLUDED

#include "dx/core.h"

#if DX_OPERATING_SYSTEM_WINDOWS == DX_OPERATING_SYSTEM
  #include "dx/val/gl/wgl/system_factory.h"
#else
  #error("environment not (yet) supported")
#endif

#endif // DX_VAL_GL_H_INCLUDED
