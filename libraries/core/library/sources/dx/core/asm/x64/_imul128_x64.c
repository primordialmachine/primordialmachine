#include "dx/core/asm/x64/_imul128_x64.h"

#include "Core/Intrinsic/_umul128/_umul128.h"

// https://learn.microsoft.com/en-us/cpp/intrinsics/mul128?view=msvc-170
// _imul128 using _umul128
int64_t _dx_imul128_x64(int64_t x, int64_t y, int64_t* z) {
  uint64_t r1, z1;
  uint64_t x1 = *((uint64_t*)(int64_t*)&x);
  uint64_t y1 = *((uint64_t*)(int64_t*)&y);
  r1 = _dx_umul128(x1, y1, &z1);
  *z = *((int64_t*)(uint64_t*)&z1);
  return *((int64_t*)(uint64_t*)&r1);
}
