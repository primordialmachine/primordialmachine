#if !defined(CORE_FORMATV_H_INCLUDED)
#define CORE_FORMATV_H_INCLUDED

#include "Core/Collections/InlineArrayListN8.h"
#include <stdarg.h>

// Print the formatted string and insert the UTF8 Bytes into the specified buffer at the specified index.
// @param buffer The buffer to insert the UTF8 Bytes into.
Core_Result
Core_insertFormatV
  (
    Core_InlineArrayListN8* buffer,
    Core_Size index,
    char const* start,
    char const* end,
    va_list arguments
  );

// Print the formatted string and prepend the UTF-8 Bytes into the specified buffer at the specified index.
// @param buffer The buffer to prepend the UTF8 Bytes into.
Core_Result
Core_prependFormatV
  (
    Core_InlineArrayListN8* buffer,
    char const* start,
    char const* end,
    va_list arguments
  );

// Print the formatted string and apppend the UTF-8 Bytes into the specified buffer at the specified index.
// @param buffer The buffer to append the UTF8 Bytes into.
Core_Result
Core_appendFormatV
  (
    Core_InlineArrayListN8* buffer,
    char const* start,
    char const* end,
    va_list arguments
  );

#endif // CORE_FORMATV_H_INCLUDED
