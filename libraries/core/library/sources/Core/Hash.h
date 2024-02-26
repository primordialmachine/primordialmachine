// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_HASH_H_INCLUDED)
#define CORE_HASH_INCLUDED

#include "Core/FundamentalTypes.h"

/* http://localhost/core#core-hashpointer */
Core_Result Core_hashPointer(Core_Size* RETURN, void const* x);

/* http://localhost/core#core-hashbytes */
Core_Result Core_hashBytes(Core_Size* RETURN, void const* p, Core_Size n);

/* http://localhost/core#core-hash */
Core_Result Core_hashB(Core_Size* RETURN, Core_Boolean x);

/* http://localhost/core#core-hash */
Core_Result Core_hashR32(Core_Size* RETURN, Core_Real32 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashR64(Core_Size* RETURN, Core_Real64 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashI8(Core_Size* RETURN, Core_Integer8 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashI16(Core_Size* RETURN, Core_Integer16 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashI32(Core_Size* RETURN, Core_Integer32 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashI64(Core_Size* RETURN, Core_Integer64 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashN8(Core_Size* RETURN, Core_Natural8 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashN16(Core_Size* RETURN, Core_Natural16 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashN32(Core_Size* RETURN, Core_Natural32 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashN64(Core_Size* RETURN, Core_Natural64 x);

/* http://localhost/core#core-hash */
Core_Result Core_hashSz(Core_Size* RETURN, Core_Size x);

/* http://localhost/core#core-combinehashes */
Core_Result Core_combineHashes(Core_Size* RETURN, Core_Size x, Core_Size y);

#endif // CORE_HASH_H_INCLUDED
