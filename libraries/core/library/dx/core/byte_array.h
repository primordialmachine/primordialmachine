#if !defined(DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED)
#define DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED

#include "dx/core/core.h"

/// @todo Should be dx_inline_n8_array.
typedef struct dx_inline_byte_array {

  /// @brief A pointer to an array of @a capacity @a (char) elements.
  /// The first @a size @a (char) elements are the contents of the buffer.
  char *elements;

  /// @brief The size, in elements, of this buffer.
  Core_Size size;

  /// @brief The capacity, in elements, of this buffer.
  Core_Size capacity;

} dx_inline_byte_array;

/// @brief Initialize this dx_inline_byte_array object.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @method{dx_inline_byte_array}
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_inline_byte_array_initialize(dx_inline_byte_array* SELF);

/// @brief Uninitialize this dx_inline_byte_array object.
/// @param SELF A pointer to this dx_inline_byte_array object.
void dx_inline_byte_array_uninitialize(dx_inline_byte_array* SELF);

/// @brief "steal" the backing Byte array from this dx_inline_byte_array object.
/// @param bytes A pointer to a <code>char</code> variable.
/// @param number_of_bytes A pointer to <code>Core_Size</code> variable.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a bytes is a null pointer
/// @error #Core_Error_ArgumentInvalid @a number_of_bytes is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success
/// <code>*bytes</code> was assigned a pointer to an array of <code>*number_of_bytes</code> Bytes.
/// <code>*number_of_bytes</code> was assigned the number of Bytes.
/// The array pointed to by <code>*bytes</code> must be deallocated using <code>free(void*)</code>.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_steal(dx_inline_byte_array* SELF, char **bytes, Core_Size *number_of_bytes);

/// @brief Increase the capacity.
/// @param additional The amount to increase the capacity by.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a additional_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The capacity increased by at least the specified amount.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_increase_capacity(dx_inline_byte_array* SELF, Core_Size additional);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param required The required free capacity.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_AllocationFailed @a required_free_capacity is too big
/// @error #Core_Error_AllocationFailed an allocation failed
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_ensure_free_capacity(dx_inline_byte_array* SELF, Core_Size required);

/// @brief Remove Bytes at the back.
/// @param start The index of the first element.
/// @param length The number of elements to remove.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_remove(dx_inline_byte_array* SELF, Core_Size index, Core_Size length);

/// @brief Set Bytes.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a bytes is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The Bytes in the array pointed to by @a bytes replaced the previous Bytes in this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_set(dx_inline_byte_array* SELF, char const* bytes, Core_Size number_of_bytes);

/// @brief Prepend Bytes.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a bytes is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were prepended to this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_prepend(dx_inline_byte_array* SELF, char const* bytes, Core_Size number_of_bytes);

/// @brief Append Bytes.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a bytes is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were appended to this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_append(dx_inline_byte_array* SELF, char const* bytes, Core_Size number_of_bytes);

/// @brief Insert Bytes.
/// @param index The index in the Byte buffer.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #DX_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_INVALID_ARGUMENT @a bytes is a null pointer
/// @error #DX_INVALID_ARGUMENT @a index is greater than the size of this dx_inline_byte_array object
/// @error #DX_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were inserted at the specified index to this Byte buffer. 
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_insert(dx_inline_byte_array* SELF, Core_Size index, char const* bytes, Core_Size number_of_bytes);

/// @brief Get the size, in Bytes, of this Byte array.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the size, in Bytes, of this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_get_size(Core_Size* RETURN, dx_inline_byte_array const* SELF);

/// @brief Get a pointer to the Bytes of this Byte array.
/// @param RETURN Apointer to a <code>void*</code> variable.
/// @remarks The pointer will invalidate if the Byte buffer is modified.
/// @success <code>*RETURN</code> was assigned a pointer to the Bytes of this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_get_bytes(void** RETURN, dx_inline_byte_array* SELF);

/// @brief Get the capacity, in Bytes, of this Byte array.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the capacity, in Bytes, of this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_get_capacity(Core_Size* RETURN, dx_inline_byte_array const* SELF);

/// @brief Get the free capacity, in Bytes, of this Byte array.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the free capacity, in Bytes, of this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_get_free_capacity(Core_Size* RETURN, dx_inline_byte_array const* SELF);

/// @brief Clear this Byte array.
/// @method{dx_inline_byte_array}
Core_Result dx_inline_byte_array_clear(dx_inline_byte_array* SELF);

#endif // DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED
