#if !defined(DX_ASM_X86__IMUL16_H_INCLUDED)
#define DX_ASM_X86__IMUL16_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

#include <inttypes.h>

// compute the product of two int8_t values
// store the high word of the int16_t result in *z
// return the low word of the int16_t result
int8_t _fastcall _dx_imul16_x86_fastcall(int8_t x, int8_t y, int8_t* z);

#endif

#endif // DX_ASM_X86__IMUL16_H_INCLUDED
