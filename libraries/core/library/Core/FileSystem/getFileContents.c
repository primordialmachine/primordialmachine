// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/getFileContents.h"

#include "dx/core/inline_byte_array.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static Core_Result _getFileContents(char const* path, char** bytes, Core_Size* numberOfBytes);

static Core_Result _getFileContents(char const* path, char** bytes, Core_Size* numberOfBytes) {
  Core_InlineArrayN8 byteArray;
  if (Core_InlineArrayN8_initialize(&byteArray)) {
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
    Core_InlineArrayN8_uninitialize(&byteArray);
    return Core_Failure;
  }
  while (true) {
    char temporary[1024]; DWORD requested = 1024;
    DWORD received = 0;
    BOOL result = ReadFile(file, temporary, requested, &received, NULL);
    if (!result) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      Core_InlineArrayN8_uninitialize(&byteArray);
      return Core_Failure;
    }
    // eof
    if (received == 0) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      break;
    }
    if (Core_InlineArrayN8_append(&byteArray, temporary, received)) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      Core_InlineArrayN8_uninitialize(&byteArray);
      return Core_Failure;
    }
  }

  Core_Natural8* bytes1; Core_Size numberOfBytes1;
  if (Core_InlineArrayN8_steal(&byteArray, &bytes1, &numberOfBytes1)) {
    Core_InlineArrayN8_uninitialize(&byteArray);
    return Core_Failure;
  }
  Core_InlineArrayN8_uninitialize(&byteArray);
  *bytes = bytes1;
  *numberOfBytes = numberOfBytes1;

  return Core_Success;
}

Core_Result Core_getFileContents(Core_String* path, Core_Natural8** bytes, Core_Size* numberOfBytes) {
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
  if (_getFileContents(path->bytes, bytes, numberOfBytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  return Core_Success;
}
