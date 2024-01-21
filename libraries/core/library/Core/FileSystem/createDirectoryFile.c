#include "Core/FileSystem/deleteDirectoryFile.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static Core_Result createDirectoryFile(char const* path) {
  BOOL result = CreateDirectory(path, NULL);
  if (!result) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_createDirectoryFile(Core_String* path) {
  if (!path) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  //
  Core_Boolean containsSymbol;
  containsSymbol = dx_string_contains_symbol(path, '\0');
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
  if (createDirectoryFile(path->bytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  return Core_Success;
}
