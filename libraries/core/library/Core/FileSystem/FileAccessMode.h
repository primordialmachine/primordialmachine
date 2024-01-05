// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_FILEACCESSMODE_H_INCLUDED)
#define CORE_FILESYSTEM_FILEACCESSMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/// @ingroup Core_FileSystem
/// @brief An enumeration of file access modes.
/// @remarks The enumeration elements can be combined.
DX_DECLARE_ENUMERATION_TYPE("Core.FileAccessMode",
                            Core_FileAccessMode);

enum Core_FileAccessMode {

  /// @brief Read access.
  /// @remark Can be combined with @{code('Core_FileAccessMode_Write')}.
  Core_FileAccessMode_Read = 1,

  /// @brief Write access.
  /// @remark Can be combined with @{code('Core_FileAccessMode_Read')}.
  Core_FileAccessMode_Write = 2,

  /// @brief Read and write access.
  /// @remark Alias for @{code('Core_FileAccessMode_Read|Core_FileAccessMode_Write')}.
  /// Alias for @{code('Core_FileAccessMode_WriteRead')}.
  Core_FileAccessMode_ReadWrite = Core_FileAccessMode_Read | Core_FileAccessMode_Write,

  /// @brief Write and read access.
  /// @remark Alias for @{code('Core_FileAccessMode_Write|Core_FileAccessMode_Read')}.
  /// Alias for @{code('Core_FileAccessMode_ReadWrite')}.
  Core_FileAccessMode_WriteRead = Core_FileAccessMode_Write | Core_FileAccessMode_Read,

};

#endif // CORE_FILESYSTEM_FILEACCESSMODE_H_INCLUDED
