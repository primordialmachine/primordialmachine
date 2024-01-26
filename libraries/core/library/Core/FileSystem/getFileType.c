// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/getFileType.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static Core_Result _getFileType(Core_FileType* RETURN, char const* path);

static Core_Result _getFileType(Core_FileType* RETURN, char const* path) {
  DWORD t = GetFileAttributesA(path);
  if (t == INVALID_FILE_ATTRIBUTES) {
    t = GetLastError();
    SetLastError(0);
    if (t == ERROR_FILE_NOT_FOUND || t == ERROR_PATH_NOT_FOUND) {
      Core_setError(Core_Error_NotFound);
      return Core_Failure;
    } else if (t == ERROR_INVALID_NAME ||
               t == ERROR_INVALID_DRIVE ||
               t == ERROR_NOT_READY ||
               t == ERROR_INVALID_PARAMETER ||
               t == ERROR_BAD_PATHNAME ||
               t == ERROR_BAD_NETPATH) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } else {
      // The case was not properly handled.
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
  } else {
    if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & t)) {
      *RETURN = Core_FileType_Directory;
    } else {
      *RETURN = Core_FileType_Regular;
    }
    return Core_Success;
  }
}

Core_Result Core_getFileType(Core_FileType* RETURN, Core_String* path) {
  //
  Core_Boolean containsSymbol;
  if (Core_String_containsSymbol(&containsSymbol, path, '\0')) {
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
  if (Core_String_printf(&path, format, path)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  //
  Core_FileType file_state;
  if (_getFileType(&file_state, path->bytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  *RETURN = file_state;
  return Core_Success;
}
