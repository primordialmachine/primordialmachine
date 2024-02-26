// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_EXISTINGFILEPOLICY_H_INCLUDED)
#define CORE_FILESYSTEM_EXISTINGFILEPOLICY_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/core#core-existingfilepolicy */
Core_declareEnumerationType("Core_ExistingFilePolicy",
                            Core_ExistingFilePolicy);

enum Core_ExistingFilePolicy {

  Core_ExistingFilePolicy_Retain,

  Core_ExistingFilePolicy_Truncate,

};

#endif // CORE_FILESYSTEM_EXISTINGFILEPOLICY_H_INCLUDED
