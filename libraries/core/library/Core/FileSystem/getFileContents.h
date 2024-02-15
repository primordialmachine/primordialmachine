// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_GETFILECONTENTS_H_INCLUDED)
#define CORE_FILESYSTEM_GETFILECONTENTS_H_INCLUDED

#include "Core/String.h"

typedef Core_Result(Core_GetFileContentsCallback)(void* context, Core_Natural8 const* bytes, Core_Size numberOfBytes);

/* https://localhost/core#core-getfilecontents */
Core_Result Core_getFileContents(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes);

Core_Result Core_getFileContentsCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback);

/* http://localhost/core#core-getfilecontentsmemorymapped */
Core_Result Core_getFileContentsMemoryMapped(Core_String* pathName, Core_Natural8** bytes, Core_Size* numberOfBytes);

Core_Result Core_getFileContentsMemoryMappedCallback(Core_String* pathName, void* context, Core_GetFileContentsCallback* callback);

#endif // CORE_FILESYSTEM_GETFILECONTENTS_H_INCLUDED
