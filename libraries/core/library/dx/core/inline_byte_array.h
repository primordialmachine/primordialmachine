#if !defined(DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED)
#define DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED

#include "dx/core/core.h"

typedef struct Core_InlineArrayN8 {

  /// @brief A pointer to an array of @a capacity elements.
  /// The first @a size elements are the contents of the buffer.
  char *elements;

  /// @brief The size, in elements, of this buffer.
  Core_Size size;

  /// @brief The capacity, in elements, of this buffer.
  Core_Size capacity;

} Core_InlineArrayN8;

/// @brief Initialize this Core_InlineArrayN8 object.
/// @param SELF A pointer to this Core_InlineArrayN8 object.
/// @method{Core_InlineArrayN8}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result Core_InlineArrayN8_initialize(Core_InlineArrayN8* SELF);

/// @brief Uninitialize this Core_InlineArrayN8 object.
/// @param SELF A pointer to this Core_InlineArrayN8 object.
void Core_InlineArrayN8_uninitialize(Core_InlineArrayN8* SELF);

/// @brief "steal" the backing array from this Core_InlineArrayN8 object.
/// @param elements A pointer to a <code>char</code> variable.
/// @param numberOfElements A pointer to <code>Core_Size</code> variable.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_ArgumentInvalid @a numberOfElements is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success
/// <code>*elements</code> was assigned a pointer to an array of <code>*numberOfElements</code> elements.
/// <code>*numberOfElements</code> was assigned the number of elements.
/// The array pointed to by <code>*elements</code> must be deallocated using <code>Core_Memory_deallocate(void*)</code> when no longer referenced.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_steal(Core_InlineArrayN8* SELF, char **bytes, Core_Size *number_of_bytes);

/// @brief Increase the capacity.
/// @param additional The amount to increase the capacity by.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_increaseCapacity(Core_InlineArrayN8* SELF, Core_Size additional);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param required The required free capacity.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_ensureFreeCapacity(Core_InlineArrayN8* SELF, Core_Size required);

/// @brief Remove elements at the back.
/// @param start The index of the first element.
/// @param length The number of elements to remove.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_remove(Core_InlineArrayN8* SELF, Core_Size index, Core_Size length);

/// @brief Set elements.
/// @param elements A pointer to an array of @a numberOfElements elements.
/// @param numberOfElements The number of elements in the array pointed to by @a elements.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The elements in the array pointed to by @a elements replaced the previous elements.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_set(Core_InlineArrayN8* SELF, char const* elements, Core_Size numberOfElements);

/// @brief Prepend elements.
/// @param elements A pointer to an array of @a numberOfElements Bytes.
/// @param numberOfElements The number of elements in the array pointed to by @a elements.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The elements in the array pointed to by @a elements were prepended to this array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_prepend(Core_InlineArrayN8* SELF, char const* elements, Core_Size numberOfElements);

/// @brief Append elements.
/// @param elements A pointer to an array of @a numberOfElements elements.
/// @param numberOfElements The number of elements in the array pointed to by @a bytes.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The elements in the array pointed to by @a elements were appended to this array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_append(Core_InlineArrayN8* SELF, char const* elements, Core_Size numberOfElements);

/// @brief Insert elements.
/// @param index The index at which to insert the elements.
/// @param elements A pointer to an array of @a numberOfElements elements.
/// @param numberOfElements The number of elements in the array pointed to by @a elements.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_ArgumentInvalid @a index is greater than the size of this Core_InlineArrayN8 object
/// @error #Core_Errro_AllocationFailed an allocation failed
/// @success The elements in the array pointed to by @a elements were inserted at the specified index into this array. 
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_insert(Core_InlineArrayN8* SELF, Core_Size index, char const* elements, Core_Size numberOfElements);

/// @brief Get the size, in elements, of this array.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the size, in elements, of this array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_getSize(Core_Size* RETURN, Core_InlineArrayN8 const* SELF);

/// @brief Get a pointer to the elements of this array.
/// @param RETURN Apointer to a <code>void*</code> variable.
/// @remarks The pointer will invalidate if this array is modified.
/// @success <code>*RETURN</code> was assigned a pointer to the elements of this array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_getElements(void** RETURN, Core_InlineArrayN8* SELF);

/// @brief Get the capacity, in elements, of this array.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the capacity, in elements, of this array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_getCapacity(Core_Size* RETURN, Core_InlineArrayN8 const* SELF);

/// @brief Get the free capacity, in elements, of this array.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the free capacity, in elements, of this Byte array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayN8 const* SELF);

/// @brief Clear this array.
/// @method{Core_InlineArrayN8}
Core_Result Core_InlineArrayN8_clear(Core_InlineArrayN8* SELF);

#endif // DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED
