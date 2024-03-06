#if !defined(DX_ASM_X64__UMUL32_X64_H_INCLUDED)
#define DX_ASM_X64__UMUL32_X64_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

#include <inttypes.h>

// compute the product of two uint32_t values
// store the high word of the uint64_t result in *z
// return the low word of the uint64_t result
uint16_t _dx_umul32_x64(uint16_t x, uint16_t y, uint16_t* z);

#endif

#endif // DX_ASM_X64__UMUL32_X64_H_INCLUDED
