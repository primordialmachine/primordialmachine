#if !defined(CORE_VISUALS_SOFTWAREPIXELBUFFER_H_INCLUDED)
#define CORE_VISUALS_SOFTWAREPIXELBUFFER_H_INCLUDED

#include "Core/Visuals/PixelFormat.h"
#include "dx/core/math.h" // TODO: Should be "Core/Visuals/InlineColor.h".

typedef struct Core_SoftwarePixelBuffer_Position {
  Core_Integer32 left, top;
} Core_SoftwarePixelBuffer_Position;

typedef struct Core_SoftwarePixelBuffer_Extend {
  Core_Natural32  width, height;
} Core_SoftwarePixelBuffer_Extend;

typedef struct Core_SoftwarePixelBuffer_Rectangle {
  Core_SoftwarePixelBuffer_Position position;
  Core_SoftwarePixelBuffer_Extend extend;
} Core_SoftwarePixelBuffer_Rectangle;

typedef struct Core_SoftwarePixelBuffer {
  void* pixels;
  Core_SoftwarePixelBuffer_Extend extend;
  Core_PixelFormat pixelFormat;
} Core_SoftwarePixelBuffer;

// initialize this pixel buffer with the default color for this pixel format
Core_Result
Core_SoftwarePixelBuffer_initialize
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_PixelFormat pixelFormat
  );

Core_Result
Core_SoftwarePixelBuffer_uninitialize
  (
    Core_SoftwarePixelBuffer* SELF
  );

Core_Result
Core_SoftwarePixelBuffer_mirrorHorizontal
  (
    Core_SoftwarePixelBuffer* SELF
  );

Core_Result
Core_SoftwarePixelBuffer_mirrorVertical
  (
    Core_SoftwarePixelBuffer* SELF
  );

Core_Result
Core_SoftwarePixelBuffer_swapColumns
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Natural32 i,
    Core_Natural32 j
  );

Core_Result
Core_SoftwarePixelBuffer_swapRows
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Natural32 i,
    Core_Natural32 j
  );

Core_Result
Core_SoftwarePixelBuffer_fillBgraN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineBgraN8 const* color
  );

Core_Result
Core_SoftwarePixelBuffer_fillBgrN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineBgrN8 const* color
  );

Core_Result
Core_SoftwarePixelBuffer_fillLaN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineLaN8 const* color
  );

Core_Result
Core_SoftwarePixelBuffer_fillLN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineLN8 const* color
  );

Core_Result
Core_SoftwarePixelBuffer_fillRgbaN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineRgbaN8 const* color
  );

Core_Result
Core_SoftwarePixelBuffer_fillRgbN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineRgbN8 const* color
  );

Core_Result
Core_SoftwarePixelBuffer_blit
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Position const* position,
    Core_SoftwarePixelBuffer* source
  );

#endif // CORE_VISUALS_SOFTWAREPIXELBUFFER_H_INCLUDED
