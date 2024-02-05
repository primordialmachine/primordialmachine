// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_CONVERT_H_INCLUDED)
#define CORE_CONVERT_H_INCLUDED

#include "Core/FundamentalTypes.h"

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToI8(Core_Integer8* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToN8(Core_Natural8* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToI16(Core_Integer16* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToN16(Core_Natural16* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToI32(Core_Integer32* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToN32(Core_Natural32* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToI64(Core_Integer64* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToN64(Core_Natural64* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToSz(Core_Size* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToR32(Core_Real32* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToR64(Core_Real64* RETURN, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-convertstringto */
Core_Result Core_convertStringToB(Core_Boolean* RETURN, Core_Natural8 const* p, Core_Size n);

#endif // CORE_CONVERT_H_INCLUDED
