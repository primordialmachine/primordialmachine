/// @file dx/core/file_system.h
/// @brief Core functionality related to the file system.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_FILE_SYSTEM_H_INCLUDED)
#define DX_CORE_FILE_SYSTEM_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/core.h"
#include "dx/core/string.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief An enumeration of file access modes to files.
/// @remarks The enumeration elements can be combined.
typedef enum DX_FILE_ACCESS_MODE {

  /// @brief Read access.
  /// @remark Can be combined with DX_FILE_ACCESS_MODE_WRITE.
  DX_FILE_ACCESS_MODE_READ = 1,

  /// @brief Write access.
  /// @remark Can be combined with DX_FILE_ACCESS_MODE_READ.
  DX_FILE_ACCESS_MODE_WRITE = 2,

  /// @brief Read and write access.
  /// @remark Alias for @code{DX_FILE_ACCESS_MODE_READ|DX_FILE_ACCESS_MODE_WRITE}.
  /// Alias for @code{DX_FILE_ACCESS_MODE_WRITE_READ}.
  DX_FILE_ACCESS_MODE_READ_WRITE = DX_FILE_ACCESS_MODE_READ | DX_FILE_ACCESS_MODE_WRITE,

  /// @brief Write and read access.
  /// @remark Alias for @code{DX_FILE_ACCESS_MODE_WRITE|DX_FILE_ACCESS_MODE_READ}.
  /// Alias for @code{DX_FILE_ACCESS_MODE_READ_WRITE}.
  DX_FILE_ACCESS_MODE_WRITE_READ = DX_FILE_ACCESS_MODE_WRITE | DX_FILE_ACCESS_MODE_READ,

} DX_FILE_ACCESS_MODE;

/// @ingroup Core_FileSystem
/// @brief An enumeration of policies for opening a file in case of that the file exists.
/// @remarks The enumeration elements can not be combined.
typedef enum DX_EXISTING_FILE_POLICY {

  /// @brief Retain the file.
  DX_EXISTING_FILE_POLICY_RETAIN,

  /// @brief Truncate the file.
  DX_EXISTING_FILE_POLICY_TRUNCATE,

} DX_EXISTING_FILE_POLICY;

/// @ingroup Core_FileSystem
/// @brief An enumeration of policies for opening a file in case of that the file does not exist.
/// @remarks The enumeration elements can not be combined.
typedef enum DX_NON_EXISTING_FILE_POLICY {

  /// @brief Fail.
  DX_NON_EXISTING_FILE_POLICY_FAIL,

  /// @brief Create the file.
  DX_NON_EXISTING_FILE_POLICY_CREATE,

} DX_NON_EXISTING_FILE_POLICY;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief
/// Get the contents of a file.
/// @param path
/// The path to the file.
/// @param bytes
/// A pointer to a <code>char*</code> variable.
/// @param number_of_bytes
/// A pointer to a <code>dx_size</code> variable.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the contents.
/// <code>*bytes</code> was assigned a pointer to an array of that length.
/// That array must be deallocated using dx_memory_deallocate() when no long required.
/// @default-return
/// @default-failure
int dx_get_file_contents(char const *path, char **bytes, dx_size*number_of_bytes);

/// @ingroup Core_FileSystem
/// @brief
/// Get the contents of a file.
/// @param path
/// The path to the file.
/// @param bytes
/// A pointer to a <code>char*</code> variable.
/// @param number_of_bytes
/// A pointer to a <code>dx_size</code> variable.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the contents.
/// <code>*bytes</code> was assigned a pointer to an array of that length.
/// That array must be deallocated using dx_memory_deallocate() when no long required.
/// @default-return
/// @default-failure
int dx_get_file_contents_s(dx_string* path, char** bytes, dx_size* number_of_bytes);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_FileSystem
/// @brief Enumeration of file states.
typedef enum DX_FILE_STATE {

  /// @brief Unable to determine the file's state.
  DX_FILE_STATE_UNKNOWN,

  /// @brief A file was found and is a director file.
  DX_FILE_STATE_ISDIRECTORY,
  
  /// @brief A file was found and is a regular file.
  DX_FILE_STATE_ISREGULAR,
  
  /// @brief A file was not found.
  DX_FILE_STATE_NOTFOUND,
  
} DX_FILE_STATE;

/// @ingroup Core_FileSystem
/// @brief
/// Get the state of a file.
/// @param path
/// The path to the file.
/// @return
/// A DX_FILE_STATE value not equal to DX_FILE_STATE_UNKNOWN on success.
/// DX_FILE_STATE_UNKNOWN on failure.
/// @default-failure 
DX_FILE_STATE dx_get_file_state(char const* path);

/// @ingroup Core_FileSystem
/// @brief
/// Get the state of a file.
/// @param path
/// The path to the file.
/// @return
/// A DX_FILE_STATE value not equal to DX_FILE_STATE_UNKNOWN on success.
/// DX_FILE_STATE_UNKNOWN on failure.
/// @default-failure 
DX_FILE_STATE dx_get_file_state_s(dx_string* path);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_FILE_SYSTEM_H_INCLUDED
