#if !defined(CORE_CONVERT_H_INCLUDED)
#define CORE_CONVERT_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup core
/// @brief Convert a string to a <code>Core_Integer8</code> value.
/// @param p A pointer to an UTF-8 byte sequence of length @a n.
/// @param n The length of the UTF-8 Byte sequence pointed to by @a p.
/// @param target A pointer to a <code>Core_Integer8</code> variable.
/// @error #Core_Error_ArgumentInvalid @a p is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a target is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a p and @a n do not denote a valid UTF-8 byte sequence.
/// @procedure
Core_Result Core_convertStringToI8(Core_Integer8* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToN8(Core_Natural8* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToI16(Core_Integer16* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToN16(Core_Natural16* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToI32(Core_Integer32* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToN32(Core_Natural32* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToI64(Core_Integer64* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToN64(Core_Natural64* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToSz(Core_Size* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToR32(Core_Real32* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToR64(Core_Real64* RETURN, Core_Natural8 const* p, Core_Size n);

Core_Result Core_convertStringToB(Core_Boolean* RETURN, Core_Natural8 const* p, Core_Size n);

#endif // CORE_CONVERT_H_INCLUDED
