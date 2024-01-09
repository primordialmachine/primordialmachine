#include "dx/core/file_system.h"

#include "dx/core/inline_byte_array.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineEnumerationType("Core.FileState",
                           Core_FileState);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_get_file_contents(char const *path, char **bytes, Core_Size*number_of_bytes) {
  Core_InlineArrayN8 byte_array;
  if (Core_InlineArrayN8_initialize(&byte_array)) {
    return Core_Failure;
  }
  HANDLE file = CreateFile(path,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           0,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           0);
  if (INVALID_HANDLE_VALUE == file) {
    Core_InlineArrayN8_uninitialize(&byte_array);
    return Core_Failure;
  }
  while (true) {
    char temporary[1024]; DWORD requested = 1024;
    DWORD received = 0;
    BOOL result = ReadFile(file, temporary, requested, &received, NULL);
    if (!result) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      Core_InlineArrayN8_uninitialize(&byte_array);
      return Core_Failure;
    }
    // eof
    if (received == 0) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      break;
    }
    if (Core_InlineArrayN8_append(&byte_array, temporary, received)) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      Core_InlineArrayN8_uninitialize(&byte_array);
      return Core_Failure;
    }
  }
  
  char *bytes_1; Core_Size number_of_bytes_1;
  if (Core_InlineArrayN8_steal(&byte_array, &bytes_1, &number_of_bytes_1)) {
    Core_InlineArrayN8_uninitialize(&byte_array);
    return Core_Failure;
  }
  Core_InlineArrayN8_uninitialize(&byte_array);
  *bytes = bytes_1;
  *number_of_bytes = number_of_bytes_1;
  
  return Core_Success;
}

Core_Result dx_get_file_contents_s(Core_String* path, char** bytes, Core_Size* number_of_bytes) {
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
  if (dx_get_file_contents(path->bytes, bytes, number_of_bytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_get_file_state(Core_FileState* RETURN, char const* path) {
  DWORD t = GetFileAttributesA(path);
  if (t == INVALID_FILE_ATTRIBUTES) {
    t = GetLastError();
    SetLastError(0);
    if (t == ERROR_FILE_NOT_FOUND ||
        t == ERROR_PATH_NOT_FOUND ||
        t == ERROR_INVALID_NAME ||
        t == ERROR_INVALID_DRIVE ||
        t == ERROR_NOT_READY ||
        t == ERROR_INVALID_PARAMETER ||
        t == ERROR_BAD_PATHNAME ||
        t == ERROR_BAD_NETPATH)
    {
      *RETURN = Core_FileState_NotFound;
      return Core_Success;
    } else {
      // The case was not properly handled.
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
  } else {
    if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & t)) {
      *RETURN = Core_FileState_IsDirectory;
    } else {
      *RETURN = Core_FileState_IsRegular;
    }
    return Core_Success;
  }
}

Core_Result dx_get_file_state_s(Core_FileState* RETURN, Core_String* path) {
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
  Core_FileState file_state;
  if (dx_get_file_state(&file_state, path->bytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  *RETURN = file_state;
  return Core_Success;
}
