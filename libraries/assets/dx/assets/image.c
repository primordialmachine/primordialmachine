#include "dx/assets/image.h"

// string
#include <string.h>
#include "dx/assets/image_operations/color_fill.h"
#include "dx/assets/image_operations/checkerboard_pattern_fill.h"

DX_DEFINE_OBJECT_TYPE("Core.Assets.Image",
                      dx_assets_image,
                      Core_Object);

typedef struct EXTEND2 {
  Core_Size width;
  Core_Size height;
} EXTEND2;

typedef struct OFFSET2 {
  Core_Size left;
  Core_Size top;
} OFFSET2;

static const DX_RGB_N8 black = { 0, 0, 0 };

// primitive operation
static Core_Result _swap_pixels(dx_assets_image* SELF, Core_Size source_x, Core_Size source_y, Core_Size target_x, Core_Size target_y, Core_PixelFormat pixel_format);

// primitive operation
static void _fill_bgr8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_BGR_N8 const* color);
static void _fill_bgra8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_BGRA_N8 const* color);
static void _fill_l8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_L_N8 const* color);
static void _fill_la8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_LA_N8 const* color);
static void _fill_rgb8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_N8 const* color);
static void _fill_rgba8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGBA_N8 const* color);

/// @brief Swap two columns.
/// @param SELF A pointer to this image.
/// @param i, j The indices of the columns.
/// @method-call
/// @remark This is a non-primitive operation.
static Core_Result _swap_columns(dx_assets_image* SELF, Core_Size i, Core_Size j);

/// @brief Swap two rows.
/// @param SELF A pointer to this image.
/// @param i, j The indices of the rows.
/// @method-call
/// @remark This is a non-primitive operation.
static Core_Result _swap_rows(dx_assets_image* SELF, Core_Size i, Core_Size j);

static Core_Result on_color_fill_image_operation(dx_assets_image* SELF, OFFSET2 offset, EXTEND2 extend, dx_assets_image_operations_color_fill* image_operation);

static inline void _swap_bytes(uint8_t* a, uint8_t* b) {
  uint8_t t = *a;
  *a = *b;
  *b = t;
}

static Core_Result _swap_pixels(dx_assets_image* SELF, Core_Size source_x, Core_Size source_y, Core_Size target_x, Core_Size target_y, Core_PixelFormat pixel_format) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (source_x >= SELF->width || source_y >= SELF->height) {
    Core_setError(Core_Error_ArgumentInvalid); \
    return Core_Failure;
  }
  if (target_x >= SELF->width || target_y >= SELF->height) {
    Core_setError(Core_Error_ArgumentInvalid); \
    return Core_Failure;
  }
  if (source_x == target_x && source_y == target_y) {
    return Core_Success;
  }

  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, pixel_format)) {
    return Core_Failure;
  }

  Core_Size source_offset_pixels = source_y * SELF->width + source_x;
  Core_Size source_offset_bytes = source_offset_pixels * bytes_per_pixel;

  Core_Size target_offset_pixels = target_y * SELF->height + target_x;
  Core_Size target_offset_bytes = target_offset_pixels * bytes_per_pixel;

  for (Core_Size i = 0; i < bytes_per_pixel / 2; ++i) {
    // Two example for this loop:
    // - assume bytes_per_pixel = 4 then i = 0 is swapped with j = 3, i = 1 is swapped with j = 2 and the loop terminates.
    // - assume bytes_per_pixel = 3 then i = 0 is swapped with j = 2 and the loop terminates
    _swap_bytes(((uint8_t*)SELF->pixels) + source_offset_bytes + i, ((uint8_t*)SELF->pixels) + target_offset_bytes + (bytes_per_pixel - i - 1));
  }

  return Core_Success;
}

static void _fill_bgr8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_BGR_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  Core_Size fill_right = fill_offset.left + fill_extend.width;
  Core_Size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, Core_PixelFormat_Bgr8)) {
    return;
  }
  for (Core_Size y = fill_offset.top; y < fill_bottom; ++y) {
    for (Core_Size x = fill_offset.left; x < fill_right; ++x) {
      Core_Size offset_pixels = y * image_extend.width + x;
      Core_Size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_BGR_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_bgra8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_BGRA_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  Core_Size fill_right = fill_offset.left + fill_extend.width;
  Core_Size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, Core_PixelFormat_Bgra8)) {
    return;
  }
  for (Core_Size y = fill_offset.top; y < fill_bottom; ++y) {
    for (Core_Size x = fill_offset.left; x < fill_right; ++x) {
      Core_Size offset_pixels = y * image_extend.width + x;
      Core_Size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_BGRA_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_l8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_L_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  Core_Size fill_right = fill_offset.left + fill_extend.width;
  Core_Size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, Core_PixelFormat_L8)) {
    return;
  }
  for (Core_Size y = fill_offset.top; y < fill_bottom; ++y) {
    for (Core_Size x = fill_offset.left; x < fill_right; ++x) {
      Core_Size offset_pixels = y * image_extend.width + x;
      Core_Size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_L_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_la8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_LA_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  Core_Size fill_right = fill_offset.left + fill_extend.width;
  Core_Size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, Core_PixelFormat_La8)) {
    return;
  }
  for (Core_Size y = fill_offset.top; y < fill_bottom; ++y) {
    for (Core_Size x = fill_offset.left; x < fill_right; ++x) {
      Core_Size offset_pixels = y * image_extend.width + x;
      Core_Size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_LA_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_rgb8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  Core_Size fill_right = fill_offset.left + fill_extend.width;
  Core_Size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, Core_PixelFormat_Rgb8)) {
    return;
  }
  for (Core_Size y = fill_offset.top; y < fill_bottom; ++y) {
    for (Core_Size x = fill_offset.left; x < fill_right; ++x) {
      Core_Size offset_pixels = y * image_extend.width + x;
      Core_Size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_RGB_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_rgba8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGBA_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  Core_Size fill_right = fill_offset.left + fill_extend.width;
  Core_Size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, Core_PixelFormat_Rgba8)) {
    return;
  }
  for (Core_Size y = fill_offset.top; y < fill_bottom; ++y) {
    for (Core_Size x = fill_offset.left; x < fill_right; ++x) {
      Core_Size offset_pixels = y * image_extend.width + x;
      Core_Size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_RGBA_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static Core_Result _swap_columns(dx_assets_image* SELF, Core_Size i, Core_Size j) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= SELF->width || j >= SELF->width) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  switch (SELF->pixel_format) {
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
      for (Core_Size y = 0; y < SELF->height; ++y) {
        _swap_pixels(SELF, i, y, j, y, SELF->pixel_format);
      }
    } break;
    default: {
      Core_setError(Core_Error_NotImplemented);
      return Core_Failure;
    } break;
  } 
  return Core_Success;
}

static Core_Result _swap_rows(dx_assets_image* SELF, Core_Size i, Core_Size j) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i >= SELF->height || j >= SELF->height) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  switch (SELF->pixel_format) {
    case Core_PixelFormat_Abgr8:
    case Core_PixelFormat_Argb8:
    case Core_PixelFormat_Bgr8:
    case Core_PixelFormat_Bgra8:
    case Core_PixelFormat_L8:
    case Core_PixelFormat_La8:
    case Core_PixelFormat_Rgb8:
    case Core_PixelFormat_Rgba8: {
      // iterate over the x-axis
      for (Core_Size x = 0; x < SELF->width; ++x) {
        _swap_pixels(SELF, x, i, x, j, SELF->pixel_format);
      }
    } break;
    default: {
      Core_setError(Core_Error_NotImplemented);
      return Core_Failure;
    } break;
  }
  return Core_Success;
}

static void dx_assets_image_destruct(dx_assets_image* SELF) {
  dx_inline_object_array_uninitialize(&SELF->operations);
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
  Core_Memory_deallocate(SELF->pixels);
  SELF->pixels = NULL;
}

static void dx_assets_image_constructDispatch(dx_assets_image_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_image_construct(dx_assets_image* SELF,
                                      Core_String* name,
                                      Core_PixelFormat pixel_format,
                                      Core_Size width,
                                      Core_Size height) {
  DX_CONSTRUCT_PREFIX(dx_assets_image);
  SELF->width = width;
  SELF->height = height;
  SELF->pixel_format = pixel_format;
  Core_Size overflow;
  Core_Size number_of_pixels;
  Core_safeMulSz(&number_of_pixels, SELF->width, SELF->height, &overflow); // must succeed
  if (overflow) {
    return Core_Failure;
  }
  Core_Size bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&bytes_per_pixel, pixel_format)) {
    return Core_Failure;
  }
  Core_Size number_of_bytes;
  Core_safeMulSz(&number_of_bytes, number_of_pixels, sizeof(uint8_t) * bytes_per_pixel, &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  if (Core_Memory_allocate(&SELF->pixels, number_of_bytes)) {
    return Core_Failure;
  }

  OFFSET2 fill_offset = { .left = 0, .top = 0 };
  EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
  EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };

  switch (SELF->pixel_format) {
    case Core_PixelFormat_L8: {
      DX_L_N8 color = { .l = 0 };
    #if 0
      OFFSET2 fill_offset = { .left = 0, .top = 0 };
      EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
      EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
    #endif
      _fill_l8(SELF->pixels, fill_offset, fill_size, image_size, &color);
    } break;
  case Core_PixelFormat_Bgr8: {
    DX_BGR_N8 color = { .r = dx_colors_black.r,
                        .g = dx_colors_black.g,
                        .b = dx_colors_black.b, };
  #if 0
    OFFSET2 fill_offset = { .left = 0, .top = 0 };
    EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
    EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
  #endif
    _fill_bgr8(SELF->pixels, fill_offset, fill_size, image_size, &color);
  } break;
  case Core_PixelFormat_Rgb8: {
    DX_RGB_N8 color = { .r = dx_colors_black.r,
                        .g = dx_colors_black.g,
                        .b = dx_colors_black.b, };
  #if 0
    OFFSET2 fill_offset = { .left = 0, .top = 0 };
    EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
    EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
  #endif
    _fill_rgb8(SELF->pixels, fill_offset, fill_size, image_size, &dx_colors_black);
  } break;
  default: {
    Core_Memory_deallocate(SELF->pixels);
    SELF->pixels = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  } break;
  };
  SELF->name = name;
  DX_REFERENCE(name);

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    Core_Memory_deallocate(SELF->pixels);
    SELF->pixels = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_image_create(dx_assets_image** RETURN,
                                   Core_String* name, 
                                   Core_PixelFormat pixel_format,
                                   Core_Size width,
                                   Core_Size height) {
  DX_CREATE_PREFIX(dx_assets_image);
  if (dx_assets_image_construct(SELF, name, pixel_format, width, height)) {
    DX_UNREFERENCE(SELF);
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

Core_Result dx_assets_image_construct_path(dx_assets_image* SELF, Core_String* name, Core_String* path) {
  DX_CONSTRUCT_PREFIX(dx_assets_image);
  //
  if (dx_string_contains_symbol(path, '\0')) {
    return Core_Failure;
  }
  Core_String* format = NULL;
  if (Core_String_create(&format, "${s}\0", sizeof("${s}\0"))) {
    return Core_Failure;
  }
  Core_String* path1 = NULL;
  if (Core_String_printf(&path1, format, path)) {
    DX_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  //
  HMODULE dll = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == dll) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  DX_WIC_PLUGIN_READ_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_READ_IMAGE_PROC*)GetProcAddress(dll, DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME);
  if (!procedure) {
    DX_UNREFERENCE(path1);
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
    DX_UNREFERENCE(path1);
    path1 = NULL;
    FreeLibrary(dll);
    dll = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return 1;
  }
  DX_UNREFERENCE(path1);
  path1 = NULL;
  FreeLibrary(dll);
  dll = NULL;
  //
  SELF->width = width;
  SELF->height = height;
  switch (pixel_format) {
    case DX_WIC_PLUGIN_PIXEL_FORMAT_BN8_GN8_RN8: {
      SELF->pixel_format = Core_PixelFormat_Bgr8;
    } break;
    case DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8: {
      SELF->pixel_format = Core_PixelFormat_Rgb8;
    } break;
    default: {
      Core_Memory_deallocate(pixels);
      pixels = NULL;
      return Core_Failure;
    } break;
  }
  SELF->pixels = pixels;

  SELF->name = name;
  DX_REFERENCE(name);

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    Core_Memory_deallocate(SELF->pixels);
    SELF->pixels = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_image_create_path(dx_assets_image** RETURN, Core_String* name, Core_String* path) {
  DX_CREATE_PREFIX(dx_assets_image);
  if (dx_assets_image_construct_path(SELF, name, path)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result on_color_fill_image_operation(dx_assets_image* SELF, OFFSET2 offset, EXTEND2 extend, dx_assets_image_operations_color_fill* image_operation) {
  switch (SELF->pixel_format) {
  case Core_PixelFormat_Rgb8: {
    EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
    _fill_rgb8(SELF->pixels, offset, extend, image_size, &(DX_ASSETS_COLOR_RGB_N8(image_operation->color->object)->value));
  } break;
  default: {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  } break;
  };
  return Core_Success;
}

#include "dx/assets/image_operations/checkerboard_pattern_fill_impl.i"
#include "dx/assets/image_operations/mirror_horizontal_impl.i"
#include "dx/assets/image_operations/mirror_vertical_impl.i"

Core_Result dx_assets_image_apply(dx_assets_image* SELF,
                                  Core_Size left,
                                  Core_Size top,
                                  Core_Size width,
                                  Core_Size height,
                                  dx_assets_image_operation* image_operation) {
  Core_Type* type = NULL;

  //
  if (dx_assets_image_operations_color_fill_getType(&type)) {
    return Core_Failure;
  }
  if (dx_rti_type_is_leq(CORE_OBJECT(image_operation)->type, type)) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_color_fill_image_operation(SELF, offset, extend, DX_ASSETS_IMAGE_OPERATIONS_COLOR_FILL(image_operation));
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  
  //
  if (dx_assets_image_operations_checkerboard_pattern_fill_getType(&type)) {
    return Core_Failure;
  }
  if (dx_rti_type_is_leq(CORE_OBJECT(image_operation)->type, type)) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_checkerboard_pattern_fill_image_operation(SELF, offset, extend, DX_ASSETS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(image_operation));
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  
  //
  Core_setError(Core_Error_ArgumentInvalid);
  return Core_Failure;
}
