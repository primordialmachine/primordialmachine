#if !defined(CORE_FILESYSTEMROOTS_H_INCLUDED)
#define CORE_FILESYSTEMROOTS_H_INCLUDED

#include "dx/core.h"

// Add strings with file system roots to the list.
// Under Linux: Usually only the string "/" is added.
// Under Windows: Strings like "A:\", "B:\", ..., "X:\" are added.
Core_Result Core_getFileSystemRoots(Core_InlineArrayListP* list);

#endif // CORE_FILESYSTEMROOTS_H_INCLUDED
