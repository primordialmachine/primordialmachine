#if !defined(CORE_INLINEUTF8ITERATOR_H_INCLUDED)
#define CORE_INLINEUTF8ITERATOR_H_INCLUDED

#include "Core/InlineByteIterator.h"

// A validating UTF-8 iterator.
typedef struct InlineUtf8Iterator {
  InlineByteIterator* backingIterator;
  Core_Boolean hasError;
  Core_Boolean hasSymbol;
  Core_Size byteIndex;
  Core_Size symbolIndex;
  Core_Natural32 symbol;
  Core_Size symbolLength;
} InlineUtf8Iterator;

Core_Result InlineUtf8Iterator_initialize(InlineUtf8Iterator* SELF, InlineByteIterator* backingIterator);

Core_Result InlineUtf8Iterator_uninitialize(InlineUtf8Iterator* SELF);

// Get if the iterator has a symbol to get.
// Returns false if there is an error or the end of the input was reached.
Core_Result InlineUtf8Iterator_hasSymbol(Core_Boolean* RETURN, InlineUtf8Iterator* SELF);

// Get if the iterator encountered an error.
// There are two kinds of errors:
// - the first Byte is ill-formatted
// - the second, third, or fourth Byte are ill-formatted
// - the second, third, or fourth Byte is not available
Core_Result InlineUtf8Iterator_hasError(Core_Boolean* RETURN, InlineUtf8Iterator* SELF);

// Get the symbol.
// Core_Error_OperationInvalid there is an error or the end of the input was reached
Core_Result InlineUtf8Iterator_getSymbol(Core_Natural32* RETURN, InlineUtf8Iterator* SELF);

// Get if the symbol consists of one, two, three, or four Bytes.
// Core_Error_OperationInvalid there is an error or the end of the input was reached
Core_Result InlineUtf8Iterator_getSymbolLength(Core_Size* RETURN, InlineUtf8Iterator* SELF);

// Get the index of the current value.
Core_Result InlineUtf8Iterator_getByteIndex(Core_Size* RETURN, InlineUtf8Iterator* SELF);

// Get the index of the current symbol.
Core_Result InlineUtf8Iterator_getSymbolIndex(Core_Size* RETURN, InlineUtf8Iterator* SELF);

// no op if iterator has error or has no value
Core_Result InlineUtf8Iterator_next(InlineUtf8Iterator* SELF);

#endif // CORE_INLINEUTF8ITERATOR_H_INCLUDED
