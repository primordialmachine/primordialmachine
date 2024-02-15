#include "Core/Tests/FileSystem/getFileContentsCallbackTests.h"

static Core_Result callback(Core_InlineArrayListN8* array, Core_Natural8 const* bytes, Core_Size numberOfBytes) {
  return Core_InlineArrayListN8_appendMany(array, bytes, numberOfBytes);
}

static Core_Result Core_Tests_FileSystem_getFileContentsMemoryMappedCallbackTest1() {
  Core_InlineArrayListN8_Configuration arrayConfiguration = {
    .addedCallback = NULL,
    .removedCallback = NULL,
  };
  Core_InlineArrayListN8 array;
  if (Core_InlineArrayListN8_initialize(&array, 0, &arrayConfiguration)) {
    return Core_Failure;
  }
  Core_String* path;
  if (Core_String_create(&path, "./assets/getFileContentsCallbackTests/helloworld.txt", sizeof("./assets/getFileContentsCallbackTests/helloworld.txt") - 1)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  if (Core_getFileContentsMemoryMappedCallback(path, &array, &callback)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  Core_Natural8 const expectedBytes[] = { 'H' , 'e' , 'l' , 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\r', '\n' };
  Core_Size const expectedNumberOfBytes = sizeof(expectedBytes);
  Core_Size numberOfBytes;
  if (Core_InlineArrayListN8_getSize(&numberOfBytes, &array)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  if (expectedNumberOfBytes != numberOfBytes) {
    Core_InlineArrayListN8_uninitialize(&array);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_Natural8* bytes;
  if (Core_InlineArrayListN8_getElements(&bytes, &array)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  Core_Natural8 compareResult;
  if (Core_Memory_compare(&compareResult, bytes, expectedBytes, expectedNumberOfBytes)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  if (compareResult != 0) {
    Core_InlineArrayListN8_uninitialize(&array);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_InlineArrayListN8_uninitialize(&array);
  return Core_Success;
}

static Core_Result Core_Tests_FileSystem_getFileContentsMemoryMappedCallbackTest2() {
  Core_InlineArrayListN8_Configuration arrayConfiguration = {
    .addedCallback = NULL,
    .removedCallback = NULL,
  };
  Core_InlineArrayListN8 array;
  if (Core_InlineArrayListN8_initialize(&array, 0, &arrayConfiguration)) {
    return Core_Failure;
  }
  Core_String* path;
  if (Core_String_create(&path, "./assets/getFileContentsCallbackTests/empty.txt", sizeof("./assets/getFileContentsCallbackTests/empty.txt") - 1)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  if (Core_getFileContentsMemoryMappedCallback(path, &array, &callback)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;
  Core_Natural8 const expectedBytes[] = { 0 };
  Core_Size const expectedNumberOfBytes = 0;
  Core_Size numberOfBytes;
  if (Core_InlineArrayListN8_getSize(&numberOfBytes, &array)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  if (expectedNumberOfBytes != numberOfBytes) {
    Core_InlineArrayListN8_uninitialize(&array);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_Natural8* bytes;
  if (Core_InlineArrayListN8_getElements(&bytes, &array)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  Core_Natural8 compareResult;
  if (Core_Memory_compare(&compareResult, bytes, expectedBytes, expectedNumberOfBytes)) {
    Core_InlineArrayListN8_uninitialize(&array);
    return Core_Failure;
  }
  if (compareResult != 0) {
    Core_InlineArrayListN8_uninitialize(&array);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_InlineArrayListN8_uninitialize(&array);
  return Core_Success;
}

Core_Result Core_Tests_FileSystem_getFileContentsMemoryMappedCallbackTests() {
  if (Core_Tests_FileSystem_getFileContentsMemoryMappedCallbackTest1()) {
    return Core_Failure;
  }
  if (Core_Tests_FileSystem_getFileContentsMemoryMappedCallbackTest2()) {
    return Core_Failure;
  }
  return Core_Success;
}
