// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_FILEACCESSMODE_H_INCLUDED)
#define CORE_FILESYSTEM_FILEACCESSMODE_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/core#core-fileaccessmode */
Core_declareEnumerationType("Core.FileAccessMode",
                            Core_FileAccessMode);

enum Core_FileAccessMode {

  Core_FileAccessMode_Read = 1,

  Core_FileAccessMode_Write = 2,

  Core_FileAccessMode_ReadWrite = Core_FileAccessMode_Read | Core_FileAccessMode_Write,

  Core_FileAccessMode_WriteRead = Core_FileAccessMode_Write | Core_FileAccessMode_Read,

};

#endif // CORE_FILESYSTEM_FILEACCESSMODE_H_INCLUDED
