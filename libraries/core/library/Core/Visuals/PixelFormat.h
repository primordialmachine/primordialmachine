/// @file Core/Visuals/PixelFormat.h
/// @brief Enumeration of pixel formats.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(CORE_VISUALS_PIXELFORMAT_H_INCLUDED)
#define CORE_VISUALS_PIXELFORMAT_H_INCLUDED

#include "dx/core/TypeSystem.h"

/* http://localhost/val#core-pixelformat */
DX_DECLARE_ENUMERATION_TYPE("Core.PixelFormat",
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

/// @ingroup Core_Visuals
/// @brief Get the number of Bytes per pixel of this pixel format.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param self This pixel format.
/// @success <code>*RETURN</code> was assigned the Bytes per pixel of this pixel format.
/// @method{Core_PixelFormat}
Core_Result dx_pixel_format_get_number_of_bytes_per_pixel(Core_Size* RETURN, Core_PixelFormat SELF);

#endif // CORE_VISUALS_PIXELFORMAT_H_INCLUDED
