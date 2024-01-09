#if !defined(DX_CORE_STRING_H_INCLUDED)
#define DX_CORE_STRING_H_INCLUDED

#include "dx/core/object.h"
#include "dx/core/string_iterator.h"

/// @ingroup Core_Strings
/// @brief The type of a string.
Core_declareObjectType("Core.String",
                       Core_String,
                       Core_Object);

static inline Core_String* CORE_STRING(void* p) {
  return (Core_String*)p;
}

struct Core_String {
  Core_Object _parent;
  Core_Size number_of_bytes;
  char* bytes;
};

static inline Core_String* CORE_STRING_DISPATCH(void* p) {
  return (Core_String*)p;
}

struct Core_String_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Create a formatted string.
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @param format The format string.
/// @param arguments The format arguments.
/// @static-method{Core_String}
/// @details See Core_String_printf for information on the format string.
Core_Result Core_String_printfv(Core_String** RETURN, Core_String* format, va_list arguments);

/// @brief Create a formatted string.
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @param format The format string. 
/// @param ... The format arguments.
/// @static-method{Core_String}
/// @details
/// A format specifier starts with a dollar sign followed by a left curly bracket.
/// It consists of a "type" specifier followed by an optional "fractional digits" specifier.
/// 
/// The "type" specifier denotes the type of the argument to be printed.
/// The following "type" specifiers are currently supported:
/// - `s` a pointer to a Core_String object. Prints the contents of this string.
/// - `i8` a Core_Integer8 value.
/// - `i16` a Core_Integer16 value.
/// - `i32` a Core_Integer32 value.
/// - `i64` a Core_Integer64 value.
/// - `n8`  a Core_Natural8 value.
/// - `n16` a Core_Natural16 value.
/// - `n32` a Core_Natural32 value.
/// - `n64` a Core_Natural64 value.
/// - `r32` a Core_Real32 value.
/// - `r64` a Core_Real64 value.
/// 
/// The optional "fractional digits" specifier denotes the maximum number of fractional digits (digits after the decimal point) for Core_Real32 and Core_Real64 arguments.
/// It results in a syntax error for other argument types.
/// The significands specifier is of the form
/// ```
/// . <digits>
/// ```
/// where `<digits>` is a non-empty sequence of digits.
/// 
/// To print `$` type `$$`.
Core_Result Core_String_printf(Core_String** RETURN, Core_String* format, ...);

/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @create-operator{Core_String}
Core_Result Core_String_create(Core_String** RETURN, char const* bytes, Core_Size number_of_bytes);

/// @brief Get a pointer to the Bytes of this string.
/// @param RETURN A pointer to a <code>void const*</code> variable.
/// @success <code>*RETURN</code> was assigned pointer to the Bytes.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @method{Core_String}
Core_Result Core_String_getBytes(void const** RETURN, Core_String* SELF);

/// @brief Get the number of Bytes of this string.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the number of Bytes.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @method{Core_String}
Core_Result Core_String_getNumberOfBytes(Core_Size* RETURN, Core_String* SELF);

/// @brief Get if the string contains an UTF-8 symbol.
/// @param symbol The UTF-8 symbol.
/// @return @a true if the string contains the symbol. @a false if the string does not contain the symbol or an error occurred.
/// @method{Core_String}
Core_Boolean dx_string_contains_symbol(Core_String const* SELF, uint32_t symbol);

/// @brief Get if this string contains a symbol.
/// @param codePoint The code point.
/// @success <code>*RETURN</code> was assigned #Core_True if this string contains the specified symbol and was assigned #Core_False otherwise.
/// @method{Core_String}
Core_Result Core_String_containsSymbol(Core_Boolean* RETURN, Core_String const* SELF, uint32_t symbol);

/// @brief Get if this string is equal to another string.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @param other A pointer to the other string.
/// @success <code>*RETURN</code> was assigned a Core_Boolean value indicating if this string and the other string are equal.
/// More precisely, it is assigned <code>Core_True</code> if this string and the other string are equal and <code>Core_False</code> if they are not. 
/// @method{Core_String}
Core_Result Core_String_isEqualTo(Core_Boolean* RETURN, Core_String const* SELF, Core_String const* other);

/// @brief Get the hash value of this string.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the hash value of this string.
/// @method{Core_String}
Core_Result Core_String_getHashValue(Core_Size* RETURN, Core_String const* SELF);

/// @brief Create an iterator for this string.
/// @param RETURN A pointer to a <code>dx_string_iterator*</code> variable.
/// @success <code>*RETURN</code> was assigned the iterator.
/// @method{Core_String}
Core_Result Core_String_create_iterator(dx_string_iterator** RETURN, Core_String* SELF);

#endif // DX_CORE_STRING_H_INCLUDED
