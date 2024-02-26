// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Time.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Core_Result Core_getNow(Core_Natural64* RETURN) {
  static_assert(Core_Natural64_Greatest >= MAXULONGLONG, "Core_Natural64_Greatest smaller than MAXULONGLONG");
  ULONGLONG v = GetTickCount64();
  *RETURN = v;
  return Core_Success;
}

#elif Core_OperatingSystem_Linux == Core_OperatingSystem

#include <unistd.h>
#include <time.h>

Core_Result Core_getNow(Core_Natural64* RETURN) {
  struct timespec ts;
  if(0 != clock_gettime(CLOCK_REALTIME, &ts)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  Core_Natural64 t = ts.tv_nsec / 1000000;
  t += ts.tv_sec * 1000;
  *RETURN = t;
  return Core_Success;
}

#else
  #error("environment not (yet) supported")
#endif

