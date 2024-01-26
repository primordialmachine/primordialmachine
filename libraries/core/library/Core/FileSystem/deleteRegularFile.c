#include "Core/FileSystem/deleteRegularFile.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static Core_Result deleteRegularFile(char const* path) {
  BOOL result = DeleteFile(path);
  if (!result) {
    DWORD error = GetLastError();
    if (ERROR_ACCESS_DENIED == error || ERROR_FILE_NOT_FOUND == error) {
      Core_setError(Core_Error_NotFound);
      return Core_Failure;
    }
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_deleteRegularFile(Core_String* path) {
  if (!path) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
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
  if (deleteRegularFile(path->bytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  return Core_Success;
}