// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_FILESYSTEM_SETFILECONTENTS_H_INCLUDED)
#define CORE_FILESYSTEM_SETFILECONTENTS_H_INCLUDED

#include "Core/String.h"

/* https://localhost/core#core-setfilecontents */
Core_Result Core_setFileContents(Core_String* pathName, Core_Natural8 const* bytes, Core_Size numberOfBytes);

/* https://localhost/core#core-setfilecontentsmemorymapped */
Core_Result Core_setFileContentsMemoryMapped(Core_String* pathName, Core_Natural8 const* bytes, Core_Size numberOfBytes);

#endif // CORE_FILESYSTEM_SETFILECONTENTS_H_INCLUDED
