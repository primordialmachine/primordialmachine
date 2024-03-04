#if !defined(CORE_UTF8_ITERATOR_H_INCLUDED)
#define CORE_UTF8_ITERATOR_H_INCLUDED

#include "Core/InlineByteIterator.h"
#include "Core/Memory.h"

// A validating UTF-8 iterator.
typedef struct Core_Utf8_Iterator Core_Utf8_Iterator;

struct Core_Utf8_Iterator {
  InlineByteIterator* backingIterator;
  Core_Result(*uninitialize)(Core_Utf8_Iterator*);
  Core_Result(*hasSymbol)(Core_Boolean*, Core_Utf8_Iterator*);
  Core_Result(*hasError)(Core_Boolean*, Core_Utf8_Iterator*);
  Core_Result(*getSymbol)(Core_Natural32* RETURN, Core_Utf8_Iterator*);
  Core_Result(*getSymbolLength)(Core_Size* RETURN, Core_Utf8_Iterator*);
  Core_Result(*getByteIndex)(Core_Size* RETURN, Core_Utf8_Iterator*);
  Core_Result(*getSymbolIndex)(Core_Size* RETURN, Core_Utf8_Iterator*);
  Core_Result(*next)(Core_Utf8_Iterator*);
};

static inline Core_Result Core_Utf8_Iterator_initialize(Core_Utf8_Iterator* SELF, InlineByteIterator* backingIterator) {
  Core_Memory_fill(SELF, sizeof(Core_Utf8_Iterator), 0);
  SELF->backingIterator = backingIterator;
  return Core_Success;
}

static inline Core_Result Core_Utf8_Iterator_uninitialize(Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->uninitialize(SELF);
}

// Get if the iterator has a symbol to get.
// Returns false if there is an error or the end of the input was reached.
static inline Core_Result Core_Utf8_Iterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->hasSymbol(RETURN, SELF);
}

// Get if the iterator encountered an error.
// There are two kinds of errors:
// - the first Byte is ill-formatted
// - the second, third, or fourth Byte are ill-formatted
// - the second, third, or fourth Byte is not available
static inline Core_Result  Core_Utf8_Iterator_hasError(Core_Boolean* RETURN, Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->hasError(RETURN, SELF);
}

// Get the symbol.
// Core_Error_OperationInvalid there is an error or the end of the input was reached
static inline Core_Result Core_Utf8_Iterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->getSymbol(RETURN, SELF);
}

// Get if the symbol consists of one, two, three, or four Bytes.
// Core_Error_OperationInvalid there is an error or the end of the input was reached
static inline Core_Result Core_Utf8_Iterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->getSymbolLength(RETURN, SELF);
}

// Get the index of the current value.
static inline Core_Result Core_Utf8_Iterator_getByteIndex(Core_Size* RETURN, Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->getByteIndex(RETURN, SELF);
}

// Get the index of the current symbol.
static inline Core_Result Core_Utf8_Iterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->getSymbolIndex(RETURN, SELF);
}

// no op if iterator has error or has no value
static inline Core_Result Core_Utf8_Iterator_next(Core_Utf8_Iterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return SELF->next(SELF);
}

#endif // CORE_UTF8_ITERATOR_H_INCLUDED
