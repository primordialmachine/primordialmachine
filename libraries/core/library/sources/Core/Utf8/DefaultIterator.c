#include "Core/Utf8/DefaultIterator.h"

#include "Core/Utf8/classify.i"

Core_Result Core_Utf8_DefaultIterator_initialize(Core_Utf8_DefaultIterator* SELF, InlineByteIterator* backingIterator);

Core_Result Core_Utf8_DefaultIterator_uninitialize(Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_hasError(Core_Boolean* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getByteIndex(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_next(Core_Utf8_DefaultIterator* SELF);

Core_Result Core_Utf8_DefaultIterator_initialize(Core_Utf8_DefaultIterator* SELF, InlineByteIterator* backingIterator) {
  if (Core_Utf8_Iterator_initialize((Core_Utf8_Iterator*)SELF, backingIterator)) {
    return Core_Failure;
  }
  ((Core_Utf8_Iterator*)SELF)->hasError = (Core_Result(*)(Core_Boolean*, Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_hasError;
  ((Core_Utf8_Iterator*)SELF)->hasSymbol = (Core_Result(*)(Core_Boolean*, Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_hasSymbol;
  ((Core_Utf8_Iterator*)SELF)->getByteIndex = (Core_Result(*)(Core_Size*, Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_getByteIndex;
  ((Core_Utf8_Iterator*)SELF)->getSymbol = (Core_Result(*)(Core_Natural32*, Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_getSymbol;
  ((Core_Utf8_Iterator*)SELF)->getSymbolIndex = (Core_Result(*)(Core_Size*, Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_getSymbolIndex;
  ((Core_Utf8_Iterator*)SELF)->getSymbolLength = (Core_Result(*)(Core_Size*, Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_getSymbolLength;
  ((Core_Utf8_Iterator*)SELF)->next = (Core_Result(*)(Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_next;
  ((Core_Utf8_Iterator*)SELF)->uninitialize = (Core_Result(*)(Core_Utf8_Iterator*))Core_Utf8_DefaultIterator_uninitialize;

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
  if (classify(&SELF->symbolLength, byteValue)) {
    Core_setError(Core_Error_NoError);
    SELF->hasError = Core_True;
    return Core_Success;
  }
  if (InlineByteIterator_next(SELF->backingIterator)) {
    Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = SELF->symbolLength - 1; i < n; ++i) {
    if (InlineByteIterator_hasValue(&hasByteValue, SELF->backingIterator)) {
      Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
      return Core_Failure;
    }
    if (!hasByteValue) {
      SELF->hasError = Core_True;
      return Core_Success;
    }
    if (InlineByteIterator_getValue(&byteValue, SELF->backingIterator)) {
      return Core_Failure;
    }
    if ((byteValue & 0xC0) != 0x80) {
      SELF->hasError = Core_True;
      return Core_Success;
    }
    SELF->symbol |= byteValue << i;
    if (InlineByteIterator_next(SELF->backingIterator)) {
      Core_Utf8_Iterator_uninitialize((Core_Utf8_Iterator*)SELF);
      return Core_Failure;
    }
  }
  //
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_uninitialize(Core_Utf8_DefaultIterator* SELF) {
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_hasSymbol(Core_Boolean* RETURN, Core_Utf8_DefaultIterator* SELF) {
  *RETURN = SELF->hasSymbol;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_hasError(Core_Boolean* RETURN, Core_Utf8_DefaultIterator* SELF) {
  *RETURN = SELF->hasError;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_getSymbol(Core_Natural32* RETURN, Core_Utf8_DefaultIterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->symbol;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_getSymbolLength(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->symbolLength;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_getByteIndex(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF) {
  *RETURN = SELF->byteIndex;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_getSymbolIndex(Core_Size* RETURN, Core_Utf8_DefaultIterator* SELF) {
  *RETURN = SELF->symbolIndex;
  return Core_Success;
}

Core_Result Core_Utf8_DefaultIterator_next(Core_Utf8_DefaultIterator* SELF) {
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
  if (classify(&SELF->symbolLength, byteValue)) {
    Core_setError(Core_Error_NoError);
    SELF->hasError = Core_True;
    return Core_Success;
  }
  if (InlineByteIterator_next(SELF->backingIterator)) {
    return Core_Failure;
  }
  for (Core_Size i = 0, n = SELF->symbolLength - 1; i < n; ++i) {
    if (InlineByteIterator_hasValue(&hasByteValue, SELF->backingIterator)) {
      return Core_Failure;
    }
    if (!hasByteValue) {
      SELF->hasError = Core_True;
      return Core_Success;
    }
    if (InlineByteIterator_getValue(&byteValue, SELF->backingIterator)) {
      return Core_Failure;
    }
    if ((byteValue & 0xC0) != 0x80) {
      SELF->hasError = Core_True;
      return Core_Success;
    }
    SELF->symbol |= byteValue << i;
    if (InlineByteIterator_next(SELF->backingIterator)) {
      return Core_Failure;
    }
  }
  SELF->hasSymbol = Core_True;
  return Core_Success;
}
