#if !defined(DX_CORE_SAFE_ADD_NX_H_INCLUDED)
#define DX_CORE_SAFE_ADD_NX_H_INCLUDED

#include <stdint.h>
#include "dx/core/core.h"
#include "dx/core/configuration.h"

/// @ingroup core
/// Compute the sum of two <code>Core_Natural8</code> values.
/// Promote both operands to unsigned values of 16 bit width and compute the sum of the promoted values.
/// Return the lower 8 bit and the higher 8 bit of the resulting 16 bit value separately. 
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param RETURN A pointer to a <code>Core_Natural8</variable>.
/// @param c A pointer to a <code>Core_Natural8</variable>.
/// @return
/// #Core_Success on success. #Core_Failure on failure.
/// @success
/// The lower 8 bit were stored in <code>*RETURN</code>.
/// The higher 8 bit were stored in <code>*c</code>.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a c is a null pointer
Core_Result Core_safeAddN8(Core_Natural8* RETURN, Core_Natural8 a, Core_Natural8 b, Core_Natural8* c);

/// @ingroup core
/// Compute the sum of two <code>uint16_t</code> values.
/// Promote both operands to unsigned values of 32 bit width and compute the sum of the promoted values.
/// Return the lower 16 bit and the higher 16 bit of the resulting 32 bit value separately.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param RETURN A pointer to a <code>uint16_t</variable>.
/// @param c A pointer to a <code>uint16_t</variable>.
/// @return
/// #Core_Success on success. #Core_Failure on failure.
/// @success
/// The lower 16 bit were stored in <code>*RETURN</code>.
/// The higher 16 bit were stored in <code>*c</code>.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a c is a null pointer
Core_Result Core_safeAddN16(Core_Natural16* RETURN, Core_Natural16 a, Core_Natural16 b, Core_Natural16* c);

/// @ingroup core
/// Compute the sum of two <code>Core_Natural32</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>Core_Natural32</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The low word of the sum was stored in <code>*RETURN</code>.
/// The high word of the sum was stored in <code>*c</code>.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a c is a null pointer
Core_Result Core_safeAddN32(Core_Natural32* RETURN, Core_Natural32 a, Core_Natural32 b, Core_Natural32* c);

/// @ingroup core
/// Compute the sum of two <code>Core_Natural64</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>Core_Natural64</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The low word of the sum was stored in <code>*RETURN</code>.
/// The high word of the sum was stored in <code>*c</code>.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a c is a null pointer
Core_Result Core_safeAddN64(Core_Natural64* RETURN, Core_Natural64 a, Core_Natural64 b, Core_Natural64* c);

/// @ingroup core
/// Compute the sum of two <code>Core_Size</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>Core_Size</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The low word of the sum was stored in <code>*RETURN</code>.
/// The high word of the sum was stored in <code>*c</code>.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a c is a null pointer
Core_Result Core_safeAddSz(Core_Size* RETURN, Core_Size a, Core_Size b, Core_Size* c);

#endif // DX_CORE_SAFE_ADD_NX_H_INCLUDED
