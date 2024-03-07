#include "dx/core/asm/x86/_imul128_x86.h"

#include "Core/Intrinsic/_umul128/_umul128.h"

// _imul128 using _umul128
int64_t _dx_imul128_x86(int64_t x, int64_t y, int64_t* z) {
  uint64_t r1, z1;
  uint64_t x1 = *((uint64_t*)(int64_t*)&x);
  uint64_t y1 = *((uint64_t*)(int64_t*)&y);
  r1 = _dx_umul128(x1, y1, &z1);
  *z = *((int64_t*)(uint64_t*)&z1);
  return *((int64_t*)(uint64_t*)&r1);
}