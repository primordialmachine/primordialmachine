/// @file Runtime/IO/_FileMapping.h
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#if !defined(MACHINE_RUNTIME_IO__FILEMAPPING_H_INCLUDED)
#define MACHINE_RUNTIME_IO__FILEMAPPING_H_INCLUDED

#include "Runtime/IO/_FileHandle.h"
#include "Runtime/Status.h"

/// @brief Helper for file mappings.
typedef struct _Machine_FileMapping {
  _Machine_FileHandle fileHandle;
  HANDLE hFileMapping;
  char* bytes;
  size_t numberOfBytes;
} _Machine_FileMapping; // struct Nucleus_FileMapping

static inline void _Machine_FileMapping_openRead(_Machine_FileMapping* self, Machine_String* path) {
  _Machine_FileHandle_open(&self->fileHandle,
                          path,
                          Machine_FileAccessMode_Read,
                          Machine_ExistingFilePolicy_Retain,
                          Machine_NonExistingFilePolicy_Fail);
  Machine_JumpTarget jumpTarget1;
  Machine_pushJumpTarget(&jumpTarget1);
  if (!setjmp(jumpTarget1.environment)) {
    self->numberOfBytes = _Machine_FileHandle_getFileSize(&self->fileHandle);
    // If the file is empty, use a dummy buffer.
    if (0 == self->numberOfBytes) {
      // Use a dummy buffer.
      static const char DUMMY = 0;
      self->bytes = (char*)&DUMMY;
    } else {
      // Create file mapping.
      self->hFileMapping = CreateFileMapping(self->fileHandle.hHandle, 0, PAGE_READONLY, 0, 0, 0);
      if (NULL == self->hFileMapping) {
        Machine_log(Machine_LogFlags_ToErrors, __FILE__, __LINE__, "unable to create file mapping for file '%s'\n",
                    Machine_String_getBytes(path));
        Machine_setStatus(Machine_Status_EnvironmentFailed);
        Machine_jump();
      }
      // Create view of file mapping.
      self->bytes = (char*)MapViewOfFile(self->hFileMapping, FILE_MAP_READ, 0, 0, 0);
      if (NULL == self->bytes) {
        // Close file mapping.
        CloseHandle(self->hFileMapping);
        self->hFileMapping = NULL;
        //
        Machine_log(Machine_LogFlags_ToErrors, __FILE__, __LINE__, "unable to create file mapping view for file '%s'\n",
                    Machine_String_getBytes(path));
        Machine_setStatus(Machine_Status_EnvironmentFailed);
        Machine_jump();
      }
    }
    //
    Machine_popJumpTarget();
  } else {
    Machine_popJumpTarget();
    //
    _Machine_FileHandle_close(&self->fileHandle);
  }
}

static inline void _Machine_FileMapping_openWrite(_Machine_FileMapping* self, Machine_String* path, size_t numberOfBytes) {
  _Machine_FileHandle_open(&self->fileHandle,
                           path,
                           Machine_FileAccessMode_Write,
                           Machine_ExistingFilePolicy_Truncate,
                           Machine_NonExistingFilePolicy_Create);
  Machine_JumpTarget jumpTarget1;
  Machine_pushJumpTarget(&jumpTarget1);
  if (!setjmp(jumpTarget1.environment)) {
    self->numberOfBytes = numberOfBytes;
    // If the file is empty, use a dummy buffer.
    if (0 == self->numberOfBytes) {
      // Use a dummy buffer.
      static const char DUMMY = 0;
      self->bytes = (char*)&DUMMY;
    }
    else {
      // Create file mapping.
      self->hFileMapping = CreateFileMapping(self->fileHandle.hHandle, 0, PAGE_READWRITE, 0, numberOfBytes, 0);
      if (NULL == self->hFileMapping) {
        Machine_log(Machine_LogFlags_ToErrors, __FILE__, __LINE__, "unable to create file mapping for file '%s'\n",
                    Machine_String_getBytes(path));
        Machine_setStatus(Machine_Status_EnvironmentFailed);
        Machine_jump();
      }
      // Create view of file mapping.
      self->bytes = (char*)MapViewOfFile(self->hFileMapping, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
      if (NULL == self->bytes) {
        // Close file mapping.
        CloseHandle(self->hFileMapping);
        self->hFileMapping = NULL;
        //
        Machine_log(Machine_LogFlags_ToErrors, __FILE__, __LINE__, "unable to create file mapping view for file '%s'\n",
                    Machine_String_getBytes(path));
        Machine_setStatus(Machine_Status_EnvironmentFailed);
        Machine_jump();
      }
    }
    //
    Machine_popJumpTarget();
  }
  else {
    Machine_popJumpTarget();
    //
    _Machine_FileHandle_close(&self->fileHandle);
  }
}

static inline void _Machine_FileMapping_close(_Machine_FileMapping* self) {
  if (self->hFileMapping) {
    if (self->bytes) {
      UnmapViewOfFile(self->bytes);
      self->bytes = NULL;
    }
    CloseHandle(self->hFileMapping);
    self->hFileMapping = NULL;
  }
  self->numberOfBytes = 0;
  _Machine_FileHandle_close(&self->fileHandle);
}

#endif // MACHINE_RUNTIME_IO__FILEMAPPING_H_INCLUDED
