// Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_NONEXISTINGFILEPOLICY_H_INCLUDED)
#define CORE_FILESYSTEM_NONEXISTINGFILEPOLICY_H_INCLUDED

#include "dx/core/TypeSystem.h"

/// @ingroup Core_FileSystem
/// @brief An enumeration of policies for opening a file in case of that the file does not exist.
/// @remarks The enumeration elements can not be combined.
DX_DECLARE_ENUMERATION_TYPE("Core.NonExistingFilePolicy",
                            Core_NonExistingFilePolicy);

enum Core_NonExistingFilePolicy {

  /// @brief Fail if the file does not exist.
  Core_NonExistingFilePolicy_Fail,

  /// @brief Create the file if it does not exist.
  Core_NonExistingFilePolicy_Create,

};

#endif // CORE_FILESYSTEM_NONEXISTINGFILEPOLICY_H_INCLUDED
