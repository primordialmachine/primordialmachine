#include "Core/Tests/Utf8/InlineByteIteratorTests.h"

#include <string.h>

static Core_Result byteIteratorTest(InlineByteIterator* byteIterator, char const*expectedBytes, Core_Size numberOfExpectedBytes) {
  Core_Size indexExpectedBytes = 0;
  Core_Size receivedBytes = 0;
  Core_Boolean hasValue;
  if (InlineByteIterator_hasValue(&hasValue, byteIterator)) {
    return Core_Failure;
  }
  while (hasValue) {
    Core_Natural8 value;
    if (InlineByteIterator_getValue(&value, byteIterator)) {
      return Core_Failure;
    }
    if (value != expectedBytes[indexExpectedBytes]) {
      Core_setError(Core_Error_TestFailed);
      return Core_Failure;
    }
    indexExpectedBytes++;
    if (InlineByteIterator_next(byteIterator)) {
      return Core_Failure;
    }
    if (InlineByteIterator_hasValue(&hasValue, byteIterator)) {
      return Core_Failure;
    }
    receivedBytes++;
  }
  if (numberOfExpectedBytes != receivedBytes) {
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result byteIteratorTest1() {
  char const* bytes = "";
  char const* expectedBytes = "";
  InlineByteIteratorCxxArray byteIterator;
  if (InlineByteIteratorCxxArray_initialize(&byteIterator, bytes, strlen(bytes))) {
    return Core_Failure;
  }
  if (byteIteratorTest((InlineByteIterator*)&byteIterator, expectedBytes, strlen(expectedBytes))) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}

static Core_Result byteIteratorTest2() {
  char const* bytes = "Hello, World!";
  char const* expectedBytes = "Hello, World!";
  InlineByteIteratorCxxArray byteIterator;
  if (InlineByteIteratorCxxArray_initialize(&byteIterator, bytes, strlen(bytes))) {
    return Core_Failure;
  }
  if (byteIteratorTest((InlineByteIterator*)&byteIterator, expectedBytes, strlen(expectedBytes))) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}

static Core_Result byteIteratorReverseTest1() {
  char const* bytes = "";
  char const* expectedBytes = "";
  InlineByteIteratorCxxArrayReverse byteIterator;
  if (InlineByteIteratorCxxArrayReverse_initialize(&byteIterator, bytes, strlen(bytes))) {
    return Core_Failure;
  }
  if (byteIteratorTest((InlineByteIterator*)&byteIterator, expectedBytes, strlen(expectedBytes))) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}

static Core_Result byteIteratorReverseTest2() {
  char const* bytes = "Hello, World!";
  char const* expectedBytes = "!dlroW ,olleH";
  InlineByteIteratorCxxArrayReverse byteIterator;
  if (InlineByteIteratorCxxArrayReverse_initialize(&byteIterator, bytes, strlen(bytes))) {
    return Core_Failure;
  }
  if (byteIteratorTest((InlineByteIterator*)&byteIterator, expectedBytes, strlen(expectedBytes))) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}


Core_Result Core_Tests_Utf8_inlineByteIteratorTests() {
  if (byteIteratorTest1()) {
    return Core_Failure;
  }
  if (byteIteratorTest2()) {
    return Core_Failure;
  }
  if (byteIteratorReverseTest1()) {
    return Core_Failure;
}
  if (byteIteratorReverseTest2()) {
    return Core_Failure;
  }
  return Core_Success;
}
