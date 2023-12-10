#if !defined (DX_STRING_BUFFER_H_INCLUDED)
#define DX_STRING_BUFFER_H_INCLUDED

#include "dx/core/byte_array.h"
#include "dx/core/object.h"
#include "dx/core/string_iterator.h"
typedef struct Core_String Core_String;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.string_buffer",
                       dx_string_buffer,
                       Core_Object);

static inline dx_string_buffer* DX_STRING_BUFFER(void* p) {
  return (dx_string_buffer*)p;
}

struct dx_string_buffer {
  Core_Object _parent;
  dx_inline_byte_array backend;
};

static inline dx_string_buffer_dispatch* DX_STRING_BUFFER_DISPATCH(void* p) {
  return (dx_string_buffer_dispatch*)p;
}

struct dx_string_buffer_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_string_buffer_construct(dx_string_buffer* SELF);

Core_Result dx_string_buffer_create(dx_string_buffer** RETURN);

Core_Result dx_string_buffer_clone(dx_string_buffer** RETURN, dx_string_buffer const* SELF);

Core_Result dx_string_buffer_get_bytes(void** RETURN, dx_string_buffer* SELF);

Core_Result dx_string_buffer_get_number_of_bytes(Core_Size* RETURN, dx_string_buffer* SELF);

Core_Result dx_string_buffer_to_string(Core_String** RETURN, dx_string_buffer* SELF);

/// @brief Create an iterator for this string buffer.
/// @param RETURN A pointer to a <code>dx_string_iterator*</code> variable.
/// @success <code>*RETURN</code> was assigned the iterator.
/// @method{Core_String}
Core_Result dx_string_buffer_create_iterator(dx_string_iterator** RETURN, dx_string_buffer* SELF);

/// @brief Set the contents of this string buffer to the specified sequence of Bytes.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The number of Bytes in the array pointed to by @a p.
/// @remark The function checks if the specified sequence of Bytes is a UTF-8 sequence.
/// @method{dx_string_buffer}
/// @error #Core_Error_ArgumentInvalid The sequence of Bytes is not a valid UTF-8 sequence.
Core_Result dx_string_buffer_set_bytes(dx_string_buffer* SELF, char const* p, Core_Size n);

/// @brief Set the contents of this string buffer to the specified string.
/// @param string A pointer to the string.
/// @method{dx_string_buffer}
Core_Result dx_string_buffer_set_string(dx_string_buffer* SELF, Core_String* string);

/// @brief Prepend Bytes to the contents of this string buffer.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The number of Bytes in the array pointed to by @a p.
/// @remark The function checks if the specified sequence of Bytes is a UTF-8 sequence.
/// @method{dx_string_buffer}
/// @error #Core_Error_ArgumentInvalid The sequence of Bytes is not a valid UTF-8 sequence.
Core_Result dx_string_buffer_prepend_bytes(dx_string_buffer* SELF, char const* p, Core_Size n);

/// @brief Prepend a string to the contents of this string buffer.
/// @param string A pointer to the string.
/// @method{dx_string_buffer}
Core_Result dx_string_buffer_prepend_string(dx_string_buffer* SELF, Core_String* string);

/// @brief Append Bytes to the contents of this string buffer.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The number of Bytes in the array pointed to by @a p.
/// @remark The function checks if the specified sequence of Bytes is a UTF-8 sequence.
/// @method{dx_string_buffer}
/// @error #Core_Error_ArgumentInvalid The sequence of Bytes is not a valid UTF-8 sequence.
Core_Result dx_string_buffer_append_bytes(dx_string_buffer* SELF, char const* p, Core_Size n);

/// @brief Append a string to the contents of this string buffer.
/// @param string A pointer to the string.
/// @method{dx_string_buffer}
Core_Result dx_string_buffer_append_string(dx_string_buffer* SELF, Core_String* string);

/// @brief Append a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
/// @param format A pointer to the string.
/// @param arguments The variadic arguments.
/// @details
/// The following format specifiers are currently supported:
/// - `${s}` a pointer to a Core_String object. Prints the contents of this string.
/// - `$$` prints `%`.
/// - `${i8}` a Core_Integer8 value.
/// - `${i16}` a Core_Integer16 value.
/// - `${i32}` a Core_Integer32 value.
/// - `${i64}` a Core_Integer64 value.
/// - `${n8}`  a Core_Natural8 value.
/// - `${n16}` a Core_Natural16 value.
/// - `${n32}` a Core_Natural32 value.
/// - `${n64}` a Core_Natural64 value.
/// - `${r32}` a Core_Real32 value.
/// - `${r64}` a Core_Real64 value.
/// @method{dx_string_buffer}
Core_Result dx_string_buffer_append_fv(dx_string_buffer* SELF, Core_String* format, va_list arguments);

/// @brief Append a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
/// @param format A pointer to the string.
/// @param arguments The variadic arguments.
/// @see See dx_string_buffer_appendfv for a description of the format specifiers.
/// @method{dx_string_buffer}
Core_Result dx_string_buffer_append_f(dx_string_buffer* SELF, Core_String* format, ...);

/// @brief Clear this string buffer.
/// @method{dx_string_buffer}
Core_Result dx_string_buffer_clear(dx_string_buffer* SELF);

Core_Result dx_string_buffer_remove_last(dx_string_buffer* SELF);

#endif // DX_STRING_BUFFER_H_INCLUDED
