#include "Core/Tests/FileSystem/getFileContentsTests.h"

static Core_Result Core_Tests_FileSystem_getFileContentsMemoryMappedTest1() {
  Core_String* path;
  if (Core_String_create(&path, "./assets/getFileContentsMemoryMappedTests/helloworld.txt", sizeof("./assets/getFileContentsMemoryMappedTests/helloworld.txt") - 1)) {
    return Core_Failure;
  }
  Core_Natural8* bytes;
  Core_Size numberOfBytes;
  if (Core_getFileContentsMemoryMapped(path, &bytes, &numberOfBytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  Core_Natural8 const expectedBytes[] = { 'H' , 'e' , 'l' , 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\r', '\n' };
  Core_Size const expectedNumberOfBytes = sizeof(expectedBytes);
  if (expectedNumberOfBytes != numberOfBytes) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_Natural8 compareResult;
  if (Core_Memory_compare(&compareResult, bytes, expectedBytes, expectedNumberOfBytes)) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    return Core_Failure;
  }
  Core_Memory_deallocate(bytes);
  bytes = NULL;
  if (compareResult != 0) {
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result Core_Tests_FileSystem_getFileContentsMemoryMappedTest2() {
  Core_String* path;
  if (Core_String_create(&path, "./assets/getFileContentsMemoryMappedTests/empty.txt", sizeof("./assets/getFileContentsMemoryMappedTests/empty.txt") - 1)) {
    return Core_Failure;
  }
  Core_Natural8* bytes;
  Core_Size numberOfBytes;
  if (Core_getFileContentsMemoryMapped(path, &bytes, &numberOfBytes)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  Core_Natural8 const expectedBytes[] = { 0 };
  Core_Size const expectedNumberOfBytes = 0;
  if (expectedNumberOfBytes != numberOfBytes) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_Natural8 compareResult;
  if (Core_Memory_compare(&compareResult, bytes, expectedBytes, expectedNumberOfBytes)) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    return Core_Failure;
  }
  Core_Memory_deallocate(bytes);
  bytes = NULL;
  if (compareResult != 0) {
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Tests_FileSystem_getFileContentsMemoryMappedTests() {
  if (Core_Tests_FileSystem_getFileContentsMemoryMappedTest1()) {
    return Core_Failure;
  }
  if (Core_Tests_FileSystem_getFileContentsMemoryMappedTest2()) {
    return Core_Failure;
  }
  return Core_Success;
}
