// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "Core/Assets/Image.h"

// string
#include <string.h>
#include "Core/Assets/ImageOperations/ColorFill.h"
#include "Core/Assets/ImageOperations/CheckerboardPatternFill.h"
#include "Core/Assets/ImageOperations/MirrorHorizontal.h"
#include "Core/Assets/ImageOperations/MirrorVertical.h"

Core_defineObjectType("Core.Assets.Image",
                      Core_Assets_Image,
                      Core_Assets_Def);

typedef struct EXTEND2 {
  Core_Size width;
  Core_Size height;
} EXTEND2;

typedef struct OFFSET2 {
  Core_Size left;
  Core_Size top;
} OFFSET2;

static const Core_InlineRgbN8 black = { 0, 0, 0 };

static void Core_Assets_Image_destruct(Core_Assets_Image* SELF) {
  dx_inline_object_array_uninitialize(&SELF->operations);
  CORE_UNREFERENCE(SELF->name);
  SELF->name = NULL;
  Core_SoftwarePixelBuffer_uninitialize(&SELF->backing);
}

static void Core_Assets_Image_constructDispatch(Core_Assets_Image_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Image_construct(Core_Assets_Image* SELF,
                                        Core_String* name,
                                        Core_PixelFormat pixelFormat,
                                        Core_Size width,
                                        Core_Size height) {
  Core_BeginConstructor(Core_Assets_Image);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  Core_Size overflow;
  Core_Size number_of_pixels;
  Core_safeMulSz(&number_of_pixels, width, height, &overflow); // must succeed
  if (overflow) {
    return Core_Failure;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, pixelFormat)) {
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  Core_safeMulSz(&number_of_bytes, number_of_pixels, sizeof(uint8_t) * bytes_per_pixel, &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  SELF->name = name;
  CORE_REFERENCE(name);

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    return Core_Failure;
  }
  Core_SoftwarePixelBuffer_Extend extend = {
    .width = width,
    .height = height,
  };
  if (Core_SoftwarePixelBuffer_initialize(&SELF->backing, &extend, pixelFormat)) {
    dx_inline_object_array_uninitialize(&SELF->operations);
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    return Core_Failure;
  }
  Core_EndConstructor(Core_Assets_Image);
}

Core_Result Core_Assets_Image_create(Core_Assets_Image** RETURN,
                                     Core_String* name, 
                                     Core_PixelFormat pixelFormat,
                                     Core_Size width,
                                     Core_Size height) {
  DX_CREATE_PREFIX(Core_Assets_Image);
  if (Core_Assets_Image_construct(SELF, name, pixelFormat, width, height)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

#include "wic-plugin.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static void* read_image_allocate(void* context, size_t n) {
  int old_error = Core_getError();
  void* p = NULL;
  if (Core_Memory_allocate(&p, n)) {
    Core_setError(old_error);
    return NULL;
  }
  return p;
}

static void read_image_deallocate(void* context, void* p) {
  if (p) {
    Core_Memory_deallocate(p);
  }
}

Core_Result Core_Assets_Image_construct_path(Core_Assets_Image* SELF, Core_String* name, Core_String* path) {
  Core_BeginConstructor(Core_Assets_Image);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  //
  Core_Boolean containsSymbol = Core_False;
  if (Core_String_containsSymbol(&containsSymbol, path, '\0')) {
    return Core_Failure;
  }
  if (containsSymbol) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_String* format = NULL;
  if (Core_String_create(&format, "${s}\0", sizeof("${s}\0"))) {
    return Core_Failure;
  }
  Core_String* path1 = NULL;
  if (Core_String_printf(&path1, format, path)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  //
  HMODULE dll = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == dll) {
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  DX_WIC_PLUGIN_READ_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_READ_IMAGE_PROC*)GetProcAddress(dll, DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME);
  if (!procedure) {
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    FreeLibrary(dll);
    dll = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  void* pixels;
  uint32_t stride;
  uint8_t pixel_format;
  uint32_t width;
  uint32_t height;
  if (procedure(path1->bytes, DX_WIC_PLUGIN_IMAGE_FORMAT_PNG, &pixels, &pixel_format, &stride, &width, &height, NULL, &read_image_allocate, &read_image_deallocate)) {
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    FreeLibrary(dll);
    dll = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  CORE_UNREFERENCE(path1);
  path1 = NULL;
  FreeLibrary(dll);
  dll = NULL;
  //
  switch (pixel_format) {
    case DX_WIC_PLUGIN_PIXEL_FORMAT_BN8_GN8_RN8: {
      Core_SoftwarePixelBuffer_Extend extend = {
        .width = width,
        .height = height,
      };
      if (stride != width * 3) {
        Core_Memory_deallocate(pixels);
        pixels = NULL;
        return Core_Failure;
      }
      if (Core_SoftwarePixelBuffer_initialize(&SELF->backing, &extend, Core_PixelFormat_Bgr8)) {
        Core_Memory_deallocate(pixels);
        pixels = NULL;
        return Core_Failure;
      }
      memcpy(SELF->backing.pixels, pixels, stride * height);
    } break;
    case DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8: {
      Core_SoftwarePixelBuffer_Extend extend = {
        .width = width,
        .height = height,
      };
      if (stride != width * 3) {
        Core_Memory_deallocate(pixels);
        pixels = NULL;
        return Core_Failure;
      }
      if (Core_SoftwarePixelBuffer_initialize(&SELF->backing, &extend, Core_PixelFormat_Rgb8)) {
        Core_Memory_deallocate(pixels);
        pixels = NULL;
        return Core_Failure;
      }
      memcpy(SELF->backing.pixels, pixels, stride * height);
    } break;
    default: {
      Core_Memory_deallocate(pixels);
      pixels = NULL;
      return Core_Failure;
    } break;
  }
  Core_Memory_deallocate(pixels);
  pixels = NULL;

  SELF->name = name;
  CORE_REFERENCE(name);

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    Core_SoftwarePixelBuffer_uninitialize(&SELF->backing);
    return Core_Failure;
  }

  Core_EndConstructor(Core_Assets_Image);
}

Core_Result Core_Assets_Image_create_path(Core_Assets_Image** RETURN, Core_String* name, Core_String* path) {
  DX_CREATE_PREFIX(Core_Assets_Image);
  if (Core_Assets_Image_construct_path(SELF, name, path)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result on_color_fill_image_operation(Core_Assets_Image* SELF, OFFSET2 offset, EXTEND2 extend, Core_Assets_ImageOperations_ColorFill* image_operation) {
  switch (SELF->backing.pixelFormat) {
  case Core_PixelFormat_Rgb8: {
    Core_SoftwarePixelBuffer_Rectangle rectangle = {
      .position = {
        .left = offset.left,
        .top = offset.top,
      },
      .extend = {
        .width = extend.width,
        .height = extend.height,
      },
    };
    Core_SoftwarePixelBuffer_fillRgbN8(&SELF->backing, &rectangle, &(CORE_ASSETS_COLORRGBN8(image_operation->color->object)->value));
  } break;
  default: {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  } break;
  };
  return Core_Success;
}

#include "Core/Assets/ImageOperations/CheckerboardPatternFillImpl.i"
#include "Core/Assets/ImageOperations/MirrorHorizontalImpl.i"
#include "Core/Assets/ImageOperations/MirrorVerticalImpl.i"

Core_Result Core_Assets_Image_apply(Core_Assets_Image* SELF,
                                    Core_Size left,
                                    Core_Size top,
                                    Core_Size width,
                                    Core_Size height,
                                    Core_Assets_ImageOperation* image_operation) {
  Core_Type* type = NULL;
  Core_Boolean result = Core_False;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  if (Core_Assets_ImageOperations_ColorFill_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(image_operation)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_color_fill_image_operation(SELF, offset, extend, CORE_ASSETS_IMAGEOPERATIONS_COLORFILL(image_operation));
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  if (Core_Assets_ImageOperations_CheckerboardPatternFill_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(image_operation)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_checkerboard_pattern_fill_image_operation(SELF, offset, extend, CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL(image_operation));
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  if (Core_Assets_ImageOperations_MirrorHorizontal_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(image_operation)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    return on_mirror_horizontal_operation(SELF);
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  if (Core_Assets_ImageOperations_MirrorVertical_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(image_operation)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    return on_mirror_vertical_operation(SELF);
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  Core_setError(Core_Error_ArgumentInvalid);
  return Core_Failure;
}
