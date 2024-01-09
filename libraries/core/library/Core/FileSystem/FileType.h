// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_FILETYPE_H_INCLUDED)
#define CORE_FILESYSTEM_FILETYPE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/core#core-filetype */
Core_declareEnumerationType("Core.FileType",
                            Core_FileType);

/// @ingroup Core_FileSystem
/// @brief Enumeration of file types.
enum Core_FileType {

  /// @brief A file is a directory file.
  Core_FileType_Directory,

  /// @brief A file is a regular file.
  Core_FileType_Regular,

};

#endif // CORE_FILESYSTEM_FILETYPE_H_INCLUDED
