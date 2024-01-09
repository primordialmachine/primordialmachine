// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_SAFEADD_NX_H_INCLUDED)
#define CORE_SAFEADDNX_H_INCLUDED

#include "Core/FundamentalTypes.h"

/* http://localhost/core#core-safeadd */
Core_Result Core_safeAddN8(Core_Natural8* RETURN, Core_Natural8 a, Core_Natural8 b, Core_Natural8* c);

/* http://localhost/core#core-safeadd */
Core_Result Core_safeAddN16(Core_Natural16* RETURN, Core_Natural16 a, Core_Natural16 b, Core_Natural16* c);

/* http://localhost/core#core-safeadd */
Core_Result Core_safeAddN32(Core_Natural32* RETURN, Core_Natural32 a, Core_Natural32 b, Core_Natural32* c);

/* http://localhost/core#core-safeadd */
Core_Result Core_safeAddN64(Core_Natural64* RETURN, Core_Natural64 a, Core_Natural64 b, Core_Natural64* c);

/* http://localhost/core#core-safeadd */
Core_Result Core_safeAddSz(Core_Size* RETURN, Core_Size a, Core_Size b, Core_Size* c);

#endif // CORE_SAFEADDNX_H_INCLUDED
