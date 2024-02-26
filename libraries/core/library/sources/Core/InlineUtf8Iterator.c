#include "Core/InlineUtf8Iterator.h"

/// @brief Classify the first Byte of an UTF8 sequence to determine the length of the sequence.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param x The Byte.
/// @success <code>*RETURN</code> was assigned the length of the sequence.
/// @error Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error Core_Error_DecodingFailed @a x can not be classified.
static Core_Result classify(Core_Size* RETURN, Core_Natural8 x) {
  if ((x & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    *RETURN = 1;
    return Core_Success;
  } else if ((x & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    *RETURN = 2;
    return Core_Success;
  } else if ((x & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    *RETURN = 3;
    return Core_Success;
  } else if ((x & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    *RETURN = 3;
    return Core_Success;
  } else {
    Core_setError(Core_Error_DecodingFailed);
    return Core_Failure;
  }
}

Core_Result InlineUtf8Iterator_initialize(InlineUtf8Iterator* SELF, InlineByteIterator* backingIterator) {
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
  //
  return Core_Success;
}

Core_Result InlineUtf8Iterator_uninitialize(InlineUtf8Iterator* SELF) {
  return Core_Success;
}

Core_Result InlineUtf8Iterator_hasSymbol(Core_Boolean* RETURN, InlineUtf8Iterator* SELF) {
  *RETURN = SELF->hasSymbol;
  return Core_Success;
}

Core_Result InlineUtf8Iterator_hasError(Core_Boolean* RETURN, InlineUtf8Iterator* SELF) {
  *RETURN = SELF->hasError;
  return Core_Success;
}

Core_Result InlineUtf8Iterator_getSymbol(Core_Natural32* RETURN, InlineUtf8Iterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->symbol;
  return Core_Success;
}

Core_Result InlineUtf8Iterator_getSymbolLength(Core_Size* RETURN, InlineUtf8Iterator* SELF) {
  if (SELF->hasError || !SELF->hasSymbol) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->symbolLength;
  return Core_Success;
}

Core_Result InlineUtf8Iterator_getByteIndex(Core_Size* RETURN, InlineUtf8Iterator* SELF) {
  *RETURN = SELF->byteIndex;
  return Core_Success;
}

Core_Result InlineUtf8Iterator_getSymbolIndex(Core_Size* RETURN, InlineUtf8Iterator* SELF) {
  *RETURN = SELF->symbolIndex;
  return Core_Success;
}

Core_Result InlineUtf8Iterator_next(InlineUtf8Iterator* SELF) {
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
