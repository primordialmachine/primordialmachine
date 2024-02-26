// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/FileHandle.h"

#include <stdio.h>
#include <malloc.h>

Core_Result
Core_FileHandle_create
  (
    Core_FileHandle **fileHandle,
    Core_String* pathName,
    Core_FileAccessMode fileAccessMode,
    Core_ExistingFilePolicy existingFilePolicy,
    Core_NonExistingFilePolicy nonExistingFilePolicy
  )
{
  // Validate arguments.
  if (!fileHandle || !pathName) {
    //fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);
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

  // Translate desired access and (non) existing file policies.
  DWORD dwDesiredAccess = 0;
  if (!(fileAccessMode & (Core_FileAccessMode_ReadWrite))) {
    //fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);
    CORE_UNREFERENCE(pathName);
    pathName = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (fileAccessMode & Core_FileAccessMode_Read) {
    dwDesiredAccess |= GENERIC_READ;
  }
  if (fileAccessMode & Core_FileAccessMode_Write) {
    dwDesiredAccess |= GENERIC_WRITE;
  }
  DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
  DWORD dwCreationDisposition = 0;
  typedef struct PolicyMapping {
    Core_ExistingFilePolicy existingFilePolicy;
    Core_NonExistingFilePolicy nonExistingFilePolicy;
    DWORD dwCreationDisposition;
    DWORD dwDesiredAccessModifier;
  } PolicyMapping;
  static const PolicyMapping policyMappings[] = {
    // CREATE_NEW is a conditional flag which fails
    // if the file exists and succeeds if the file
    // does not exist and is successfully created.
    // It is not used.
    { Core_ExistingFilePolicy_Truncate,
      Core_NonExistingFilePolicy_Create,
      CREATE_ALWAYS,
      0 },
    { Core_ExistingFilePolicy_Retain,
      Core_NonExistingFilePolicy_Create,
      OPEN_ALWAYS,
      0 },
    { Core_ExistingFilePolicy_Truncate,
      Core_NonExistingFilePolicy_Fail,
      TRUNCATE_EXISTING,
      GENERIC_WRITE },
    { Core_ExistingFilePolicy_Retain,
      Core_NonExistingFilePolicy_Fail,
      OPEN_EXISTING,
      0 },
  };
  const PolicyMapping *policyMapping = NULL;
  for (Core_Size i = 0, n = sizeof(policyMappings) / sizeof(PolicyMapping); i < n; ++i) {
    policyMapping = policyMappings + i;
    if (policyMapping->existingFilePolicy == existingFilePolicy &&
        policyMapping->nonExistingFilePolicy == nonExistingFilePolicy) {
      break;
    }
  }
  if (policyMapping == NULL) {
    //fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);
    CORE_UNREFERENCE(pathName);
    pathName = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  dwDesiredAccess |= policyMapping->dwDesiredAccessModifier;
  Core_FileHandle *temporaryFileHandle = malloc(sizeof(Core_FileHandle));
  if (!temporaryFileHandle) {
     //fprintf(stderr, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(Core_FileHandle));
    CORE_UNREFERENCE(pathName);
    pathName = NULL;
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  temporaryFileHandle->hFileHandle = CreateFileA(pathName->bytes, dwDesiredAccess, dwShareMode, 0, policyMapping->dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
  if (INVALID_HANDLE_VALUE == temporaryFileHandle->hFileHandle) {
    free(temporaryFileHandle);
    temporaryFileHandle = NULL;
    CORE_UNREFERENCE(pathName);
    pathName = NULL;
    //fprintf(stderr, "%s:%d: unable to open file '%s'\n", __FILE__, __LINE__, pathname);
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  CORE_UNREFERENCE(pathName);
  pathName = NULL;
  *fileHandle = temporaryFileHandle;
  return Core_Success;
}

Core_Result
Core_FileHandle_destroy
  (
    Core_FileHandle *fileHandle
  )
{
  // Close file handle.
  if (INVALID_HANDLE_VALUE != fileHandle->hFileHandle) {
    CloseHandle(fileHandle->hFileHandle);
    fileHandle->hFileHandle = INVALID_HANDLE_VALUE;
  }
  // Deallocate file handle.
  free(fileHandle);
  return Core_Success;
}

Core_Result
Core_FileHandle_getFileSize
  (
    Core_FileHandle *fileHandle,
    Core_Size *fileSize
  )
{
  if (!fileHandle || !fileSize || INVALID_HANDLE_VALUE == fileHandle->hFileHandle) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // Get the size of the file. The file size must be smaller than or equal to Core_Size_Greatest.
  DWORD temporaryFileSize = GetFileSize(fileHandle->hFileHandle, NULL);
  if (INVALID_FILE_SIZE == temporaryFileSize || temporaryFileSize > Core_Size_Greatest) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *fileSize = temporaryFileSize;
  return Core_Success;
}
