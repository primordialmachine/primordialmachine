#include "dx/core/_is_utf8_sequence.h"

static Core_Result _utf8_symbol_range_to_byte_range_ex(InlineUtf8Iterator* utf8Iterator, _utf8_symbol_range* source, _utf8_byte_range* target) {
  if (!utf8Iterator || !source || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _utf8_symbol_range currentSymbolRange = {
    .start = 0,
    .length = 0
  };
  _utf8_byte_range currentByteRange = {
    .start = 0,
    .length = 0
  };
  Core_Boolean hasSymbol;
  //
  while (currentSymbolRange.start < source->start) {
    if (InlineUtf8Iterator_hasSymbol(&hasSymbol, utf8Iterator)) {
      return Core_Failure;
    }
    if (!hasSymbol) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
    if (InlineUtf8Iterator_next(utf8Iterator)) {
      return Core_Failure;
    }
    currentSymbolRange.start++;
  }
  InlineUtf8Iterator_getByteIndex(&currentByteRange.start, utf8Iterator);
  //
  while (currentSymbolRange.length < source->length) {
    if (InlineUtf8Iterator_hasSymbol(&hasSymbol, utf8Iterator)) {
      return Core_Failure;
    }
    if (!hasSymbol) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
    if (InlineUtf8Iterator_next(utf8Iterator)) {
      return Core_Failure;
    }
    currentSymbolRange.length++;
  }
  InlineUtf8Iterator_getByteIndex(&currentByteRange.length, utf8Iterator);
  currentByteRange.length -= currentByteRange.start;
  *target = currentByteRange;
  return Core_Success;
}

Core_Result _utf8_symbol_range_to_byte_range(Core_Natural8 const* p, Core_Size n, _utf8_symbol_range* source, _utf8_byte_range* target) {
  if (!p || !source || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  InlineByteIterator_C byteIterator;
  if (InlineByteIterator_C_initialize(&byteIterator, p, n)) {
    return Core_Failure;
  }
  InlineUtf8Iterator utf8Iterator;
  if (InlineUtf8Iterator_initialize(&utf8Iterator, (InlineByteIterator*)&byteIterator)) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  if (_utf8_symbol_range_to_byte_range_ex(&utf8Iterator, source, target)) {
    InlineUtf8Iterator_uninitialize(&utf8Iterator);
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  InlineUtf8Iterator_uninitialize(&utf8Iterator);
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}
