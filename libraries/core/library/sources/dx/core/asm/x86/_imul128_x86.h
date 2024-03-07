#if !defined(DX_ASM_X86__IMUL128_X86_H_INCLUDED)
#define DX_ASM_X86__IMUL128_X86_H_INCLUDED

#include "dx/core/Configure.h"

#if Core_InstructionSetArchitecture_X86 == Core_InstructionSetArchitecture

#include <inttypes.h>

// compute the product of two int64_t values
// store the high word of the int128_t_t result in *z
// return the low word of the int128_t result
int64_t _dx_imul128_x86(int64_t x, int64_t y, int64_t* z);

#endif

#endif // DX_ASM_X86__IMUL128_X86_H_INCLUDED
