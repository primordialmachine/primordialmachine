/// @file Core/Hash.h
/// @brief Compute the hash value of values.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(CORE_HASH_H_INCLUDED)
#define CORE_HASH_INCLUDED

#include "Core/FundamentalTypes.h"

/// @ingroup Core
/// @brief Compute the hash value of a pointer.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The pointer. May be the null pointer.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashPointer(Core_Size* RETURN, void const* x);

/// @ingroup core
/// @brief Compute the hash value of an array of Bytes.
/// @param RETURN A pointer to a Code_Size variable.
/// @param p A pointer to the array of Bytes.
/// @param n The length of the array.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashBytes(Core_Size* RETURN, void const* p, Core_Size n);

/// @ingroup core
/// @brief Compute the hash value of a <code>Core_Boolean</code> value.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashB(Core_Size* RETURN, Core_Boolean x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Real32</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashR32(Core_Size* RETURN, Core_Real32 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Real64</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashR64(Core_Size* RETURN, Core_Real64 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Integer8</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashI8(Core_Size* RETURN, Core_Integer8 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Integer16</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashI16(Core_Size* RETURN, Core_Integer16 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Integer32</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashI32(Core_Size* RETURN, Core_Integer32 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Integer64</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashI64(Core_Size* RETURN, Core_Integer64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>Core_Natural8</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashN8(Core_Size* RETURN, Core_Natural8 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Natural16</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashN16(Core_Size* RETURN, Core_Natural16 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Natural32</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashN32(Core_Size* RETURN, Core_Natural32 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Natural64</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashN64(Core_Size* RETURN, Core_Natural64 x);

/// @ingroup Core
/// @brief Compute the hash value of an <code>Core_Size</code> value.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The value.
/// @success <code>*RETURN</code> was assigned the hash value.
/// @procedure
Core_Result Core_hashSz(Core_Size* RETURN, Core_Size x);

/// @ingroup Core
/// @brief Combine two hash values.
/// @param RETURN A pointer to a Code_Size variable.
/// @param x The first hash value.
/// @param y The second hash value.
/// @success <code>*RETURN</code> was assigned the combination of the first hash value and the second hash value.
Core_Result Core_combineHashes(Core_Size* RETURN, Core_Size x, Core_Size y);


#endif // CORE_HASH_H_INCLUDED
