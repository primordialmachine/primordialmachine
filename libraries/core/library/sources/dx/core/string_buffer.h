#if !defined (DX_STRING_BUFFER_H_INCLUDED)
#define DX_STRING_BUFFER_H_INCLUDED

#include "Core/Collections/InlineArrayListN8.h"
#include "Core/StringIterator.h"
#include <stdarg.h>
typedef struct Core_String Core_String;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("Core.StringBuffer",
                       Core_StringBuffer,
                       Core_Object);

static inline Core_StringBuffer* CORE_STRINGBUFFER(void* p) {
  return (Core_StringBuffer*)p;
}

struct Core_StringBuffer {
  Core_Object _parent;
  Core_InlineArrayListN8 backend;
};

static inline Core_StringBuffer_Dispatch* CORE_STRINGBUFFER_DISPATCH(void* p) {
  return (Core_StringBuffer_Dispatch*)p;
}

struct Core_StringBuffer_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_StringBuffer_construct(Core_StringBuffer* SELF);

Core_Result Core_StringBuffer_create(Core_StringBuffer** RETURN);

Core_Result Core_StringBuffer_clone(Core_StringBuffer** RETURN, Core_StringBuffer const* SELF);

Core_Result Core_StringBuffer_getByte(Core_Natural8* RETURN, Core_StringBuffer* SELF, Core_Size index);


/** @obsolete */
Core_Result Core_StringBuffer_getBytes(void** RETURN, Core_StringBuffer* SELF);

/// @brief Create an iterator for this string buffer.
/// @param RETURN A pointer to a <code>Core_StringIterator*</code> variable.
/// @success <code>*RETURN</code> was assigned the iterator.
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_createIterator(Core_StringIterator** RETURN, Core_StringBuffer* SELF);

/* http://localhost/core#core-stringbuffer-appendbytes */
Core_Result Core_StringBuffer_appendBytes(Core_StringBuffer* SELF, char const* p, Core_Size n);

/* http://localhost/core#core-stringbuffer-appendstring */
Core_Result Core_StringBuffer_appendString(Core_StringBuffer* SELF, Core_String* string);

/* http://localhost/core#core-stringbuffer-clear */
Core_Result Core_StringBuffer_clear(Core_StringBuffer* SELF);

/* http://localhost/core#core-stringbuffer-getstring */
Core_Result Core_StringBuffer_getString(Core_String** RETURN, Core_StringBuffer* SELF);

/* http://localhost/core#core-stringbuffer-getnumberofbytes */
Core_Result Core_StringBuffer_getNumberOfBytes(Core_Size* RETURN, Core_StringBuffer* SELF);

/* http://localhost/core#core-stringbuffer-getnumberofsymbols */
Core_Result Core_StringBuffer_getNumberOfSymbols(Core_Size* RETURN, Core_StringBuffer* SELF);

/* http://localhost/core#core-stringbuffer-prependbytes */
Core_Result Core_StringBuffer_prependBytes(Core_StringBuffer* SELF, Core_Natural8 const* bytes, Core_Size numberOfBytes);

/* http://localhost/core#core-stringbuffer-prependstring */
Core_Result Core_StringBuffer_prependString(Core_StringBuffer* SELF, Core_String* string);

/* http://localhost/core#core-stringbuffer-setbytes */
Core_Result Core_StringBuffer_setBytes(Core_StringBuffer* SELF, Core_Natural8 const* p, Core_Size n);

/* http://localhost/core#core-stringbuffer-setstring */
Core_Result Core_StringBuffer_setString(Core_StringBuffer* SELF, Core_String* string);

/* http://localhost/core#core-stringbuffer-getsubstring */
Core_Result Core_StringBuffer_getSubstring(Core_String** RETURN, Core_StringBuffer* SELF, Core_Size start, Core_Size length);

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
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_appendFv(Core_StringBuffer* SELF, Core_String* format, va_list arguments);

/// @brief Append a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
/// @param format A pointer to the string.
/// @param arguments The variadic arguments.
/// @see See Core_StringBuffer_appendfv for a description of the format specifiers.
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_appendF(Core_StringBuffer* SELF, Core_String* format, ...);

/// @brief Prepend a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
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
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_prependFv(Core_StringBuffer* SELF, Core_String* format, va_list arguments);

/// @brief Prepend a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
/// @param format A pointer to the string.
/// @param arguments The variadic arguments.
/// @see See Core_StringBuffer_appendfv for a description of the format specifiers.
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_prependF(Core_StringBuffer* SELF, Core_String* format, ...);

/// @brief Insert a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
/// @param index The index of the symbol at which to insert the string.
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
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_insertF(Core_StringBuffer* SELF, Core_Size index, Core_String* format, ...);

/// @brief Insert a string. The string is can have format specifiers. Each format specifier is replaced by its correspnding arguments in the string.
/// @param index The index of the symbol at which to insert the string.
/// @param format A pointer to the string.
/// @param arguments The variadic arguments.
/// @see See Core_StringBuffer_appendfv for a description of the format specifiers.
/// @method{Core_StringBuffer}
Core_Result Core_StringBuffer_insertFv(Core_StringBuffer* SELF, Core_Size index, Core_String* format, va_list arguments);

Core_Result Core_StringBuffer_removeLast(Core_StringBuffer* SELF);

#endif // DX_STRING_BUFFER_H_INCLUDED
