#include "Core/Tests/safeAddNx.h"


static Core_Result Core_Tests_safeAddN8Tests() {
  Core_Natural8 u, v;

  // 0 + 0 = (0, 0)
  Core_safeAddN8(&u, 0, 0, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 0 = (MAX, 0)
  Core_safeAddN8(&u, Core_Natural8_Greatest, 0, &v);
  if (u != Core_Natural8_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // 0 + MAX = (MAX, 0)
  Core_safeAddN8(&u, 0, Core_Natural8_Greatest, &v);
  if (u != Core_Natural8_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 1 = (0, 1)
  Core_safeAddN8(&u, Core_Natural8_Greatest, 1, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 1 + MAX = (0, 1)
  Core_safeAddN8(&u, 1, Core_Natural8_Greatest, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 2 = (1, 1)
  Core_safeAddN8(&u, Core_Natural8_Greatest, 2, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 2 + MAX = (1, 1)
  Core_safeAddN8(&u, 2, Core_Natural8_Greatest, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 37 = (36, 1)
  Core_safeAddN8(&u, Core_Natural8_Greatest, 37, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 37 + MAX = (36, 1)
  Core_safeAddN8(&u, 37, Core_Natural8_Greatest, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1,1)
  Core_safeAddN8(&u, Core_Natural8_Greatest, Core_Natural8_Greatest, &v);
  if (u != Core_Natural8_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX+MAX=(MAX-1,1)
  Core_safeAddN8(&u, Core_Natural8_Greatest, Core_Natural8_Greatest, &v);
  if (u != Core_Natural8_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }

  return Core_Success;
}

static Core_Result Core_Tests_safeAddN16Tests() {
  Core_Natural16 u, v;

  // 0 + 0 = (0, 0)
  Core_safeAddN16(&u, 0, 0, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 0 = (MAX, 0)
  Core_safeAddN16(&u, Core_Natural16_Greatest, 0, &v);
  if (u != Core_Natural16_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // 0 + MAX = (MAX, 0)
  Core_safeAddN16(&u, 0, Core_Natural16_Greatest, &v);
  if (u != Core_Natural16_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 1 = (0, 1)
  Core_safeAddN16(&u, Core_Natural16_Greatest, 1, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 1 + MAX = (0, 1)
  Core_safeAddN16(&u, 1, Core_Natural16_Greatest, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 2 = (1, 1)
  Core_safeAddN16(&u, Core_Natural16_Greatest, 2, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 2 + MAX = (1, 1)
  Core_safeAddN16(&u, 2, Core_Natural16_Greatest, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 37 = (36, 1)
  Core_safeAddN16(&u, Core_Natural16_Greatest, 37, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 37 + MAX = (36, 1)
  Core_safeAddN16(&u, 37, Core_Natural16_Greatest, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1, 1)
  Core_safeAddN16(&u, Core_Natural16_Greatest, Core_Natural16_Greatest, &v);
  if (u != Core_Natural16_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1, 1)
  Core_safeAddN16(&u, Core_Natural16_Greatest, Core_Natural16_Greatest, &v);
  if (u != Core_Natural16_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }

  return Core_Success;
}

static Core_Result Core_Tests_safeAddN32Tests() {
  Core_Natural32 u, v;

  // 0 + 0 = (0, 0)
  Core_safeAddN32(&u, 0, 0, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 0 = (MAX, 0)
  Core_safeAddN32(&u, Core_Natural32_Greatest, 0, &v);
  if (u != Core_Natural32_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // 0 + MAX = (MAX, 0)
  Core_safeAddN32(&u, 0, Core_Natural32_Greatest, &v);
  if (u != Core_Natural32_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 1 = (0, 1)
  Core_safeAddN32(&u, Core_Natural32_Greatest, 1, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 1 + MAX = (0, 1)
  Core_safeAddN32(&u, 1, Core_Natural32_Greatest, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 2 = (1, 1)
  Core_safeAddN32(&u, Core_Natural32_Greatest, 2, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 2 + MAX = (1, 1)
  Core_safeAddN32(&u, 2, Core_Natural32_Greatest, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 37 = (36, 1)
  Core_safeAddN32(&u, Core_Natural32_Greatest, 37, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 37 + MAX = (36, 1)
  Core_safeAddN32(&u, 37, Core_Natural32_Greatest, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1, 1)
  Core_safeAddN32(&u, Core_Natural32_Greatest, Core_Natural32_Greatest, &v);
  if (u != Core_Natural32_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1, 1)
  Core_safeAddN32(&u, Core_Natural32_Greatest, Core_Natural32_Greatest, &v);
  if (u != Core_Natural32_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }

  return Core_Success;
}

static Core_Result Core_Tests_safeAddN64Tests() {
  Core_Natural64 u, v;

  // 0 + 0 = (0, 0)
  Core_safeAddN64(&u, 0, 0, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 0 = (MAX, 0)
  Core_safeAddN64(&u, Core_Natural64_Greatest, 0, &v);
  if (u != Core_Natural64_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // 0 + MAX = (MAX, 0)
  Core_safeAddN64(&u, 0, Core_Natural64_Greatest, &v);
  if (u != Core_Natural64_Greatest) {
    return Core_Failure;
  }
  if (v != 0) {
    return Core_Failure;
  }
  // MAX + 1 = (0, 1)
  Core_safeAddN64(&u, Core_Natural64_Greatest, 1, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 1 + MAX = (0, 1)
  Core_safeAddN64(&u, 1, Core_Natural64_Greatest, &v);
  if (u != 0) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 2 = (1, 1)
  Core_safeAddN64(&u, Core_Natural64_Greatest, 2, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 2 + MAX = (1, 1)
  Core_safeAddN64(&u, 2, Core_Natural64_Greatest, &v);
  if (u != 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + 37 = (36, 1)
  Core_safeAddN64(&u, Core_Natural64_Greatest, 37, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // 37 + MAX = (36, 1)
  Core_safeAddN64(&u, 37, Core_Natural64_Greatest, &v);
  if (u != 36) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1, 1)
  Core_safeAddN64(&u, Core_Natural64_Greatest, Core_Natural64_Greatest, &v);
  if (u != Core_Natural64_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }
  // MAX + MAX = (MAX - 1, 1)
  Core_safeAddN64(&u, Core_Natural64_Greatest, Core_Natural64_Greatest, &v);
  if (u != Core_Natural64_Greatest - 1) {
    return Core_Failure;
  }
  if (v != 1) {
    return Core_Failure;
  }

  return Core_Success;
}

Core_Result Core_Tests_safeAddNxTests() {
  if (Core_Tests_safeAddN8Tests()) {
    return Core_Failure;
  }
  if (Core_Tests_safeAddN16Tests()) {
    return Core_Failure;
  }
  if (Core_Tests_safeAddN32Tests()) {
    return Core_Failure;
  }
  if (Core_Tests_safeAddN64Tests()) {
    return Core_Failure;
  }
  return Core_Success;
}
