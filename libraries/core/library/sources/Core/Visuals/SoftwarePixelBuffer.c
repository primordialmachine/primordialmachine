#include "Core/Visuals/SoftwarePixelBuffer.h"

#include "dx/core/math.h"
#include "Core/Memory.h"
#include "Core/safeMulNx.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void
fillBgraN8
  (
    void* pixels,
    Core_SoftwarePixelBuffer_Extend* size,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineBgraN8 const* color
  );

static void
fillBgrN8
  (
    void* pixels,
    Core_SoftwarePixelBuffer_Extend *size,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineBgrN8 const* color
  );

static void
fillLaN8
  (
    void* pixels,
    Core_SoftwarePixelBuffer_Extend* size,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineLaN8 const* color
  );

static void
fillLN8
  (
    void* pixels,
    Core_SoftwarePixelBuffer_Extend* size,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineLN8 const* color
  );

static void
fillRgbaN8
  (
    void* pixels,
    Core_SoftwarePixelBuffer_Extend* size,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineRgbaN8 const* color
  );

static void
fillRgbN8
  (
    void* pixels,
    Core_SoftwarePixelBuffer_Extend* size,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineRgbN8 const* color
  );

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static inline void
swapBytes
  (
    uint8_t* a,
    uint8_t* b
  );

static Core_Result
swapPixels
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Position const* source,
    Core_SoftwarePixelBuffer_Position const* target
  );

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static Core_Result
swapColumns
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Size i,
    Core_Size j
  );

static Core_Result
swapRows
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Size i,
    Core_Size j
  );

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void
fillBgraN8
  (
    void* targetPixels,
    Core_SoftwarePixelBuffer_Extend* targetExtend,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineBgraN8 const* color
  )
{
  // fast clip
  if (start->left > targetExtend->width) {
    return;
  }
  if (start->top > targetExtend->height) {
    return;
  }
  // TODO: Overflow check.
  Core_Natural32 fillRight = start->left + extend->width;
  Core_Natural32 fillBottom = start->top + extend->height;
  // clamp
  if (fillRight > targetExtend->width) {
    fillRight = targetExtend->width;
  }
  if (fillBottom > targetExtend->height) {
    fillBottom = targetExtend->height;
  }
  if (start->left == fillRight) {
    return;
  }
  if (start->top == fillBottom) {
    return;
  }
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_Bgra8)) {
    return;
  }
  for (Core_Size y = start->top; y < fillBottom; ++y) {
    for (Core_Size x = start->left; x < fillRight; ++x) {
      Core_Size offsetPixels = y * targetExtend->width + x;
      Core_Size offsetBytes = offsetPixels * numberOfBytesPerPixel;
      *(Core_InlineBgraN8*)(((uint8_t*)targetPixels) + offsetBytes) = *color;
    }
  }
}

static void
fillBgrN8
  (
    void* targetPixels,
    Core_SoftwarePixelBuffer_Extend* targetExtend,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineBgrN8 const* color
  )
{
  // fast clip
  if (start->left > targetExtend->width) {
    return;
  }
  if (start->top > targetExtend->height) {
    return;
  }
  // TODO: Overflow check.
  Core_Natural32 fillRight = start->left + extend->width;
  Core_Natural32 fillBottom = start->top + extend->height;
  // clamp
  if (fillRight > targetExtend->width) {
    fillRight = targetExtend->width;
  }
  if (fillBottom > targetExtend->height) {
    fillBottom = targetExtend->height;
  }
  if (start->left == fillRight) {
    return;
  }
  if (start->top == fillBottom) {
    return;
  }
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_Bgr8)) {
    return;
  }
  for (Core_Size y = start->top; y < fillBottom; ++y) {
    for (Core_Size x = start->left; x < fillRight; ++x) {
      Core_Size offsetPixels = y * targetExtend->width + x;
      Core_Size offsetBytes = offsetPixels * numberOfBytesPerPixel;
      *(Core_InlineBgrN8*)(((uint8_t*)targetPixels) + offsetBytes) = *color;
    }
  }
}

static void
fillLaN8
  (
    void* targetPixels,
    Core_SoftwarePixelBuffer_Extend* targetExtend,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineLaN8 const* color
  )
{
  // fast clip
  if (start->left > targetExtend->width) {
    return;
  }
  if (start->top > targetExtend->height) {
    return;
  }
  // TODO: Overflow check.
  Core_Natural32 fillRight = start->left + extend->width;
  Core_Natural32 fillBottom = start->top + extend->height;
  // clamp
  if (fillRight > targetExtend->width) {
    fillRight = targetExtend->width;
  }
  if (fillBottom > targetExtend->height) {
    fillBottom = targetExtend->height;
  }
  if (start->left == fillRight) {
    return;
  }
  if (start->top == fillBottom) {
    return;
  }
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_La8)) {
    return;
  }
  for (Core_Size y = start->top; y < fillBottom; ++y) {
    for (Core_Size x = start->left; x < fillRight; ++x) {
      Core_Size offsetPixels = y * targetExtend->width + x;
      Core_Size offsetBytes = offsetPixels * numberOfBytesPerPixel;
      *(Core_InlineLaN8*)(((uint8_t*)targetPixels) + offsetBytes) = *color;
    }
  }
}

static void
fillLN8
  (
    void* targetPixels,
    Core_SoftwarePixelBuffer_Extend* targetExtend,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineLN8 const* color
  )
{
  // fast clip
  if (start->left > targetExtend->width) {
    return;
  }
  if (start->top > targetExtend->height) {
    return;
  }
  // TODO: Overflow check.
  Core_Natural32 fillRight = start->left + extend->width;
  Core_Natural32 fillBottom = start->top + extend->height;
  // clamp
  if (fillRight > targetExtend->width) {
    fillRight = targetExtend->width;
  }
  if (fillBottom > targetExtend->height) {
    fillBottom = targetExtend->height;
  }
  if (start->left == fillRight) {
    return;
  }
  if (start->top == fillBottom) {
    return;
  }
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_L8)) {
    return;
  }
  for (Core_Size y = start->top; y < fillBottom; ++y) {
    for (Core_Size x = start->left; x < fillRight; ++x) {
      Core_Size offsetPixels = y * targetExtend->width + x;
      Core_Size offsetBytes = offsetPixels * numberOfBytesPerPixel;
      *(Core_InlineLN8*)(((uint8_t*)targetPixels) + offsetBytes) = *color;
    }
  }
}

static void
fillRgbaN8
  (
    void* targetPixels,
    Core_SoftwarePixelBuffer_Extend* targetExtend,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineRgbaN8 const* color
  )
{
  // fast clip
  if (start->left > targetExtend->width) {
    return;
  }
  if (start->top > targetExtend->height) {
    return;
  }
  // TODO: Overflow check.
  Core_Natural32 fillRight = start->left + extend->width;
  Core_Natural32 fillBottom = start->top + extend->height;
  // clamp
  if (fillRight > targetExtend->width) {
    fillRight = targetExtend->width;
  }
  if (fillBottom > targetExtend->height) {
    fillBottom = targetExtend->height;
  }
  if (start->left == fillRight) {
    return;
  }
  if (start->top == fillBottom) {
    return;
  }
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_Rgba8)) {
    return;
  }
  for (Core_Size y = start->top; y < fillBottom; ++y) {
    for (Core_Size x = start->left; x < fillRight; ++x) {
      Core_Size offsetPixels = y * targetExtend->width + x;
      Core_Size offsetBytes = offsetPixels * numberOfBytesPerPixel;
      *(Core_InlineRgbaN8*)(((uint8_t*)targetPixels) + offsetBytes) = *color;
    }
  }
}

static void
fillRgbN8
  (
    void* targetPixels,
    Core_SoftwarePixelBuffer_Extend* targetExtend,
    Core_SoftwarePixelBuffer_Position const* start,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_InlineRgbN8 const* color
  )
{ 
  // fast clip
  if (start->left > targetExtend->width) {
    return;
  }
  if (start->top > targetExtend->height) {
    return;
  }
  // TODO: Overflow check.
  Core_Natural32 fillRight = start->left + extend->width;
  Core_Natural32 fillBottom = start->top + extend->height;
  // clamp
  if (fillRight > targetExtend->width) {
    fillRight = targetExtend->width;
  }
  if (fillBottom > targetExtend->height) {
    fillBottom = targetExtend->height;
  }
  if (start->left == fillRight) {
    return;
  }
  if (start->top == fillBottom) {
    return;
  }
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_Rgb8)) {
    return;
  }
  for (Core_Size y = start->top; y < fillBottom; ++y) {
    for (Core_Size x = start->left; x < fillRight; ++x) {
      Core_Size offsetPixels = y * targetExtend->width + x;
      Core_Size offsetBytes = offsetPixels * numberOfBytesPerPixel;
      *(Core_InlineRgbN8*)(((uint8_t*)targetPixels) + offsetBytes) = *color;
    }
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static inline void
swapBytes
  (
    uint8_t* a,
    uint8_t* b
  )
{
  uint8_t t = *a;
  *a = *b;
  *b = t;
}

static Core_Result
swapPixels
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Position const* source,
    Core_SoftwarePixelBuffer_Position const* target
  )
{
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (source->left >= SELF->extend.width || source->top >= SELF->extend.height) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (target->left >= SELF->extend.width || target->top >= SELF->extend.height) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (source->left == target->left && source->top == target->top) {
    return Core_Success;
  }

  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, SELF->pixelFormat)) {
    return Core_Failure;
  }

  Core_Size source_offset_pixels = source->top * SELF->extend.width + source->left;
  Core_Size source_offset_bytes = source_offset_pixels * bytes_per_pixel;

  Core_Size target_offset_pixels = target->top * SELF->extend.height + target->left;
  Core_Size target_offset_bytes = target_offset_pixels * bytes_per_pixel;

  for (Core_Size i = 0; i < bytes_per_pixel / 2; ++i) {
    // Two example for this loop:
    // - assume bytes_per_pixel = 4 then i = 0 is swapped with j = 3, i = 1 is swapped with j = 2 and the loop terminates.
    // - assume bytes_per_pixel = 3 then i = 0 is swapped with j = 2 and the loop terminates
    swapBytes(((uint8_t*)SELF->pixels) + source_offset_bytes + i, ((uint8_t*)SELF->pixels) + target_offset_bytes + (bytes_per_pixel - i - 1));
  }

  return Core_Success;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static Core_Result
swapColumns
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Size i,
    Core_Size j
  )
{
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= SELF->extend.width || j >= SELF->extend.width) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  switch (SELF->pixelFormat) {
    case Core_PixelFormat_Abgr8:
    case Core_PixelFormat_Argb8:
    case Core_PixelFormat_Bgr8:
    case Core_PixelFormat_Bgra8:
    case Core_PixelFormat_L8:
    case Core_PixelFormat_La8:
    case Core_PixelFormat_Rgb8:
    case Core_PixelFormat_Rgba8:
    {
      // iterate over the y-axis
      for (Core_Size y = 0; y < SELF->extend.height; ++y) {
        Core_SoftwarePixelBuffer_Position source = {
          .left = i,
          .top = y,
        };
        Core_SoftwarePixelBuffer_Position target = {
          .left = j,
          .top = y
        };
        swapPixels(SELF, &source, &target);
      }
    } break;
    default: {
      Core_setError(Core_Error_NotImplemented);
      return Core_Failure;
    } break;
  }
  return Core_Success;
}

static Core_Result
swapRows
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Size i,
    Core_Size j
  )
{
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= SELF->extend.width || j >= SELF->extend.height) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  switch (SELF->pixelFormat) {
    case Core_PixelFormat_Abgr8:
    case Core_PixelFormat_Argb8:
    case Core_PixelFormat_Bgr8:
    case Core_PixelFormat_Bgra8:
    case Core_PixelFormat_L8:
    case Core_PixelFormat_La8:
    case Core_PixelFormat_Rgb8:
    case Core_PixelFormat_Rgba8: {
      // iterate over the x-axis
      for (Core_Size x = 0; x < SELF->extend.width; ++x) {
        Core_SoftwarePixelBuffer_Position source = {
          .left = x,
          .top = i,
        };
        Core_SoftwarePixelBuffer_Position target = {
          .left = x,
          .top = j,
        };
        swapPixels(SELF, &source, &target);
      }
    } break;
    default: {
      Core_setError(Core_Error_NotImplemented);
      return Core_Failure;
    } break;
  }
  return Core_Success;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

Core_Result
Core_SoftwarePixelBuffer_initialize
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Extend const* extend,
    Core_PixelFormat pixelFormat
  )
{
  if (!SELF || !extend) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  
  SELF->extend = *extend;
  SELF->pixelFormat = pixelFormat;

  Core_Natural32 overflow;
  
  //
  Core_Natural32 numberOfPixels;
  Core_safeMulN32(&numberOfPixels, SELF->extend.width, SELF->extend.height, &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  
  //
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, pixelFormat)) {
    return Core_Failure;
  }
  if (numberOfBytesPerPixel > Core_Natural32_Greatest) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  Core_Natural32 numberOfBytes;
  Core_safeMulN32(&numberOfBytes, numberOfPixels, (Core_Natural32)sizeof(uint8_t) * (Core_Natural32)numberOfBytesPerPixel, &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }

  //
  if (Core_Memory_allocate(&SELF->pixels, numberOfBytes)) {
    return Core_Failure;
  }

  //
  Core_SoftwarePixelBuffer_Position fillStart = { .left = 0, .top = 0 };
  Core_SoftwarePixelBuffer_Extend fillExtend = SELF->extend;
  switch (SELF->pixelFormat) {
    case Core_PixelFormat_L8: {
      Core_InlineLN8 color = { .l = 0 };
      fillLN8(SELF->pixels, &SELF->extend, &fillStart, &fillExtend, &color);
    } break;
    case Core_PixelFormat_La8: {
      Core_InlineLaN8 color = { .l = 0, .a = 255, };
      fillLaN8(SELF->pixels, &SELF->extend, &fillStart, &fillExtend, &color);
    } break;
    case Core_PixelFormat_Bgr8: {
      Core_InlineBgrN8 color = { .r = dx_colors_black.r,
                                 .g = dx_colors_black.g,
                                 .b = dx_colors_black.b, };
      fillBgrN8(SELF->pixels, &SELF->extend, &fillStart, &fillExtend, &color);
    } break;
    case Core_PixelFormat_Bgra8: {
      Core_InlineBgraN8 color = { .r = dx_colors_black.r,
                                  .g = dx_colors_black.g,
                                  .b = dx_colors_black.b,
                                  .a = 255, };
      fillBgraN8(SELF->pixels, &SELF->extend, &fillStart, &fillExtend, &color);
    } break;
    case Core_PixelFormat_Rgb8: {
      Core_InlineRgbN8 color = { .r = dx_colors_black.r,
                                 .g = dx_colors_black.g,
                                 .b = dx_colors_black.b, };
      fillRgbN8(SELF->pixels, &SELF->extend, &fillStart, &fillExtend, &color);
    } break;
    case Core_PixelFormat_Rgba8: {
      Core_InlineRgbaN8 color = { .r = dx_colors_black.r,
                                  .g = dx_colors_black.g,
                                  .b = dx_colors_black.b,
                                  .a = 255, };
      fillRgbaN8(SELF->pixels, &SELF->extend, &fillStart, &fillExtend, &color);
    } break;
    default: {
      Core_Memory_deallocate(SELF->pixels);
      SELF->pixels = NULL;
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };

  //
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_uninitialize
  (
    Core_SoftwarePixelBuffer* SELF
  )
{
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Memory_deallocate(SELF->pixels);
  SELF->pixels = NULL;
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_mirrorHorizontal
  (
    Core_SoftwarePixelBuffer* SELF
  )
{
  for (Core_Natural32 row = 0; row < SELF->extend.height / 2; ++row) {
    Core_SoftwarePixelBuffer_swapRows(SELF, row, SELF->extend.height - 1 - row);
  }
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_mirrorVertical
  (
    Core_SoftwarePixelBuffer* SELF
  )
{
  for (Core_Natural32 column = 0; column < SELF->extend.width / 2; ++column) {
    Core_SoftwarePixelBuffer_swapColumns(SELF, column, SELF->extend.width - 1 - column);
  }
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_swapColumns
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Natural32 i,
    Core_Natural32 j
  )
{ return swapColumns(SELF, i, j); }

Core_Result
Core_SoftwarePixelBuffer_swapRows
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_Natural32 i,
    Core_Natural32 j
  )  
{ return swapRows(SELF, i, j); }

Core_Result
Core_SoftwarePixelBuffer_fillBgraN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineBgraN8 const* color
  )
{
  fillBgraN8(SELF->pixels, &SELF->extend, &rectangle->position, &rectangle->extend, color);
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_fillBgrN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineBgrN8 const* color
  )
{
  fillBgrN8(SELF->pixels, &SELF->extend, &rectangle->position, &rectangle->extend, color);
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_fillLaN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineLaN8 const* color
  )
{
  fillLaN8(SELF->pixels, &SELF->extend, &rectangle->position, &rectangle->extend, color);
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_fillLN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineLN8 const* color
  ) 
{
  fillLN8(SELF->pixels, &SELF->extend, &rectangle->position, &rectangle->extend, color);
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_fillRgbaN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineRgbaN8 const* color
  )
{
  fillRgbaN8(SELF->pixels, &SELF->extend, &rectangle->position, &rectangle->extend, color);
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_fillRgbN8
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Rectangle const* rectangle,
    Core_InlineRgbN8 const* color
  )
{
  fillRgbN8(SELF->pixels, &SELF->extend, &rectangle->position, &rectangle->extend, color);
  return Core_Success;
}

Core_Result
Core_SoftwarePixelBuffer_blit
  (
    Core_SoftwarePixelBuffer* SELF,
    Core_SoftwarePixelBuffer_Position const* position,
    Core_SoftwarePixelBuffer* source
  )
{
  if (position->left + source->extend.width < 0) {
    return Core_Success;
  }
  if (position->left > SELF->extend.width) {
    return Core_Success;
  }
  if (position->top + source->extend.height < 0) {
    return Core_Success;
  }
  if (position->top > SELF->extend.height) {
    return Core_Success;
  }
  // the maximal extend
  Core_Integer32 maxExtendWidth = SELF->extend.width - position->left;
  if (maxExtendWidth > source->extend.width) {
    maxExtendWidth = source->extend.width;
  }
  Core_Integer32 maxExtendHeight = SELF->extend.height - position->top;
  if (maxExtendHeight > source->extend.height) {
    maxExtendHeight = source->extend.height;
  }
  // target rectangle
  Core_SoftwarePixelBuffer_Rectangle targetRectangle;
  targetRectangle.position.left = position->left;
  targetRectangle.extend.width = maxExtendWidth;
  targetRectangle.position.top = position->top;
  targetRectangle.extend.height = maxExtendHeight;
  // source rectangle
  Core_SoftwarePixelBuffer_Rectangle sourceRectangle;
  sourceRectangle.position.left = 0;
  sourceRectangle.extend.width = maxExtendWidth;
  sourceRectangle.position.top = 0;
  sourceRectangle.extend.height = maxExtendHeight;

  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_Rgba8)) {
    return Core_Failure;
  }
  for (size_t y = 0; y < maxExtendHeight; ++y) {
    for (size_t x = 0; x < maxExtendWidth; ++x) {
      size_t srcx = x, srcy = y;
      size_t srcoffset = srcx + srcy * source->extend.width;
      size_t dstx = x + position->left, dsty = y + position->top;
      size_t dstoffset = dstx + dsty * SELF->extend.width;
      char* src = ((char*)source->pixels) + srcoffset;
      char* dst = ((char*)SELF->pixels) + dstoffset;
      for (size_t b = 0; b < numberOfBytesPerPixel; ++b) {
        dst[b] = src[b];
      }
    }
  }
  //
  return Core_Success;
}
