#if !defined(CORE_UTF8_DEFAULTITERATOR_H_INCLUDED)
#define CORE_UTF8_DEFAULTITERATOR_H_INCLUDED

#include "Core/Utf8/Iterator.h"

// A validating UTF-8 iterator.
typedef struct Core_Utf8_DefaultIterator {
  Core_Utf8_Iterator _parent;
  InlineByteIterator* backingIterator;
  Core_Boolean hasError;
  Core_Boolean hasSymbol;
  Core_Size byteIndex;
  Core_Size symbolIndex;
  Core_Natural32 symbol;
  Core_Size symbolLength;
} Core_Utf8_DefaultIterator;

Core_Result Core_Utf8_DefaultIterator_initialize(Core_Utf8_DefaultIterator* SELF, InlineByteIterator* backingIterator);

Core_Result Core_Utf8_DefaultIterator_uninitialize(Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_hasError(Core_Boolean* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getByteIndex(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_next(Core_Utf8_DefaultIterator* SELF);

#endif // CORE_UTF8_UTF8ITERATORIMPL_H_INCLUDED
