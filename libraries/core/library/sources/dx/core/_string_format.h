#if !defined(DX_CORE__STRING_FORMAT_H_INCLUDED)
#define DX_CORE__STRING_FORMAT_H_INCLUDED

#include "Core/Collections/InlineArrayListN8.h"
#include <stdarg.h>

Core_Result Core_insertFormatV(Core_InlineArrayListN8* buffer, Core_Size index, char const* start, char const* end, va_list arguments);
Core_Result Core_prependFormatV(Core_InlineArrayListN8* buffer, char const* start, char const* end, va_list arguments);
Core_Result Core_appendFormatV(Core_InlineArrayListN8* buffer, char const* start, char const* end, va_list arguments);

#endif // DX_CORE__STRING_FORMAT_H_INCLUDED
