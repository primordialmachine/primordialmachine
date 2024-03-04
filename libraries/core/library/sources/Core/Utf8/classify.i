
/// @brief Classify the first Byte of an UTF8 sequence to determine the length of the sequence.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param x The Byte.
/// @success <code>*RETURN</code> was assigned the length of the sequence.
/// @error Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error Core_Error_DecodingFailed @a x can not be classified.
static inline Core_Result classify(Core_Size* RETURN, Core_Natural8 x) {
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
    *RETURN = 4;
    return Core_Success;
  } else {
    Core_setError(Core_Error_DecodingFailed);
    return Core_Failure;
  }
}
