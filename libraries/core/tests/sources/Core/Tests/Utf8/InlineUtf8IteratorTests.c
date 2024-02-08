#include "Core/Tests/Utf8/InlineUtf8IteratorTests.h"

#include <string.h>

static Core_Result byteIteratorTest(char const* bytes, Core_Size numberOfBytes) {
  InlineByteIterator_C byteIterator;
  if (InlineByteIterator_C_initialize(&byteIterator, bytes, strlen(bytes))) {
    return Core_Failure;
  }
  Core_Size receivedBytes = 0;
  Core_Boolean hasValue;
  if (InlineByteIterator_hasValue(&hasValue, (InlineByteIterator*)&byteIterator)) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  while (hasValue) {
    Core_Natural8 value;
    if (InlineByteIterator_getValue(&value, (InlineByteIterator*)&byteIterator)) {
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    if (InlineByteIterator_next((InlineByteIterator*)&byteIterator)) {
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    if (InlineByteIterator_hasValue(&hasValue, (InlineByteIterator*)&byteIterator)) {
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    receivedBytes++;
  }
  if (numberOfBytes != receivedBytes) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}

static Core_Result byteIteratorTest1() {
  char const* bytes = "";
  if (byteIteratorTest(bytes, strlen(bytes))) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result byteIteratorTest2() {
  char const* bytes = "Hello, World!";
  if (byteIteratorTest(bytes, strlen(bytes))) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result utf8IteratorTest(char const *bytes, Core_Size numberOfBytes, Core_Size expectedSymbols) {
  InlineByteIterator_C byteIterator;
  if (InlineByteIterator_C_initialize(&byteIterator, bytes, numberOfBytes)) {
    return Core_Failure;
  }
  InlineUtf8Iterator utf8Iterator;
  if (InlineUtf8Iterator_initialize(&utf8Iterator, (InlineByteIterator*)&byteIterator)) {
    return Core_Failure;
  }
  Core_Size receivedSymbols = 0;
  Core_Boolean hasSymbol;
  if (InlineUtf8Iterator_hasSymbol(&hasSymbol, &utf8Iterator)) {
    InlineUtf8Iterator_uninitialize(&utf8Iterator);
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  while (hasSymbol) {
    Core_Natural32 value;
    if (InlineUtf8Iterator_getSymbol(&value, &utf8Iterator)) {
      InlineUtf8Iterator_uninitialize(&utf8Iterator);
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    if (InlineUtf8Iterator_next(&utf8Iterator)) {
      InlineUtf8Iterator_uninitialize(&utf8Iterator);
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    if (InlineUtf8Iterator_hasSymbol(&hasSymbol, &utf8Iterator)) {
      InlineUtf8Iterator_uninitialize(&utf8Iterator);
      InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
      return Core_Failure;
    }
    receivedSymbols++;
  }
  if (expectedSymbols != receivedSymbols) {
    InlineUtf8Iterator_uninitialize(&utf8Iterator);
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  InlineUtf8Iterator_uninitialize(&utf8Iterator);
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

#include "dx/core/_is_utf8_sequence.h"

static Core_Result utf8IteratorTest2() {
  char const* bytes = "Hello, World!";
  if (utf8IteratorTest(bytes, strlen(bytes), strlen(bytes))) {
    return Core_Failure;
  }
  _utf8_symbol_range source = { .start = 0, .length = strlen(bytes) };
  _utf8_byte_range target = { .start = 0, .length = 0 };
  if (_utf8_symbol_range_to_byte_range(bytes, strlen(bytes), &source, &target)) {
    return Core_Failure;
  }
  if (source.start != target.start || source.length != target.length) {
    Core_setError(Core_Error_TestFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Tests_Utf8_inlineUtf8IteratorTests() {
  if (byteIteratorTest1()) {
    return Core_Failure;
  }
  if (byteIteratorTest2()) {
    return Core_Failure;
  }
  if (utf8IteratorTest1()) {
    return Core_Failure;
  }
  if (utf8IteratorTest2()) {
    return Core_Failure;
  }
  return Core_Success;
}