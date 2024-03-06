#if !defined(DX_ASM_X86__IMUL32_X64_H_INCLUDED)
#define DX_ASM_X86__IMUL32_X64_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X64 == Core_InstructionSetArchitecture

#include <inttypes.h>

// compute the product of two int16_t values
// store the high word of the int32_t result in *z
// return the low word of the int32_t result
int16_t _dx_imul32_x64(int16_t x, int16_t y, int16_t* z);

#endif

#endif // DX_ASM_X86__IMUL32_X64_H_INCLUDED
