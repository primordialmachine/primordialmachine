#include "dx/core/_is_utf8_sequence.h"

static Core_Result _utf8_symbol_range_to_byte_range_ex(Core_Utf8_Iterator* utf8Iterator, _utf8_symbol_range* source, _utf8_byte_range* target) {
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
    if (Core_Utf8_Iterator_hasSymbol(&hasSymbol, utf8Iterator)) {
      return Core_Failure;
    }
    if (!hasSymbol) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
    if (Core_Utf8_Iterator_next(utf8Iterator)) {
      return Core_Failure;
    }
    currentSymbolRange.start++;
  }
  Core_Utf8_Iterator_getByteIndex(&currentByteRange.start, utf8Iterator);
  //
  while (currentSymbolRange.length < source->length) {
    if (Core_Utf8_Iterator_hasSymbol(&hasSymbol, utf8Iterator)) {
      return Core_Failure;
    }
    if (!hasSymbol) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
    if (Core_Utf8_Iterator_next(utf8Iterator)) {
      return Core_Failure;
    }
    currentSymbolRange.length++;
  }
  Core_Utf8_Iterator_getByteIndex(&currentByteRange.length, utf8Iterator);
  currentByteRange.length -= currentByteRange.start;
  *target = currentByteRange;
  return Core_Success;
}

Core_Result _utf8_symbol_range_to_byte_range(Core_Natural8 const* p, Core_Size n, _utf8_symbol_range* source, _utf8_byte_range* target) {
  if (!p || !source || !target) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  InlineByteIteratorCxxArray byteIterator;
  if (InlineByteIteratorCxxArray_initialize(&byteIterator, p, n)) {
    return Core_Failure;
  }
  Core_Utf8_DefaultIterator _utf8Iterator;
  Core_Utf8_Iterator* utf8Iterator = (Core_Utf8_Iterator*)&_utf8Iterator;
  if (Core_Utf8_DefaultIterator_initialize(&_utf8Iterator, (InlineByteIterator*)&byteIterator)) {
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  if (_utf8_symbol_range_to_byte_range_ex(utf8Iterator, source, target)) {
    Core_Utf8_Iterator_uninitialize(utf8Iterator);
    InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
    return Core_Failure;
  }
  Core_Utf8_Iterator_uninitialize(utf8Iterator);
  InlineByteIterator_uninitialize((InlineByteIterator*)&byteIterator);
  return Core_Success;
}
