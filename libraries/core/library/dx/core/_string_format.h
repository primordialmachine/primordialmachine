#if !defined(DX_CORE__STRING_FORMAT_H_INCLUDED)
#define DX_CORE__STRING_FORMAT_H_INCLUDED

#include "dx/core/inline_byte_array.h"
#include <stdarg.h>

Core_Result dx__format_v(Core_InlineArrayN8* buffer, char const* start, char const* end, va_list arguments);

#endif // DX_CORE__STRING_FORMAT_H_INCLUDED
