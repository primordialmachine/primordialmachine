#include "Core/Utf8/DefaultReverseIterator.h"

#include "Core/Utf8/classify.i"

Core_Result Core_Utf8_DefaultReverseIterator_initialize(Core_Utf8_DefaultReverseIterator* SELF, InlineByteIterator* backingIterator);

Core_Result Core_Utf8_DefaultReverseIterator_uninitialize(Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_hasError(Core_Boolean* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getByteIndex(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_next(Core_Utf8_DefaultReverseIterator* SELF);

Core_Result Core_Utf8_DefaultReverseIterator_initialize(Core_Utf8_DefaultReverseIterator* SELF, InlineByteIterator* backingIterator) {
  if (Core_Utf8_Iterator_initialize((Core_Utf8_Iterator*)SELF, backingIterator)) {
    return Core_Failure;
  }
  ((Core_Utf8_Iterator*)SELF)->hasError = (Core_Result(*)(Core_Boolean*, Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_hasError;
  ((Core_Utf8_Iterator*)SELF)->hasSymbol = (Core_Result(*)(Core_Boolean*, Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_hasSymbol;
  ((Core_Utf8_Iterator*)SELF)->getByteIndex = (Core_Result(*)(Core_Size*, Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_getByteIndex;
  ((Core_Utf8_Iterator*)SELF)->getSymbol = (Core_Result(*)(Core_Natural32*, Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_getSymbol;
  ((Core_Utf8_Iterator*)SELF)->getSymbolIndex = (Core_Result(*)(Core_Size*, Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_getSymbolIndex;
  ((Core_Utf8_Iterator*)SELF)->getSymbolLength = (Core_Result(*)(Core_Size*, Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_getSymbolLength;
  ((Core_Utf8_Iterator*)SELF)->next = (Core_Result(*)(Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_next;
  ((Core_Utf8_Iterator*)SELF)->uninitialize = (Core_Result(*)(Core_Utf8_Iterator*))Core_Utf8_DefaultReverseIterator_uninitialize;

  SELF->backingIterator = backingIterator;
  SELF->hasSymbol = Core_False;
  SELF->symbol = 0;
  SELF->symbolLength = 0;
  SELF->hasError = Core_False;
  SELF->byteIndex = 0;
  SELF->symbolIndex = 0;

  Core_Natural8 byteValue;
  Core_Boolean hasByteValue;
  // if there is no Byte, we are done.
  if (InlineByteIterator_hasValue(&hasByteValue, SELF->backingIterator)) {
    Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
    return Core_Failure;
  }
  if (!hasByteValue) {
    SELF->hasSymbol = Core_False;
    return Core_Success;
  }
  // Otherwise parse the Bytes.
  SELF->hasSymbol = Core_True;
  if (InlineByteIterator_getValue(&byteValue, SELF->backingIterator)) {
    Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
    return Core_Failure;
  }
  SELF->symbol = byteValue;
  SELF->symbolLength = 1;
  while ((byteValue & 0xC0) == 0x80 && SELF->symbolLength < 4) {
    if (InlineByteIterator_hasValue(&hasByteValue, SELF->backingIterator)) {
      Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
      return Core_Failure;
    }
    if (!hasByteValue) {
      SELF->hasSymbol = Core_False;
      return Core_Success;
    }
    if (InlineByteIterator_getValue(&byteValue, SELF->backingIterator)) {
      Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
      return Core_Failure;
    }
    SELF->symbol |= byteValue << (SELF->symbolLength * 8);
    SELF->symbolLength++;
  }
  Core_Size n;
  if (classify(&n, byteValue)) {
    SELF->hasSymbol = Core_False;
    return Core_Success;
  }
  if (n != SELF->symbolLength) {
    SELF->hasSymbol = Core_False;
    return Core_Success;
  }
  SELF->hasSymbol = Core_True;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_uninitialize(Core_Utf8_DefaultReverseIterator* SELF) {
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_DefaultReverseIterator* SELF) {
  *RETURN = SELF->hasSymbol;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_hasError(Core_Boolean* RETURN, Core_Utf8_DefaultReverseIterator* SELF) {
  *RETURN = SELF->hasError;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_DefaultReverseIterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->symbol;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->symbolLength;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_getByteIndex(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF) {
  *RETURN = SELF->byteIndex;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_DefaultReverseIterator* SELF) {
  *RETURN = SELF->symbolIndex;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultReverseIterator_next(Core_Utf8_DefaultReverseIterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    return Core_Success;
  }
  SELF->symbolIndex++;
  if (InlineByteIterator_getIndex(&SELF->byteIndex, SELF->backingIterator)) {
    return Core_Failure;
  }
  Core_Natural8 byteValue;
  Core_Boolean hasByteValue;
  // if there is no Byte, we are done.
  if (InlineByteIterator_hasValue(&hasByteValue, SELF->backingIterator)) {
    return Core_Failure;
  }
  if (!hasByteValue) {
    SELF->hasSymbol = Core_False;
    return Core_Success;
  }
  // Otherwise parse the Bytes.
  SELF->hasSymbol = Core_True;
  if (InlineByteIterator_getValue(&byteValue, SELF->backingIterator)) {
    return Core_Failure;
  }
  SELF->symbol = byteValue;
  SELF->symbolLength = 1;
  while ((byteValue & 0xC0) == 0x80 && SELF->symbolLength < 4) {
    if (InlineByteIterator_hasValue(&hasByteValue, SELF->backingIterator)) {
      return Core_Failure;
    }
    if (!hasByteValue) {
      SELF->hasSymbol = Core_False;
      return Core_Success;
    }
    if (InlineByteIterator_getValue(&byteValue, SELF->backingIterator)) {
      return Core_Failure;
    }
    SELF->symbol |= byteValue << (SELF->symbolLength * 8);
    SELF->symbolLength++;
  }
  Core_Size n;
  if (classify(&n, byteValue)) {
    SELF->hasSymbol = Core_False;
    return Core_Success;
  }
  if (n != SELF->symbolLength) {
    SELF->hasSymbol = Core_False;
    return Core_Success;
  }
  SELF->hasSymbol = Core_True;
  return Core_Success;
}
