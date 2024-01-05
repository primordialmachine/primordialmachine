// Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_EXISTINGFILEPOLICY_H_INCLUDED)
#define CORE_FILESYSTEM_EXISTINGFILEPOLICY_H_INCLUDED

#include "dx/core/TypeSystem.h"

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

#endif // CORE_FILESYSTEM_EXISTINGFILEPOLICY_H_INCLUDED
