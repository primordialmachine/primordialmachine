// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/getFileContents.h"

#include "Core/Collections/InlineArrayListN8.h"
#include "Core/FileSystem/FileHandle.h"
#include "Core/FileSystem/FileMapping.h"
#include "Core/Memory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static Core_Result _getFileContents(Core_String *pathName, Core_Natural8** bytes, Core_Size* numberOfBytes);

static Core_Result _getFileContentsCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback);

static Core_Result _getFileContentsMemoryMapped(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes);

static Core_Result _getFileContentsMemoryMappedCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback);

static Core_Result _getFileContents(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes) {
  Core_InlineArrayListN8 byteArray;
  Core_InlineArrayListN8_Configuration configuration = {
    .addedCallback = NULL,
    .removedCallback = NULL,
  };
  if (Core_InlineArrayListN8_initialize(&byteArray, 0, &configuration)) {
    return Core_Failure;
  }
  Core_FileHandle* fileHandle;
  if (Core_FileHandle_create(&fileHandle,
                             pathName,
                             Core_FileAccessMode_Read,
                             Core_ExistingFilePolicy_Retain,
                             Core_NonExistingFilePolicy_Fail)) {
    Core_InlineArrayListN8_uninitialize(&byteArray);
    return Core_Failure;
  }
  while (true) {
    char temporary[1024]; DWORD requested = 1024;
    DWORD received = 0;
    BOOL result = ReadFile(fileHandle->hFileHandle, temporary, requested, &received, NULL);
    if (!result) {
      Core_FileHandle_destroy(fileHandle);
      fileHandle = NULL;
      Core_InlineArrayListN8_uninitialize(&byteArray);
      return Core_Failure;
    }
    // eof
    if (received == 0) {
      Core_FileHandle_destroy(fileHandle);
      fileHandle = NULL;
      break;
    }
    if (Core_InlineArrayListN8_appendMany(&byteArray, temporary, received)) {
      Core_FileHandle_destroy(fileHandle);
      fileHandle = NULL;
      Core_InlineArrayListN8_uninitialize(&byteArray);
      return Core_Failure;
    }
  }

  Core_Natural8* bytes1; Core_Size numberOfBytes1;
  if (Core_InlineArrayListN8_steal(&byteArray, &bytes1, &numberOfBytes1)) {
    Core_InlineArrayListN8_uninitialize(&byteArray);
    return Core_Failure;
  }
  Core_InlineArrayListN8_uninitialize(&byteArray);
  *bytes = bytes1;
  *numberOfBytes = numberOfBytes1;

  return Core_Success;
}

static Core_Result _getFileContentsCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback) {
  // Validate arguments.
  if (!callback) {
    /*fprintf(stderr, "invalid arguments\n");*/
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_FileHandle* fileHandle;
  if (Core_FileHandle_create(&fileHandle,
                             pathName,
                             Core_FileAccessMode_Read,
                             Core_ExistingFilePolicy_Retain,
                             Core_NonExistingFilePolicy_Fail)) {
    return Core_Failure;
  }
  while (true) {
    char temporary[1024]; DWORD requested = 1024;
    DWORD received = 0;
    BOOL result = ReadFile(fileHandle->hFileHandle, temporary, requested, &received, NULL);
    if (!result) {
      Core_FileHandle_destroy(fileHandle);
      fileHandle = NULL;
      return Core_Failure;
    }
    // eof
    if (received == 0) {
      Core_FileHandle_destroy(fileHandle);
      fileHandle = NULL;
      break;
    }
    if (callback(context, temporary, received)) {
      Core_FileHandle_destroy(fileHandle);
      fileHandle = NULL;
      return Core_Failure;
    }
  }
  return Core_Success;
}

static Core_Result _getFileContentsMemoryMapped(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes) {
  // Validate arguments.
  if (!bytes || !numberOfBytes) {
    /*fprintf(stderr, "invalid arguments\n");*/
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // Open the file mapping.
  Core_FileMapping fileMapping;
  if (Core_FileMapping_initializeRead(&fileMapping, pathName)) {
    return Core_Failure;
  }
  // Allocate target bytes.
  Core_Natural8* temporaryBytes;
  if (Core_Memory_allocate(&temporaryBytes, fileMapping.numberOfBytes)) {
    // Close file mapping.
    Core_FileMapping_uninitialize(&fileMapping);
    return Core_Failure;
  }
  // Copy from source bytes to target bytes.
  if (Core_Memory_copy(temporaryBytes, fileMapping.bytes, fileMapping.numberOfBytes)) {
    Core_Memory_deallocate(temporaryBytes);
    temporaryBytes = NULL;
    // Close file mapping.
    Core_FileMapping_uninitialize(&fileMapping);
    return Core_Failure;
  }
  // Close file mapping.
  Core_FileMapping_uninitialize(&fileMapping);
  // Return the result.
  *numberOfBytes = fileMapping.numberOfBytes;
  *bytes = temporaryBytes;
  return Core_Success;
}

static Core_Result _getFileContentsMemoryMappedCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback) {
  // Validate arguments.
  if (!callback) {
    /*fprintf(stderr, "invalid arguments\n");*/
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // Open the file mapping.
  Core_FileMapping fileMapping;
  if (Core_FileMapping_initializeRead(&fileMapping, pathName)) {
    return Core_Failure;
  }
  // Copy from source bytes to target bytes.
  if (callback(context, fileMapping.bytes, fileMapping.numberOfBytes)) {
    // Close file mapping.
    Core_FileMapping_uninitialize(&fileMapping);
    return Core_Failure;
  }
  // Close file mapping.
  Core_FileMapping_uninitialize(&fileMapping);
  return Core_Success;
}

Core_Result Core_getFileContents(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes) {
  if (_getFileContents(pathName, bytes, numberOfBytes)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_getFileContentsCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback *callback) {
  if (_getFileContentsCallback(pathName, context, callback)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_getFileContentsMemoryMapped(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes) {
  if (_getFileContentsMemoryMapped(pathName, bytes, numberOfBytes)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_getFileContentsMemoryMappedCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback) {
  if (_getFileContentsMemoryMappedCallback(pathName, context, callback)) {
    return Core_Failure;
  }
  return Core_Success;
}
