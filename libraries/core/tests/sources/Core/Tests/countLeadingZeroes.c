#include "Core/Tests/countLeadingZeroes.h"

#define EXPECT(SUFFIX, TYPE, VALUE, EXPECTED) \
  { \
    Core_Size expected = EXPECTED; \
    Core_Size received = 0; \
    TYPE value = VALUE; \
    if (Core_countLeadingZeroes##SUFFIX(&received, value)) { \
      return Core_Failure; \
    } \
    if (received != expected) { \
      return Core_Failure; \
    } \
  }


Core_Result Core_Tests_countLeadingZeroesTests() {
  EXPECT(N64, Core_Natural64, 0, 56 + 8);
  EXPECT(N64, Core_Natural64, 1, 56 + 7);
  EXPECT(N64, Core_Natural64, 2, 56 + 6);
  EXPECT(N64, Core_Natural64, 3, 56 + 6);
  EXPECT(N64, Core_Natural64, 4, 56 + 5);
  EXPECT(N64, Core_Natural64, Core_Natural64_Greatest, 0);

  EXPECT(N32, Core_Natural32, 0, 24 + 8);
  EXPECT(N32, Core_Natural32, 1, 24 + 7);
  EXPECT(N32, Core_Natural32, 2, 24 + 6);
  EXPECT(N32, Core_Natural32, 3, 24 + 6);
  EXPECT(N32, Core_Natural32, 4, 24 + 5);
  EXPECT(N32, Core_Natural32, Core_Natural32_Greatest, 0);

  EXPECT(N16, Core_Natural16, 0, 8 + 8);
  EXPECT(N16, Core_Natural16, 1, 8 + 7);
  EXPECT(N16, Core_Natural16, 2, 8 + 6);
  EXPECT(N16, Core_Natural16, 3, 8 + 6);
  EXPECT(N16, Core_Natural16, 4, 8 + 5);
  EXPECT(N16, Core_Natural32, Core_Natural16_Greatest, 0);

  EXPECT(N8, Core_Natural8, 0, 8);
  EXPECT(N8, Core_Natural8, 1, 7);
  EXPECT(N8, Core_Natural8, 2, 6);
  EXPECT(N8, Core_Natural8, 3, 6);
  EXPECT(N8, Core_Natural8, 4, 5);
  EXPECT(N8, Core_Natural8, Core_Natural8_Greatest, 0);

  return Core_Success;
}