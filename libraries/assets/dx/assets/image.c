#include "dx/assets/image.h"

// string
#include <string.h>
#include "dx/assets/image_operations/color_fill.h"
#include "dx/assets/image_operations/checkerboard_pattern_fill.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.image",
                      dx_asset_image,
                      dx_object);

typedef struct EXTEND2 {
  dx_size width;
  dx_size height;
} EXTEND2;

typedef struct OFFSET2 {
  dx_size left;
  dx_size top;
} OFFSET2;

static const DX_RGB_N8 black = { 0, 0, 0 };

// primitive operation
static dx_result _swap_pixels(dx_asset_image* SELF, dx_size source_x, dx_size source_y, dx_size target_x, dx_size target_y, dx_pixel_format pixel_format);

// primitive operation
static void _fill_bn8_gn8_rn8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_BGR_N8 const* color);
static void _fill_rn8_gn8_bn8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_N8 const* color);
static void _fill_ln8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, uint8_t color);

/// @brief Swap two columns.
/// @param SELF A pointer to this image.
/// @param i, j The indices of the columns.
/// @method-call
/// @remark This is a non-primitive operation.
static dx_result _swap_columns(dx_asset_image* SELF, dx_size i, dx_size j);

/// @brief Swap two rows.
/// @param SELF A pointer to this image.
/// @param i, j The indices of the rows.
/// @method-call
/// @remark This is a non-primitive operation.
static dx_result _swap_rows(dx_asset_image* SELF, dx_size i, dx_size j);

static dx_result on_color_fill_image_operation(dx_asset_image* SELF, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_color_fill* image_operation);

static inline void _swap_bytes(uint8_t* a, uint8_t* b) {
  uint8_t t = *a;
  *a = *b;
  *b = t;
}

static dx_result _swap_pixels(dx_asset_image* SELF, dx_size source_x, dx_size source_y, dx_size target_x, dx_size target_y, dx_pixel_format pixel_format) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (source_x >= SELF->width || source_y >= SELF->height) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT); \
    return DX_FAILURE;
  }
  if (target_x >= SELF->width || target_y >= SELF->height) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT); \
    return DX_FAILURE;
  }
  if (source_x == target_x && source_y == target_y) {
    return DX_SUCCESS;
  }

  dx_size bytes_per_pixel;
  if (dx_pixel_format_get_number_of_bytes_per_pixel(&bytes_per_pixel, pixel_format)) {
    return DX_FAILURE;
  }

  dx_size source_offset_pixels = source_y * SELF->width + source_x;
  dx_size source_offset_bytes = source_offset_pixels * bytes_per_pixel;

  dx_size target_offset_pixels = target_y * SELF->height + target_x;
  dx_size target_offset_bytes = target_offset_pixels * bytes_per_pixel;

  for (dx_size i = 0; i < bytes_per_pixel / 2; ++i) {
    // Two example for this loop:
    // - assume bytes_per_pixel = 4 then i = 0 is swapped with j = 3, i = 1 is swapped with j = 2 and the loop terminates.
    // - assume bytes_per_pixel = 3 then i = 0 is swapped with j = 2 and the loop terminates
    _swap_bytes(((uint8_t*)SELF->pixels) + source_offset_bytes + i, ((uint8_t*)SELF->pixels) + target_offset_bytes + (bytes_per_pixel - i - 1));
  }

  return DX_SUCCESS;
}

static void _fill_bn8_gn8_rn8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_BGR_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  dx_size fill_right = fill_offset.left + fill_extend.width;
  dx_size fill_bottom = fill_offset.top + fill_extend.height;
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
  dx_size bytes_per_pixel;
  if (dx_pixel_format_get_number_of_bytes_per_pixel(&bytes_per_pixel, dx_pixel_format_rn8_gn8_bn8)) {
    return;
  }
  for (dx_size y = fill_offset.top; y < fill_bottom; ++y) {
    for (dx_size x = fill_offset.left; x < fill_right; ++x) {
      dx_size offset_pixels = y * image_extend.width + x;
      dx_size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_BGR_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_rn8_gn8_bn8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_N8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  dx_size fill_right = fill_offset.left + fill_extend.width;
  dx_size fill_bottom = fill_offset.top + fill_extend.height;
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
  dx_size bytes_per_pixel;
  if (dx_pixel_format_get_number_of_bytes_per_pixel(&bytes_per_pixel, dx_pixel_format_rn8_gn8_bn8)) {
    return;
  }
  for (dx_size y = fill_offset.top; y < fill_bottom; ++y) {
    for (dx_size x = fill_offset.left; x < fill_right; ++x) {
      dx_size offset_pixels = y * image_extend.width + x;
      dx_size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_RGB_N8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static void _fill_ln8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, uint8_t color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  dx_size fill_right = fill_offset.left + fill_extend.width;
  dx_size fill_bottom = fill_offset.top + fill_extend.height;
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
  dx_size bytes_per_pixel;
  if (dx_pixel_format_get_number_of_bytes_per_pixel(&bytes_per_pixel, dx_pixel_format_ln8)) {
    return;
  }
  for (dx_size y = fill_offset.top; y < fill_bottom; ++y) {
    for (dx_size x = fill_offset.left; x < fill_right; ++x) {
      dx_size offset_pixels = y * image_extend.width + x;
      dx_size offset_bytes = offset_pixels * bytes_per_pixel;
      *(((uint8_t*)pixels) + offset_bytes) = color;
    }
  }
}

static dx_result _swap_columns(dx_asset_image* SELF, dx_size i, dx_size j) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (i >= SELF->width || j >= SELF->width) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  switch (SELF->pixel_format) {
    case dx_pixel_format_ln8:
    case dx_pixel_format_an8_bn8_gn8_rn8:
    case dx_pixel_format_an8_rn8_gn8_bn8:
    case dx_pixel_format_bn8_gn8_rn8:
    case dx_pixel_format_bn8_gn8_rn8_an8:
    case dx_pixel_format_rn8_gn8_bn8:
    case dx_pixel_format_rn8_gn8_bn8_an8:
    {
      // iterate over the y-axis
      for (dx_size y = 0; y < SELF->height; ++y) {
        _swap_pixels(SELF, i, y, j, y, SELF->pixel_format);
      }
    } break;
    default: {
      dx_set_error(DX_ERROR_NOT_IMPLEMENTED);
      return DX_FAILURE;
    } break;
  } 
  return DX_SUCCESS;
}

static dx_result _swap_rows(dx_asset_image* SELF, dx_size i, dx_size j) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (i >= SELF->height || j >= SELF->height) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  switch (SELF->pixel_format) {
    case dx_pixel_format_ln8:
    case dx_pixel_format_an8_bn8_gn8_rn8:
    case dx_pixel_format_an8_rn8_gn8_bn8:
    case dx_pixel_format_bn8_gn8_rn8:
    case dx_pixel_format_bn8_gn8_rn8_an8:
    case dx_pixel_format_rn8_gn8_bn8:
    case dx_pixel_format_rn8_gn8_bn8_an8: {
      // iterate over the x-axis
      for (dx_size x = 0; x < SELF->width; ++x) {
        _swap_pixels(SELF, x, i, x, j, SELF->pixel_format);
      }
    } break;
    default: {
      dx_set_error(DX_ERROR_NOT_IMPLEMENTED);
      return DX_FAILURE;
    } break;
  }
  return DX_SUCCESS;
}

static void dx_asset_image_destruct(dx_asset_image* SELF) {
  dx_inline_object_array_uninitialize(&SELF->operations);
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
  dx_memory_deallocate(SELF->pixels);
  SELF->pixels = NULL;
}

static void dx_asset_image_dispatch_construct(dx_asset_image_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_image_construct(dx_asset_image* SELF,
                                   dx_string* name,
                                   dx_pixel_format pixel_format,
                                   dx_size width,
                                   dx_size height) {
  dx_rti_type* TYPE = dx_asset_image_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->width = width;
  SELF->height = height;
  SELF->pixel_format = pixel_format;
  dx_size overflow;
  dx_size number_of_pixels = dx_mul_sz(SELF->width, SELF->height, &overflow);
  if (overflow) {
    return DX_FAILURE;
  }
  dx_size bytes_per_pixel;
  if (dx_pixel_format_get_number_of_bytes_per_pixel(&bytes_per_pixel, pixel_format)) {
    return DX_FAILURE;
  }
  dx_size number_of_bytes = dx_mul_sz(number_of_pixels, sizeof(uint8_t) * bytes_per_pixel, &overflow);
  if (overflow) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  if (dx_memory_allocate(&SELF->pixels, number_of_bytes)) {
    return DX_FAILURE;
  }
  switch (SELF->pixel_format) {
    case dx_pixel_format_ln8: {
      OFFSET2 fill_offset = { .left = 0, .top = 0 };
      EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
      EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
      _fill_ln8(SELF->pixels, fill_offset, fill_size, image_size, 0);
    } break;
  case dx_pixel_format_bn8_gn8_rn8: {
    DX_BGR_N8 color = { .r = dx_colors_black.r,
                        .g = dx_colors_black.g,
                        .b = dx_colors_black.b, };
    OFFSET2 fill_offset = { .left = 0, .top = 0 };
    EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
    EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
    _fill_bn8_gn8_rn8(SELF->pixels, fill_offset, fill_size, image_size, &color);
  } break;
  case dx_pixel_format_rn8_gn8_bn8: {
    OFFSET2 fill_offset = { .left = 0, .top = 0 };
    EXTEND2 fill_size = { .width = SELF->width, .height = SELF->height };
    EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
    _fill_rn8_gn8_bn8(SELF->pixels, fill_offset, fill_size, image_size, &dx_colors_black);
  } break;
  default: {
    dx_memory_deallocate(SELF->pixels);
    SELF->pixels = NULL;
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  } break;
  };
  SELF->name = name;
  DX_REFERENCE(name);

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    dx_memory_deallocate(SELF->pixels);
    SELF->pixels = NULL;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_image_create(dx_asset_image** RETURN,
                                dx_string* name, 
                                dx_pixel_format pixel_format,
                                dx_size width,
                                dx_size height) {
  DX_CREATE_PREFIX(dx_asset_image)
  if (dx_asset_image_construct(SELF, name, pixel_format, width, height)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

#include "wic-plugin.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static void* read_image_allocate(void* context, size_t n) {
  int old_error = dx_get_error();
  void* p = NULL;
  if (dx_memory_allocate(&p, n)) {
    dx_set_error(old_error);
    return NULL;
  }
  return p;
}

static void read_image_deallocate(void* context, void* p) {
  if (p) {
    dx_memory_deallocate(p);
  }
}

dx_result dx_asset_image_construct_path(dx_asset_image* SELF, dx_string* name, dx_string* path) {
  dx_rti_type* TYPE = dx_asset_image_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  //
  if (dx_string_contains_symbol(path, '\0')) {
    return DX_FAILURE;
  }
  dx_string* format = NULL;
  if (dx_string_create(&format, "${s}\0", sizeof("${s}\0"))) {
    return DX_FAILURE;
  }
  dx_string* path1 = dx_string_printf(format, path);
  DX_UNREFERENCE(format);
  if (!path1) {
    return DX_FAILURE;
  }
  //
  HMODULE dll = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == dll) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  DX_WIC_PLUGIN_READ_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_READ_IMAGE_PROC*)GetProcAddress(dll, DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME);
  if (!procedure) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    FreeLibrary(dll);
    dll = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
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
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
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
      SELF->pixel_format = dx_pixel_format_bn8_gn8_rn8;
    } break;
    case DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8: {
      SELF->pixel_format = dx_pixel_format_rn8_gn8_bn8;
    } break;
    default: {
      dx_memory_deallocate(pixels);
      pixels = NULL;
      return DX_FAILURE;
    } break;
  }
  SELF->pixels = pixels;

  SELF->name = name;
  DX_REFERENCE(name);

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    dx_memory_deallocate(SELF->pixels);
    SELF->pixels = NULL;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_image_create_path(dx_asset_image** RETURN, dx_string* name, dx_string* path) {
  DX_CREATE_PREFIX(dx_asset_image)
  if (dx_asset_image_construct_path(SELF, name, path)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result on_color_fill_image_operation(dx_asset_image* SELF, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_color_fill* image_operation) {
  switch (SELF->pixel_format) {
  case dx_pixel_format_rn8_gn8_bn8: {
    EXTEND2 image_size = { .width = SELF->width, .height = SELF->height };
    _fill_rn8_gn8_bn8(SELF->pixels, offset, extend, image_size, &(DX_ASSETS_COLOR_RGB_N8(image_operation->color->object)->value));
  } break;
  default: {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  } break;
  };
  return DX_SUCCESS;
}

#include "dx/assets/image_operations/checkerboard_pattern_fill_impl.i"
#include "dx/assets/image_operations/mirror_horizontal_impl.i"
#include "dx/assets/image_operations/mirror_vertical_impl.i"

dx_result dx_asset_image_apply(dx_asset_image* SELF,
                               dx_size left,
                               dx_size top,
                               dx_size width,
                               dx_size height,
                               dx_asset_image_operation* image_operation) {
  if (dx_rti_type_is_leq(DX_OBJECT(image_operation)->type, dx_asset_image_operations_color_fill_get_type())) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_color_fill_image_operation(SELF, offset, extend, DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(image_operation));
  }
  if (dx_get_error()) {
    return DX_FAILURE;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(image_operation)->type, dx_asset_image_operations_checkerboard_pattern_fill_get_type())) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_checkerboard_pattern_fill_image_operation(SELF, offset, extend, DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(image_operation));
  }
  if (dx_get_error()) {
    return DX_FAILURE;
  }
  dx_set_error(DX_ERROR_INVALID_ARGUMENT);
  return DX_FAILURE;
}
