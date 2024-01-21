// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_GETFILECONTENTS_H_INCLUDED)
#define CORE_FILESYSTEM_GETFILECONTENTS_H_INCLUDED

#include "dx/core/string.h"

/* https://localhost/core#core-getfilecontents */
Core_Result Core_getFileContents(Core_String* path, Core_Natural8** bytes, Core_Size* numberOfBytes);

#endif // CORE_FILESYSTEM_GETFILECONTENTS_H_INCLUDED
