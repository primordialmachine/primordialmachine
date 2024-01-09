// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_SAFEMULNX_H_INCLUDED)
#define CORE_SAFEMULNX_H_INCLUDED

#include "Core/FundamentalTypes.h"

/* http://localhost/core#core-safemul */
Core_Result Core_safeMulN8(Core_Natural8* RETURN, Core_Natural8 x, Core_Natural8 y, Core_Natural8* z);

/* http://localhost/core#core-safemul */
Core_Result Core_safeMulN16(Core_Natural16* RETURN, Core_Natural16 x, Core_Natural16 y, Core_Natural16* z);

/* http://localhost/core#core-safemul */
Core_Result Core_safeMulN32(Core_Natural32* RETURN, Core_Natural32 x, Core_Natural32 y, Core_Natural32* z);

/* http://localhost/core#core-safemul */
Core_Result Core_safeMulN64(Core_Natural64* RETURN, Core_Natural64 x, Core_Natural64 y, Core_Natural64* z);

/* http://localhost/core#core-safemul */
Core_Result Core_safeMulSz(Core_Size* RETURN, Core_Size a, Core_Size b, Core_Size* c);

#endif // CORE_SAFEMULNX_H_INCLUDED
