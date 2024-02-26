// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_NUMERICS_H_INCLUDED)
#define CORE_NUMERICS_H_INCLUDED

#include "Core/FundamentalTypes.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://primordialmachine.com/core#core-min
Core_Result Core_minN8(Core_Natural8* RETURN, Core_Natural8 x, Core_Natural8 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minN16(Core_Natural16* RETURN, Core_Natural16 x, Core_Natural16 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minN32(Core_Natural32* RETURN, Core_Natural32 x, Core_Natural32 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minN64(Core_Natural64* RETURN, Core_Natural64 x, Core_Natural64 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI8(Core_Integer8* RETURN, Core_Integer8 x, Core_Integer8 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI16(Core_Integer16* RETURN, Core_Integer16 x, Core_Integer16 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI32(Core_Integer32* RETURN, Core_Integer32 x, Core_Integer32 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI64(Core_Integer64* RETURN, Core_Integer64 x, Core_Integer64 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minR32(Core_Real32* RETURN, Core_Real32 x, Core_Real32 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minR64(Core_Real64* RETURN, Core_Real64 x, Core_Real64 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minSz(Core_Size* RETURN, Core_Size x, Core_Size y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://primordialmachine.com/core#core-min
Core_Result Core_minN8(Core_Natural8* RETURN, Core_Natural8 x, Core_Natural8 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minN16(Core_Natural16* RETURN, Core_Natural16 x, Core_Natural16 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minN32(Core_Natural32* RETURN, Core_Natural32 x, Core_Natural32 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minN64(Core_Natural64* RETURN, Core_Natural64 x, Core_Natural64 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI8(Core_Integer8* RETURN, Core_Integer8 x, Core_Integer8 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI16(Core_Integer16* RETURN, Core_Integer16 x, Core_Integer16 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI32(Core_Integer32* RETURN, Core_Integer32 x, Core_Integer32 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minI64(Core_Integer64* RETURN, Core_Integer64 x, Core_Integer64 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minR32(Core_Real32* RETURN, Core_Real32 x, Core_Real32 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minR64(Core_Real64* RETURN, Core_Real64 x, Core_Real64 y);

// https://primordialmachine.com/core#core-min
Core_Result Core_minSz(Core_Size* RETURN, Core_Size x, Core_Size y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://primordialmachine.com/core#core-sin
Core_Result Core_sinR32(Core_Real32* RETURN, Core_Real32 x);

// https://primordialmachine.com/core#core-sin
Core_Result Core_sinR64(Core_Real64* RETURN, Core_Real64 x);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://primordialmachine.com/core#core-cos
Core_Result Core_cosR32(Core_Real32* RETURN, Core_Real32 x);

// https://primordialmachine.com/core#core-cos
Core_Result Core_cosR64(Core_Real64* RETURN, Core_Real64 x);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://primordialmachine.com/core#core-floor
Core_Result Core_floorR32(Core_Real32* RETURN, Core_Real32 x);

// https://primordialmachine.com/core#core-floor
Core_Result Core_floorR64(Core_Real64* RETURN, Core_Real64 x);

// https://primordialmachine.com/core#core-ceil
Core_Result Core_ceilR32(Core_Real32* RETURN, Core_Real32 x);

// https://primordialmachine.com/core#core-ceil
Core_Result Core_ceilR64(Core_Real64* RETURN, Core_Real64 x);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_NUMERICS_H_INCLUDED
