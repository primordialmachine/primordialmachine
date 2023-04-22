#if !defined(DX_CORE_STRING_H_INCLUDED)
#define DX_CORE_STRING_H_INCLUDED

#include "dx/core/object.h"

/// @ingroup Core_Strings
/// @brief The type of a string.
DX_DECLARE_OBJECT_TYPE("dx.string",
                       dx_string,
                       dx_object);

static inline dx_string* DX_STRING(void* p) {
  return (dx_string*)p;
}

/// @details
/// A format specifier starts with a dollar sign followed by a left curly bracket.
/// It consists of a "type" specifier followed by an optional "fractional digits" specifier.
/// 
/// The "type" specifier denotes the type of the argument to be printed.
/// The following "type" specifiers are currently supported:
/// - `s` a pointer to a dx_string object. Prints the contents of this string.
/// - `i8` a dx_i8 value.
/// - `i16` a dx_i16 value.
/// - `i32` a dx_i32 value.
/// - `i64` a dx_i64 value.
/// - `n8` a dx_n8 value.
/// - `n16` a dx_n16 value.
/// - `n32` a dx_n32 value.
/// - `n64` a dx_n8 value.
/// - `r32` a dx_f32 value.
/// - `r64` a dx_f64 value.
/// 
/// The optional "fractional digits" specifier denotes the maximum number of fractional digits (digits after the decimal point) for dx_f32 and dx_f64 arguments.
/// It results in a syntax error for other argument types.
/// The significands specifier is of the form
/// ```
/// . <digits>
/// ```
/// where `<digits>` is a non-empty sequence of digits.
/// 
/// To print `$` type `$$`.
dx_string* dx_string_printfv(dx_string* format, va_list arguments);

dx_string* dx_string_printf(dx_string* format, ...);

/// @brief Create a string from an array of Bytes representing a value UTF-8 string.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @return A pointer to the string on success. A null pointer on failure.
/// @default-failure
dx_result dx_string_create(dx_string** RETURN, char const* bytes, dx_size number_of_bytes);

/// @brief Get a pointer to the Bytes of this string.
/// @param RETURN A pointer to a <code>void const*</code> variable.
/// @param SELF A pointer to this string.
/// @success <code>*RETURN</code> was assigned pointer to the Bytes.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @default-runtime-calling-convention
dx_result dx_string_get_bytes(void const** RETURN, dx_string* SELF);

/// @brief Get the number of Bytes of this string.
/// @param RETURN A pointer to a <code>dx_size</code> variable.
/// @param SELF A pointer to this string.
/// @success <code>*RETURN</code> was assigned the number of Bytes.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @default-runtime-calling-convention
dx_result dx_string_get_number_of_bytes(dx_size* RETURN, dx_string* SELF);

/// @brief Get if the string contains an UTF-8 symbol.
/// @param self A pointer to this string.
/// @param symbol The UTF-8 symbol.
/// @return @a true if the string contains the symbol. @a false if the string does not contain the symbol or an error occurred.
/// @default-failure
bool dx_string_contains_symbol(dx_string const* self, uint32_t symbol);

/// @brief Get if this string is equal to another string.
/// @param self A pointer to this string.
/// @param other A pointer to the other string.
/// @return @a true if this string is equal to the other string. @a false otherwise. @a false is also returned on failure.
/// @default-failure
bool dx_string_is_equal_to(dx_string const* self, dx_string const* other);

/// @brief Get the hash value of this string.
/// @param self A pointer to this string.
/// @return The hash value of this string. This may be @a 0. @a 0 is also returned on failure.
/// @default-failure
dx_size dx_string_get_hash_value(dx_string const* self);

struct dx_string {
  dx_object _parent;
  dx_size number_of_bytes;
  char *bytes;
};

struct dx_string_dispatch {
  dx_object_dispatch _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.string_iterator",
                       dx_string_iterator,
                       dx_object);

static inline dx_string_iterator* DX_STRING_ITERATOR(void* p) {
  return (dx_string_iterator*)p;
}

struct dx_string_iterator {
  dx_object _parent;
};

static inline dx_string_iterator_dispatch* DX_STRING_ITERATOR_DISPATCH(void* p) {
  return (dx_string_iterator_dispatch*)p;
}

struct dx_string_iterator_dispatch {
  dx_object_dispatch _parent;
  dx_result (*has_value)(dx_bool *RETURN, dx_string_iterator* SELF);
  dx_result (*get_value)(uint32_t* RETURN, dx_string_iterator* SELF);
  dx_result (*next)(dx_string_iterator* SELF);

};

dx_result dx_string_iterator_construct(dx_string_iterator* SELF);

static inline dx_result dx_string_iterator_has_value(dx_bool *RETURN, dx_string_iterator* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_string_iterator, has_value, RETURN, SELF);
}

static inline dx_result dx_string_iterator_get_value(uint32_t* RETURN, dx_string_iterator* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_string_iterator, get_value, RETURN, SELF);
}

static inline dx_result dx_string_iterator_next(dx_string_iterator* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_string_iterator, next, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @internal 
DX_DECLARE_OBJECT_TYPE("dx.string_iterator_impl",
                       dx_string_iterator_impl,
                       dx_string_iterator);

/// @internal 
static inline dx_string_iterator_impl* DX_STRING_ITERATOR_IMPL(void* p) {
  return (dx_string_iterator_impl*)p;
}

/// @internal 
struct dx_string_iterator_impl {
  dx_string_iterator _parent;
  dx_string* string;
  dx_size index;
};

/// @internal 
static inline dx_string_iterator_impl_dispatch* DX_STRING_ITERATOR_IMPL_DISPATCH(void* p) {
  return (dx_string_iterator_impl_dispatch*)p;
}

/// @internal 
struct dx_string_iterator_impl_dispatch {
  dx_string_iterator_dispatch _parent;
};

/// @internal 
dx_result dx_string_iterator_impl_construct(dx_string_iterator_impl* SELF, dx_string* string);

/// @internal 
dx_result dx_string_iterator_impl_create(dx_string_iterator_impl** RETURN, dx_string* string);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Convert a string into an int64_t value.
/// @param p
/// A pointer to the string.
/// @param l
/// The length, in Bytes, of the string.
/// @param [out] v
/// Pointer to the variable receiving the value.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @failure
/// This function has set the error variable.
/// @success
/// <code>*v</code> was assigned the value.
int dx_str_to_i64(char const* p, dx_size l, int64_t* v);

/// @brief
/// Convert a string into an uint64_t value.
/// @param p
/// A pointer to a string.
/// @param l
/// The length, in Bytes, of the string.
/// @param [out] v
/// Pointer to the variable receiving the value.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @failure
/// This function has set the error variable.
/// @success
/// <code>*v</code> was assigned the value.
int dx_str_to_u64(char const* p, dx_size l, uint64_t* v);

#endif // DX_CORE_STRING_H_INCLUDED
