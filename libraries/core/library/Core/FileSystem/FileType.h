// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_FILETYPE_H_INCLUDED)
#define CORE_FILESYSTEM_FILETYPE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/core#core-filetype */
Core_declareEnumerationType("Core.FileType",
                            Core_FileType);

enum Core_FileType {

  Core_FileType_Directory = 1,

  Core_FileType_Regular,

};

#endif // CORE_FILESYSTEM_FILETYPE_H_INCLUDED
