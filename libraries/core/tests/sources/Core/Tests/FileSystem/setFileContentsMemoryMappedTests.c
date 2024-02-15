#include "Core/Tests/FileSystem/setFileContentsMemoryMappedTests.h"

static Core_Result Core_Tests_FileSystem_setFileContentsMemoryMappedTest1() {
  Core_String* path;
  if (Core_String_create(&path, "./assets/setFileContentsMemoryMappedTests/helloworld.txt", sizeof("./assets/setFileContentsMemoryMappedTests/helloworld.txt") - 1)) {
    return Core_Failure;
  }
  if (Core_deleteRegularFile(path)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  }
  Core_Natural8 const bytes[] = { 'H' , 'e' , 'l' , 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\r', '\n' };
  Core_Size numberOfBytes = sizeof(bytes) - 1;
  if (Core_setFileContentsMemoryMapped(path, bytes, numberOfBytes)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Tests_FileSystem_setFileContentsMemoryMappedTests() {
  if (Core_Tests_FileSystem_setFileContentsMemoryMappedTest1()) {
    return Core_Failure;
  }
  return Core_Success;
}
