#if !defined(DX_ASM_X64__UMUL64_X64_H_INCLUDED)
#define DX_ASM_X64__UMUL64_X64_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

#include <inttypes.h>

// compute the product of two uint32_t values
// store the high word of the uint64_t result in *z
// return the low word of the uint64_t result
uint32_t _dx_umul64_x64(uint32_t x, uint32_t y, uint32_t* z);

#endif

#endif // DX_ASM_X64__UMUL64_X64_H_INCLUDED
