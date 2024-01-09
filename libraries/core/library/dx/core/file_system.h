/// @file dx/core/file_system.h
/// @brief Core functionality related to the file system.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_FILE_SYSTEM_H_INCLUDED)
#define DX_CORE_FILE_SYSTEM_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/string.h"
#include "Core/FileSystem/ExistingFilePolicy.h"
#include "Core/FileSystem/FileAccessMode.h"
#include "Core/FileSystem/NonExistingFilePolicy.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareEnumerationType("Core.FileState",
                            Core_FileState);

/// @ingroup Core_FileSystem
/// @brief Enumeration of file states.
enum Core_FileState {

  /// @brief Unable to determine the file's state.
  Core_FileState_Unknown,

  /// @brief A file was found and is a director file.
  Core_FileState_IsDirectory,

  /// @brief A file was found and is a regular file.
  Core_FileState_IsRegular,

  /// @brief A file was not found.
  Core_FileState_NotFound,

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief
/// Get the contents of a file.
/// @param path
/// The path to the file.
/// @param bytes
/// A pointer to a <code>char*</code> variable.
/// @param number_of_bytes
/// A pointer to a <code>Core_Size</code> variable.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the contents.
/// <code>*bytes</code> was assigned a pointer to an array of that length.
/// That array must be deallocated using Core_Memory_deallocate() when no long required.
/// @procedure
Core_Result dx_get_file_contents(char const *path, char **bytes, Core_Size*number_of_bytes);

/// @ingroup Core_FileSystem
/// @brief
/// Get the contents of a file.
/// @param path
/// The path to the file.
/// @param bytes
/// A pointer to a <code>char*</code> variable.
/// @param number_of_bytes
/// A pointer to a <code>Core_Size</code> variable.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the contents.
/// <code>*bytes</code> was assigned a pointer to an array of that length.
/// That array must be deallocated using Core_Memory_deallocate() when no long required.
/// @procedure
Core_Result dx_get_file_contents_s(Core_String* path, char** bytes, Core_Size* number_of_bytes);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief Get the state of a file.
/// @param RETURN A pointer to a Core_FileState variable.
/// @param path The path to the file.
/// @success <code>*RETURN</code> was assigned the file state.
/// @procedure
Core_Result dx_get_file_state(Core_FileState* RETURN, char const* path);

/// @ingroup Core_FileSystem
/// @brief Get the state of a file.
/// @param RETURN A pointer to a Core_FileState variable.
/// @param path The path to the file.
/// @success <code>*RETURN</code> was assigned the file state.
/// @procedure
Core_Result dx_get_file_state_s(Core_FileState* RETURN, Core_String* path);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_FILE_SYSTEM_H_INCLUDED
