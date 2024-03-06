#if !defined(CORE_INTRINSIC__UMUL128_H_INCLUDED)
#define CORE_INTRINSIC__UMUL128_H_INCLUDED

#include <inttypes.h>

// compute the product of two uint64_t values
// store the high word of the uint128_t result in *z
// return the low word of the uint128_t result
uint64_t _dx_umul128(uint64_t x, uint64_t y, uint64_t* z);

#endif // CORE_INTRINSIC__UMUL128_H_INCLUDED
