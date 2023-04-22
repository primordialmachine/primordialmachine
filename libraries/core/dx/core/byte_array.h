#if !defined(DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED)
#define DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED

#include "dx/core/core.h"

/// @todo Should be dx_inline_n8_array.
typedef struct dx_inline_byte_array {

  /// @brief A pointer to an array of @a capacity @a (char) elements.
  /// The first @a size @a (char) elements are the contents of the buffer.
  char *elements;

  /// @brief The size, in elements, of this buffer.
  dx_size size;

  /// @brief The capacity, in elements, of this buffer.
  dx_size capacity;

} dx_inline_byte_array;

/// @brief Initialize this dx_inline_byte_array object.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_byte_array_initialize(dx_inline_byte_array* SELF);

/// @brief Uninitialize this dx_inline_byte_array object.
/// @param SELF A pointer to this dx_inline_byte_array object.
void dx_inline_byte_array_uninitialize(dx_inline_byte_array* SELF);

/// @brief "steal" the backing Byte array from this dx_inline_byte_array object.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param bytes A pointer to a <code>char</code> variable.
/// @param number_of_bytes A pointer to <code>dx_size</code> variable.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a bytes is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a number_of_bytes is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success
/// <code>*bytes</code> was assigned a pointer to an array of <code>*number_of_bytes</code> Bytes.
/// <code>*number_of_bytes</code> was assigned the number of Bytes.
/// The array pointed to by <code>*bytes</code> must be deallocated using <code>free(void*)</code>.
dx_result dx_inline_byte_array_steal(dx_inline_byte_array* SELF, char **bytes, dx_size *number_of_bytes);

/// @brief Increase the capacity.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param additional The amount to increase the capacity by.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a additional_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
dx_result dx_inline_byte_array_increase_capacity(dx_inline_byte_array* SELF, dx_size additional);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param required The required free capacity.
/// @default-runtime-calling-convention
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED @a required_free_capacity is too big
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_inline_byte_array_ensure_free_capacity(dx_inline_byte_array* SELF, dx_size required);

/// @brief Remove Bytes at the back.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param start The index of the first element.
/// @param length The number of elements to remove.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_remove(dx_inline_byte_array* SELF, dx_size index, dx_size length);

/// @brief Prepend Bytes.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a bytes is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were prepended to this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_prepend(dx_inline_byte_array* SELF, char const* bytes, dx_size number_of_bytes);

/// @brief Append Bytes.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a bytes is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were appended to this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_append(dx_inline_byte_array* SELF, char const* bytes, dx_size number_of_bytes);

/// @brief Insert Bytes.
/// @param SELF A pointer to this dx_inline_byte_array object.
/// @param index The index in the Byte buffer.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @error #DX_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_INVALID_ARGUMENT @a bytes is a null pointer
/// @error #DX_INVALID_ARGUMENT @a index is greater than the size of this dx_inline_byte_array object
/// @error #DX_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were inserted at the specified index to this Byte buffer. 
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_insert(dx_inline_byte_array* SELF, dx_size index, char const* bytes, dx_size number_of_bytes);

/// @brief Get the size, in Bytes, of this Byte array.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this Byte array.
/// @success <code>*RETURN</code> was assigned the size, in Bytes, of this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_get_size(dx_size* RETURN, dx_inline_byte_array const* SELF);

/// @brief Get a pointer to the Bytes of this Byte array.
/// @param RETURN Apointer to a <code>void*</code> variable.
/// @param SELF A pointer to this Byte array.
/// @remarks The pointer will invalidate if the Byte buffer is modified.
/// @success <code>*RETURN</code> was assigned a pointer to the Bytes of this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_get_bytes(void** RETURN, dx_inline_byte_array* SELF);

/// @brief Get the capacity, in Bytes, of this Byte array.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this Byte array.
/// @success <code>*RETURN</code> was assigned the capacity, in Bytes, of this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_get_capacity(dx_size* RETURN, dx_inline_byte_array const* SELF);

/// @brief Get the free capacity, in Bytes, of this Byte array.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this Byte array.
/// @success <code>*RETURN</code> was assigned the free capacity, in Bytes, of this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_get_free_capacity(dx_size* RETURN, dx_inline_byte_array const* SELF);

/// @brief Clear this Byte array.
/// @param SELF A pointer to this Byte array.
/// @default-runtime-calling-convention
dx_result dx_inline_byte_array_clear(dx_inline_byte_array* SELF);

#endif // DX_CORE_INLINE_BYTE_ARRAY_H_INCLUDED
