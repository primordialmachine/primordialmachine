// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#include "Core/Time.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem_Windows

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Core_Result Core_getNow(Core_Natural64* RETURN) {
  static_assert(Core_Natural64_Greatest >= MAXULONGLONG, "Core_Natural64_Greatest smaller than MAXULONGLONG");
  ULONGLONG v = GetTickCount64();
  *RETURN = v;
  return Core_Success;
}

#else
  #error("environment not (yet) supported")
#endif