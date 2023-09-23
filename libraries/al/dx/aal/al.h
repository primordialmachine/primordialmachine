#if !defined(DX_AAL_AL_H_INCLUDED)
#define DX_AAL_AL_H_INCLUDED

#include "dx/core.h"
#include "dx/application.h"

#if !defined(DX_VISUALS)
  #error("DX_VISUALS is not defined")
#endif

#if !defined(DX_OPERATING_SYSTEM)
  #error("DX_OPERATING_SYSTEM is not defined")
#endif

#if DX_AUDIALS_OPENAL == DX_AUDIALS
  /*Intentionally empty.*/
#else
  #error("environment not (yet) supported")
#endif

#endif // DX_AAL_AL_H_INCLUDED
