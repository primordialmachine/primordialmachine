// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_PIXELFORMAT_H_INCLUDED)
#define CORE_VISUALS_PIXELFORMAT_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-pixelformat */
Core_declareEnumerationType("Core.PixelFormat",
                            Core_PixelFormat);

enum Core_PixelFormat {

  Core_PixelFormat_Abgr8,

  Core_PixelFormat_Al8,

  Core_PixelFormat_Argb8,

  Core_PixelFormat_Bgr8,

  Core_PixelFormat_Bgra8,

  Core_PixelFormat_L8,

  Core_PixelFormat_La8,

  Core_PixelFormat_Rgb8,

  Core_PixelFormat_Rgba8,

};

// http://localhost/val#core-pixelformat-getnumberofbytes
Core_Result Core_PixelFormat_getNumberOfBytes(Core_Size* RETURN, Core_PixelFormat SELF);

// http://localhost/val#core-pixelformat-getnumberofcomponents
Core_Result Core_PixelFormat_getNumberOfComponents(Core_Size* RETURN, Core_PixelFormat SELF);

#endif // CORE_VISUALS_PIXELFORMAT_H_INCLUDED
