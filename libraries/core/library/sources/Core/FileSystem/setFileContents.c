// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/FileSystem/SetFileContents.h"

#include "Core/FileSystem/FileAccessMode.h"
#include "Core/FileSystem/FileHandle.h"
#include "Core/FileSystem/FileMapping.h"

Core_Result Core_setFileContents(Core_String* pathName, Core_Natural8 const* bytes, Core_Size numberOfBytes) {
  if (numberOfBytes > MAXDWORD) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_FileHandle* fileHandle = NULL;
  if (Core_FileHandle_create(&fileHandle,
                             pathName,
                             Core_FileAccessMode_ReadWrite,
                             Core_ExistingFilePolicy_Truncate,
                             Core_NonExistingFilePolicy_Create)) {
    return Core_Failure;
  }
  Core_Size bytesWritten = 0;
  while (bytesWritten < numberOfBytes) {
    DWORD bytesWrittenNowDword;
    DWORD bytesToWriteDword = numberOfBytes - bytesWritten;
    if (!WriteFile(fileHandle->hFileHandle, bytes + bytesWritten, bytesToWriteDword, &bytesWrittenNowDword, NULL)) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    bytesWritten += (Core_Size)bytesWrittenNowDword;
  }
  Core_FileHandle_destroy(fileHandle);
  fileHandle = NULL;
  return Core_Success;
}

Core_Result Core_setFileContentsMemoryMapped(Core_String* pathName, Core_Natural8 const* bytes, Core_Size numberOfBytes) {
  if (!pathName || !bytes) {
    /*fprintf(stderr, "invalid arguments\n");*/
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_FileMapping fileMapping;
  if (Core_FileMapping_initializeWrite(&fileMapping, pathName, numberOfBytes)) {
    return Core_Failure;
  }
  // Write the Bytes.
  memcpy(fileMapping.bytes, bytes, numberOfBytes);
  Core_FileMapping_uninitialize(&fileMapping);
  // Return the result.
  return Core_Success;
}
