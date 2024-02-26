// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_FILEMAPPING_H_INCLUDED)
#define CORE_FILESYSTEM_FILEMAPPING_H_INCLUDED

#include "Core/FileSystem/FileHandle.h"

/// @ingroup filesystem
/// @brief Helper to store the objects related to a file mapping.
typedef struct Core_FileMapping {
  Core_FileHandle* fileHandle;
  
  HANDLE hFileMapping;

  void* bytes;
  Core_Size numberOfBytes;

} Core_FileMapping; // struct Core_FileMapping

Core_Result
Core_FileMapping_initializeWrite
  (
    Core_FileMapping* fileMapping,
    Core_String* pathName,
    Core_Size numberOfBytes
  );

Core_Result
Core_FileMapping_initializeRead
  (
    Core_FileMapping* fileMapping,
    Core_String* pathName
  );

void
Core_FileMapping_uninitialize
  (
    Core_FileMapping* fileMapping
  );

#endif // CORE_FILESYSTEM_FILEMAPPING_H_INCLUDED
