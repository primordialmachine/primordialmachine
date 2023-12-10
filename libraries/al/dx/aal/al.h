#if !defined(DX_AAL_AL_H_INCLUDED)
#define DX_AAL_AL_H_INCLUDED

#include "dx/core.h"
#include "dx/application.h"

#if !defined(Core_AudialsBackend)
  #error("Core_AudialsBackend is not defined")
#endif

#if !defined(Core_OperatingSystem)
  #error("Core_OperatingSystem is not defined")
#endif

#if Core_AudialsBackend_OpenAl == Core_AudialsBackend
  /*Intentionally empty.*/
#else
  #error("environment not (yet) supported")
#endif

#endif // DX_AAL_AL_H_INCLUDED
