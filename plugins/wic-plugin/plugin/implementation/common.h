// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#if !defined(DX_WIC_PLUGIN_COMMON_H_INCLUDED)
#define DX_WIC_PLUGIN_COMMON_H_INCLUDED

// uint8_t, uint32_t
#include <inttypes.h>

// malloc, free
#include <malloc.h>

// WIC header
#define COBJMACROS (1)
#include <wincodec.h>
// WIC library
#pragma comment (lib, "Windowscodecs.lib")

wchar_t* mb_to_wc(const char* source);

#endif // DX_WIC_PLUGIN_COMMON_H_INCLUDED
