// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_NONEXISTINGFILEPOLICY_H_INCLUDED)
#define CORE_FILESYSTEM_NONEXISTINGFILEPOLICY_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/core#core-nonexistingfilepolicy */
Core_declareEnumerationType("Core.NonExistingFilePolicy",
                            Core_NonExistingFilePolicy);

enum Core_NonExistingFilePolicy {

  Core_NonExistingFilePolicy_Fail,

  Core_NonExistingFilePolicy_Create,

};

#endif // CORE_FILESYSTEM_NONEXISTINGFILEPOLICY_H_INCLUDED
