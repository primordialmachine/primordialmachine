/// @file dx/core/file_system.h
/// @brief Core functionality related to the file system.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_FILE_SYSTEM_H_INCLUDED)
#define DX_CORE_FILE_SYSTEM_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/string.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief An enumeration of file access modes to files.
/// @remarks The enumeration elements can be combined.
DX_DECLARE_ENUMERATION_TYPE("Core_FileAccessMode",
                            Core_FileAccessMode);

enum Core_FileAccessMode {

  /// @brief Read access.
  /// @remark Can be combined with dx_file_access_mode_write.
  Core_FileAccessMode_Read = 1,

  /// @brief Write access.
  /// @remark Can be combined with dx_file_access_mode_read.
  Core_FileAccessMode_Write = 2,

  /// @brief Read and write access.
  /// @remark Alias for @code{Core_FileAccessMode_Read|Core_FileAccessMode_Write}.
  /// Alias for @code{Core_FileAccessMode_WriteRead}.
  Core_FileAccessMode_ReadWrite = Core_FileAccessMode_Read | Core_FileAccessMode_Write,

  /// @brief Write and read access.
  /// @remark Alias for @code{Core_FileAccessMode_Write|Core_FileAccessMode_Read}.
  /// Alias for @code{Core_FileAccessMode_ReadWrite}.
  Core_FileAccessMode_WriteRead = Core_FileAccessMode_Write | Core_FileAccessMode_Read,

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief An enumeration of policies for opening a file in case of that the file exists.
/// @remarks The enumeration elements can not be combined.
DX_DECLARE_ENUMERATION_TYPE("Core_ExistingFilePolicy",
                            Core_ExistingFilePolicy);

enum Core_ExistingFilePolicy {

  /// @brief Retain the file contents.
  Core_ExistingFilePolicy_Retain,

  /// @brief Truncate the file contents.
  Core_ExistingFilePolicy_Truncate,

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief An enumeration of policies for opening a file in case of that the file does not exist.
/// @remarks The enumeration elements can not be combined.
DX_DECLARE_ENUMERATION_TYPE("Core_NonExistingFilePolicy",
                            Core_NonExistingFilePolicy);

enum Core_NonExistingFilePolicy {

  /// @brief Fail if the file does not exist.
  Core_NonExistingFilePolicy_Fail,

  /// @brief Create the file if it does not exist.
  Core_NonExistingFilePolicy_Create,

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("Core.FileState",
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
