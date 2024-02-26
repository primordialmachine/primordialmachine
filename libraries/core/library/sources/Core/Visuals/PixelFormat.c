// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Visuals/PixelFormat.h"

Core_defineEnumerationType("Core.PixelFormat",
                           Core_PixelFormat);

Core_Result Core_PixelFormat_getNumberOfBytes(Core_Size* RETURN, Core_PixelFormat SELF) {
  switch (SELF) {
    case Core_PixelFormat_Abgr8: {
      *RETURN = 4;
      return Core_Success;
    } break;
    case Core_PixelFormat_Al8: {
      *RETURN = 2;
      return Core_Success;
    } break;
    case Core_PixelFormat_Bgr8: {
      *RETURN = 3;
      return Core_Success;
    } break;
    case Core_PixelFormat_Bgra8: {
      *RETURN = 4;
      return Core_Success;
    } break;
    case Core_PixelFormat_L8: {
      *RETURN = 1;
      return Core_Success;
    } break;
    case Core_PixelFormat_La8: {
      *RETURN = 2;
      return Core_Success;
    } break;
    case Core_PixelFormat_Rgb8: {
      *RETURN = 3;
      return Core_Success;
    } break;
    case Core_PixelFormat_Rgba8: {
      *RETURN = 4;
      return Core_Success;
    } break;
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };
}

Core_Result Core_PixelFormat_getNumberOfComponents(Core_Size* RETURN, Core_PixelFormat SELF) {
  switch (SELF) {
    case Core_PixelFormat_Abgr8: {
      *RETURN = 4;
      return Core_Success;
    } break;
    case Core_PixelFormat_Al8: {
      *RETURN = 2;
      return Core_Success;
    } break;
    case Core_PixelFormat_Bgr8: {
      *RETURN = 3;
      return Core_Success;
    } break;
    case Core_PixelFormat_Bgra8: {
      *RETURN = 4;
      return Core_Success;
    } break;
    case Core_PixelFormat_L8: {
      *RETURN = 1;
      return Core_Success;
    } break;
    case Core_PixelFormat_La8: {
      *RETURN = 2;
      return Core_Success;
    } break;
    case Core_PixelFormat_Rgb8: {
      *RETURN = 3;
      return Core_Success;
    } break;
    case Core_PixelFormat_Rgba8: {
      *RETURN = 4;
      return Core_Success;
    } break;
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };
}
