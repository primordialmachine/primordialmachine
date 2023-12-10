#if !defined(CORE_COUNTLEADINGZEROES_H_INCLUDED)
#define CORE_COUNTLEADINGZEROES_H_INCLUDED

#include "Core/FundamentalTypes.h"

/* http://localhost/core#core-countleadingzeroes */
Core_Result Core_countLeadingZeroesN8(Core_Size* RETURN, Core_Natural8 x);

/* http://localhost/core#core-countleadingzeroes */
Core_Result Core_countLeadingZeroesN16(Core_Size* RETURN, Core_Natural16 x);

/* http://localhost/core#core-countleadingzeroes */
Core_Result Core_countLeadingZeroesN32(Core_Size* RETURN, Core_Natural32 x);

/* http://localhost/core#core-countleadingzeroes */
Core_Result Core_countLeadingZeroesN64(Core_Size* RETURN, Core_Natural64 x);

/* http://localhost/core#core-countleadingzeroes */
Core_Result Core_countLeadingZeroesSz(Core_Size* RETURN, Core_Size x);

#endif // CORE_COUNTLEADINGZEROES_H_INCLUDED
