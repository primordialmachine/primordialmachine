#include "FileSystemRoots.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Core_Result Core_getFileSystemRoots(Core_InlineArrayListP* list) {
  char* xl = NULL;
  if (Core_Memory_allocate(&xl, MAX_PATH)) {
    return Core_Failure;
  }
  if (GetLogicalDriveStringsA(MAX_PATH, xl) > MAX_PATH) {
    Core_Memory_deallocate(xl);
    xl = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  char* x = xl;
  while ('\0' != *x) {
    Core_String* listElement = NULL;
    if (Core_String_create(&listElement, x, strlen(x))) {
      Core_Memory_deallocate(xl);
      xl = NULL;
      return Core_Failure;
    }
    if (Core_InlineArrayListP_append(list, listElement)) {
      CORE_UNREFERENCE(listElement);
      listElement = NULL;
      Core_Memory_deallocate(xl);
      xl = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(listElement);
    listElement = NULL;
    x += strlen(x) + 1;
  }
  Core_Memory_deallocate(xl);
  xl = NULL;
  return Core_Success;
}
