// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_FILEHANDLE_H_INCLUDED)
#define CORE_FILESYSTEM_FILEHANDLE_H_INCLUDED

#include "Core/String.h"
#include "Core/FileSystem/ExistingFilePolicy.h"
#include "Core/FileSystem/FileAccessMode.h"
#include "Core/FileSystem/NonExistingFilePolicy.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef struct Core_FileHandle {
  HANDLE hFileHandle;
} Core_FileHandle;

Core_Result
Core_FileHandle_create
  (
    Core_FileHandle **fileHandle,
    Core_String *pathName,
    Core_FileAccessMode fileAccessMode,
    Core_ExistingFilePolicy existingFilePolicy,
    Core_NonExistingFilePolicy nonExistingFilePolicy
  );

Core_Result
Core_FileHandle_destroy
  (
    Core_FileHandle *fileHandle
  );

Core_Result
Core_FileHandle_getFileSize
  (
    Core_FileHandle *fileHandle,
    Core_Size  *fileSize
  );

#endif // CORE_FILESYSTEM_FILEHANDLE_H_INCLUDED
