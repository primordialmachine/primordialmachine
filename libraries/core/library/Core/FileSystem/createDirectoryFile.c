// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/deleteDirectoryFile.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static Core_Result createDirectoryFile(char const* pathName) {
  BOOL result = CreateDirectory(pathName, NULL);
  if (!result) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_createDirectoryFile(Core_String* pathName) {
  if (!pathName) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  //
  Core_Boolean containsSymbol;
  if (Core_String_containsSymbol(&containsSymbol, pathName, '\0')) {
    return Core_Failure;
  }
  if (containsSymbol) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_String* format = NULL;
  if (Core_String_create(&format, "${s}\0", sizeof("${s}\0") - 1)) {
    return Core_Failure;
  }
  if (Core_String_printf(&pathName, format, pathName)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  //
  if (createDirectoryFile(pathName->bytes)) {
    CORE_UNREFERENCE(pathName);
    pathName = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(pathName);
  pathName = NULL;
  return Core_Success;
}
