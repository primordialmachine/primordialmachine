#if !defined(CORE_UTF8_DEFAULTREVERSEITERATOR_H_INCLUDED)
#define CORE_UTF8_DEFAULTREVERSEITERATOR_H_INCLUDED

#include "Core/Utf8/Iterator.h"

// A validating UTF-8 iterator.
typedef struct Core_Utf8_DefaultReverseIterator {
  Core_Utf8_Iterator _parent;
  InlineByteIterator* backingIterator;
  Core_Boolean hasError;
  Core_Boolean hasSymbol;
  Core_Size byteIndex;
  Core_Size symbolIndex;
  Core_Natural32 symbol;
  Core_Size symbolLength;
} Core_Utf8_DefaultReverseIterator;

Core_Result Core_Utf8_DefaultReverseIterator_initialize(Core_Utf8_DefaultReverseIterator* SELF, InlineByteIterator* backingIterator);

Core_Result Core_Utf8_DefaultReverseIterator_uninitialize(Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_hasError(Core_Boolean* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getByteIndex(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_next(Core_Utf8_DefaultReverseIterator* SELF);

#endif // CORE_UTF8_DEFAULTREVERSEITERATOR_H_INCLUDED
