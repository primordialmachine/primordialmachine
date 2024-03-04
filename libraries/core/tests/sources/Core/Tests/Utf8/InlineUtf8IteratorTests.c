#include "Core/Tests/Utf8/InlineUtf8IteratorTests.h"

#include <string.h>

static Core_Result utf8IteratorTest(char const *bytes, Core_Size numberOfBytes, Core_Size expectedSymbols) {
  InlineByteIteratorCxxArray byteIterator;
  if (InlineByteIteratorCxxArray_initialize(&byteIterator, bytes, numberOfBytes)) {
    return Core_Failure;
  }
  Core_Utf8_DefaultIterator _utf8Iterator;
  Core_Utf8_Iterator* utf8Iterator = (Core_Utf8_Iterator*)&_utf8Iterator;
  if (Core_Utf8_DefaultIterator_initialize(&_utf8Iterator, (InlineByteIterator*)&byteIterator)) {
    return Core_Failure;
  }
  Core_Size receivedSymbols = 0;
  Core_Boolean hasSymbol;
  if (Core_Utf8_Iterator_hasSymbol(&hasSymbol, utf8Iterator)) {
    Core_Utf8_Iterator_uninitialize(utf8Iterator);
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  while (hasSymbol) {
    Core_Natural32 value;
    if (Core_Utf8_Iterator_getSymbol(&value, utf8Iterator)) {
      Core_Utf8_Iterator_uninitialize(utf8Iterator);
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    if (Core_Utf8_Iterator_next(utf8Iterator)) {
      Core_Utf8_Iterator_uninitialize(utf8Iterator);
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    if (Core_Utf8_Iterator_hasSymbol(&hasSymbol, utf8Iterator)) {
      Core_Utf8_Iterator_uninitialize(utf8Iterator);
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    receivedSymbols++;
  }
  if (expectedSymbols != receivedSymbols) {
    Core_Utf8_Iterator_uninitialize(utf8Iterator);
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  Core_Utf8_Iterator_uninitialize(utf8Iterator);
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}

static Core_Result utf8IteratorTest1() {
  char const* bytes = "";
  if (utf8IteratorTest(bytes, strlen(bytes), strlen(bytes))) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result utf8IteratorTest2() {
  char const* bytes = "Hello, World!";
  if (utf8IteratorTest(bytes, strlen(bytes), strlen(bytes))) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Tests_Utf8_inlineUtf8IteratorTests() {
  if (utf8IteratorTest1()) {
    return Core_Failure;
  }
  if (utf8IteratorTest2()) {
    return Core_Failure;
  }
  return Core_Success;
}