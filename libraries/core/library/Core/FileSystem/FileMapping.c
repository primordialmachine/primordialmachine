// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/FileMapping.h"

static const char DUMMY = 0;

Core_Result
Core_FileMapping_initializeWrite
  (
    Core_FileMapping* fileMapping,
    Core_String* pathName,
    Core_Size numberOfBytes
  )
{
  if (!pathName) {
    /*fprintf(stderr, "invalid arguments\n");*/
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // Initialize members that might not be initialized later on with safe defaults.
  fileMapping->hFileMapping = NULL;
  fileMapping->fileHandle = NULL;
  // Open the file handle.
  if (Core_FileHandle_create(&fileMapping->fileHandle,
                             pathName,
                             Core_FileAccessMode_ReadWrite,
                             Core_ExistingFilePolicy_Truncate,
                             Core_NonExistingFilePolicy_Create)) {
    return Core_Failure;
  }
  // If the number of Bytes is zero, this function succeeds.
  if (numberOfBytes == 0) {
    fileMapping->numberOfBytes = numberOfBytes;
    fileMapping->bytes = (void*)&DUMMY;
    // Return the result.
    return Core_Success;
  } else {
    // Create file mapping.
    fileMapping->hFileMapping = CreateFileMapping(fileMapping->fileHandle->hFileHandle, 0, PAGE_READWRITE, 0, numberOfBytes, NULL);
    if (NULL == fileMapping->hFileMapping) {
      /*fprintf(stderr, "unable to create file mapping of file '%s'\n", pathName);*/
      // Destroy file handle.
      Core_FileHandle_destroy(fileMapping->fileHandle);
      fileMapping->fileHandle = NULL;
      // Return the result.
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
  }
  // Create view of file mapping.
  fileMapping->bytes = (char*)MapViewOfFile(fileMapping->hFileMapping, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
  if (!fileMapping->bytes) {
    /*fprintf(stderr, "unable to create view of file '%s'\n", pathname);*/
    // Close file mapping.
    CloseHandle(fileMapping->hFileMapping);
    fileMapping->hFileMapping = NULL;
    // Destroy file handle.
    Core_FileHandle_destroy(fileMapping->fileHandle);
    fileMapping->fileHandle = NULL;
    // Return the result.
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }

  return Core_Success;
}

Core_Result
Core_FileMapping_initializeRead
  (
    Core_FileMapping* fileMapping,
    Core_String* pathName
  )
{
  if (!pathName) {
    /*fprintf(stderr, "invalid arguments\n");*/
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // Initialize members that might not be initialized later on with safe defaults.
  fileMapping->hFileMapping = NULL;
  fileMapping->fileHandle = NULL;

  // Open the file handle.
  if (Core_FileHandle_create(&fileMapping->fileHandle,
                             pathName,
                             Core_FileAccessMode_Read,
                             Core_ExistingFilePolicy_Retain,
                             Core_NonExistingFilePolicy_Fail)) {
    return Core_Failure;
  }
  // Determine file size.
  if (Core_FileHandle_getFileSize(fileMapping->fileHandle, &fileMapping->numberOfBytes)) {
    /*fprintf(stderr, "unable to open file '%s'\n", pathName);*/
    Core_FileHandle_destroy(fileMapping->fileHandle);
    fileMapping->fileHandle = NULL;
    return Core_Failure;
  }
  // If the file is empty, use a dummy buffer.
  if (0 == fileMapping->numberOfBytes) {
      // Use a dummy buffer.
    fileMapping->bytes = (char*)&DUMMY;
    // Close file handle.
    Core_FileHandle_destroy(fileMapping->fileHandle);
    fileMapping->fileHandle = NULL;
    // Return the result.
    return Core_Success;
  }
  // Create file mapping.
  fileMapping->hFileMapping = CreateFileMapping(fileMapping->fileHandle->hFileHandle, 0, PAGE_READONLY, 0, 0, 0);
  if (NULL == fileMapping->hFileMapping) {
    /*fprintf(stderr, "unable to open file '%s'\n", pathName);*/
    // Close file handle.
    Core_FileHandle_destroy(fileMapping->fileHandle);
    fileMapping->fileHandle = NULL;
    // Return the result.
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  // Create view of file mapping.
  fileMapping->bytes = (char*)MapViewOfFile(fileMapping->hFileMapping, FILE_MAP_READ, 0, 0, 0);
  if (NULL == fileMapping->bytes) {
    /*fprintf(stderr, "unable to create view of file '%s'\n", pathName);*/
    // Close file mapping.
    CloseHandle(fileMapping->hFileMapping);
    fileMapping->hFileMapping = NULL;
    // Close file handle.
    Core_FileHandle_destroy(fileMapping->fileHandle);
    fileMapping->fileHandle = NULL;
    // Return the result.
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  // Return the result.
  return Core_Success;
}

void
Core_FileMapping_uninitialize
  (
    Core_FileMapping* fileMapping
  )
{
  // Close view of file.
  if (&DUMMY != fileMapping->bytes) {
    UnmapViewOfFile(fileMapping->bytes);
    fileMapping->bytes = NULL;
  }
  // Close file mapping.
  if (NULL != fileMapping->hFileMapping) {
    CloseHandle(fileMapping->hFileMapping);
    fileMapping->hFileMapping = NULL;
  }
  // Close file.
  if (fileMapping->fileHandle) {
    // Close file handle.
    Core_FileHandle_destroy(fileMapping->fileHandle);
    fileMapping->fileHandle = NULL;
  }
}
