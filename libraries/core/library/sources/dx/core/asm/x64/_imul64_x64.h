#if !defined(DX_ASM_X64__IMUL64_X64_H_INCLUDED)
#define DX_ASM_X64__IMUL64_X64_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

#include <inttypes.h>

// compute the product of two int32_t values
// store the high word of the int64_t result in *z
// return the low word of the int64_t result
int32_t _dx_imul64_x64(int32_t x, int32_t y, int32_t* z);

#endif

#endif // DX_ASM_X64__IMUL64_X64_H_INCLUDED
