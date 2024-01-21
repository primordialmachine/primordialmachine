// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_GETFILETYPE_H_INCLUDED)
#define CORE_FILESYSTEM_GETFILETYPE_H_INCLUDED

#include "dx/core/string.h"
#include "Core/FileSystem/FileType.h"

/* http://localhost/core#core-getfiletype */
Core_Result Core_getFileType(Core_FileType* RETURN, Core_String* path);

#endif // CORE_FILESYSTEM_GETFILETYPE_H_INCLUDED
