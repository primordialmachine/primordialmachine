#include "dx/core/file_system.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/byte_array.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result dx_get_file_contents(char const *path, char **bytes, dx_size*number_of_bytes) {
  dx_inline_byte_array byte_array;
  if (dx_inline_byte_array_initialize(&byte_array)) {
    return DX_FAILURE;
  }
  HANDLE file = CreateFile(path,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           0,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           0);
  if (INVALID_HANDLE_VALUE == file) {
    dx_inline_byte_array_uninitialize(&byte_array);
    return DX_FAILURE;
  }
  while (true) {
    char temporary[1024]; DWORD requested = 1024;
    DWORD received = 0;
    BOOL result = ReadFile(file, temporary, requested, &received, NULL);
    if (!result) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      dx_inline_byte_array_uninitialize(&byte_array);
      return DX_FAILURE;
    }
    // eof
    if (received == 0) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      break;
    }
    if (dx_inline_byte_array_append(&byte_array, temporary, received)) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      dx_inline_byte_array_uninitialize(&byte_array);
      return DX_FAILURE;
    }
  }
  
  char *bytes_1; dx_size number_of_bytes_1;
  if (dx_inline_byte_array_steal(&byte_array, &bytes_1, &number_of_bytes_1)) {
    dx_inline_byte_array_uninitialize(&byte_array);
    return DX_FAILURE;
  }
  dx_inline_byte_array_uninitialize(&byte_array);
  *bytes = bytes_1;
  *number_of_bytes = number_of_bytes_1;
  
  return DX_SUCCESS;
}

dx_result dx_get_file_contents_s(dx_string* path, char** bytes, dx_size* number_of_bytes) {
  //
  if (dx_string_contains_symbol(path, '\0')) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_string* format = NULL;
  if (dx_string_create(&format, "${s}\0", sizeof("${s}\0") - 1)) {
    return DX_FAILURE;
  }
  path = dx_string_printf(format, path);
  DX_UNREFERENCE(format);
  if (!path) {
    return DX_FAILURE;
  }
  //
  if (dx_get_file_contents(path->bytes, bytes, number_of_bytes)) {
    DX_UNREFERENCE(path);
    path = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(path);
  path = NULL;
  return DX_SUCCESS;
}

dx_result dx_get_file_state(DX_FILE_STATE* RETURN, char const* path) {
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
      *RETURN = DX_FILE_STATE_NOTFOUND;
      return DX_SUCCESS;
    } else {
      // The case was not properly handled.
      dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
      return DX_FAILURE;
    }
  } else {
    if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & t)) {
      *RETURN = DX_FILE_STATE_ISDIRECTORY;
    } else {
      *RETURN = DX_FILE_STATE_ISREGULAR;
    }
    return DX_SUCCESS;
  }
}

dx_result dx_get_file_state_s(DX_FILE_STATE* RETURN, dx_string* path) {
  //
  if (dx_string_contains_symbol(path, '\0')) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_string* format = NULL;
  if (dx_string_create(&format, "${s}\0", sizeof("${s}\0") - 1)) {
    return DX_FAILURE;
  }
  path = dx_string_printf(format, path);
  DX_UNREFERENCE(format);
  if (!path) {
    return 1;
  }
  //
  DX_FILE_STATE file_state;
  if (dx_get_file_state(&file_state, path->bytes)) {
    DX_UNREFERENCE(path);
    path = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(path);
  path = NULL;
  *RETURN = file_state;
  return DX_SUCCESS;
}
