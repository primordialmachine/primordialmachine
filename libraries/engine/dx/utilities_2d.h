#if !defined(DX_ENGINE_UTILITIES_2D_H_INCLUDED)
#define DX_ENGINE_UTILITIES_2D_H_INCLUDED

#include "dx/val/context.h"
#include "dx/val/material.h"
#include "dx/val/program_text.h"
#include "dx/font_manager.h"
#include <string.h>

static inline dx_result dx_engine_utilities_2d_create_program_text(dx_val_program_text** RETURN, dx_val_context* context, dx_string* path, dx_string* filename) {
  dx_val_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_val_program_text* program = NULL;
  {
    dx_string* format = NULL;
    if (dx_string_create(&format, "${s}/${s}.vs", sizeof("${s}/${s}.vs") - 1)) {
      return DX_FAILURE;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      return DX_FAILURE;
    }
    if (dx_val_program_text_create_from_file(&vertex_program, DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, p)) {
      DX_UNREFERENCE(p);
      p = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(p);
    p = NULL;
  }
  {
    dx_string* format = NULL;
    if (dx_string_create(&format, "${s}/${s}.fs", sizeof("${s}/${s}.fs") - 1)) {
      DX_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      return DX_FAILURE;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      DX_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      return DX_FAILURE;
    }
    if (dx_val_program_text_create_from_file(&fragment_program, DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT, p)) {
      DX_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      DX_UNREFERENCE(p);
      p = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(p);
    p = NULL;
  }
  {
    if (dx_val_program_text_create(&program, vertex_program, fragment_program)) {
      DX_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      DX_UNREFERENCE(fragment_program);
      fragment_program = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  *RETURN = program;
  return DX_SUCCESS;
}

static inline dx_result dx_engine_utilities_2d_create_program(dx_val_program** RETURN, dx_val_context* val_context, char* path, char* filename) {
  dx_string* path1 = NULL;
  if (dx_string_create(&path1, path, strlen(path))) {
    return DX_FAILURE;
  }
  dx_string* filename1 = NULL;
  if (dx_string_create(&filename1, filename, strlen(filename))) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    return DX_FAILURE;
  }
  dx_val_program_text* val_program_text = NULL;
  if (dx_engine_utilities_2d_create_program_text(&val_program_text, val_context, path1, filename1)) {
    DX_UNREFERENCE(filename1);
    filename1 = NULL;
    //
    DX_UNREFERENCE(path1);
    path1 = NULL;
    //
    return DX_FAILURE;
  }
  DX_UNREFERENCE(filename1);
  filename1 = NULL;
  //
  DX_UNREFERENCE(path1);
  path1 = NULL;
  //
  dx_val_program* val_program;
  if (dx_val_context_create_program(&val_program, val_context, val_program_text)) {
    DX_UNREFERENCE(val_program_text);
    val_program_text = NULL;
    //
    return DX_FAILURE;
  }
  DX_UNREFERENCE(val_program_text);
  val_program_text = NULL;
  *RETURN = val_program;
  return DX_SUCCESS;
}

/// @todo Asset materials must be created via the assets manager.
static inline dx_result dx_engine_utilities_2d_create_material(dx_val_material** RETURN, dx_val_context* context, char const* name) {
  // create the asset material
  dx_string* name_string = NULL;
  if (dx_string_create(&name_string, name, strlen(name))) {
    return DX_FAILURE;
  }
  dx_asset_material* asset_material = NULL;
  if (dx_asset_material_create(&asset_material, name_string)) {
    DX_UNREFERENCE(name_string);
    name_string = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(name_string);
  name_string = NULL;
  dx_assets_color_rgb_n8* ambient_color = NULL;
  DX_RGB_N8 WHITE = { .r = 255, .g = 255, .b = 255 };
  if (dx_assets_color_rgb_n8_create(&ambient_color, &WHITE)) {
    DX_UNREFERENCE(asset_material);
    asset_material = NULL;
    return DX_FAILURE;
  }
  if (dx_asset_material_set_ambient_color(asset_material, ambient_color)) {
    DX_UNREFERENCE(ambient_color);
    ambient_color = NULL;
    DX_UNREFERENCE(asset_material);
    asset_material = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(ambient_color);
  ambient_color = NULL;
  // create the val material
  dx_val_material* val_material = NULL;
  if (dx_val_material_create(&val_material, context, asset_material)) {
    DX_UNREFERENCE(asset_material);
    asset_material = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(asset_material);
  asset_material = NULL;
  *RETURN = val_material;
  return DX_SUCCESS;
}

/// @utility
/// @brief 
/// Create an anonymous image asset from the specified pixel rectangle.
/// Create an anonymous texture asset from the image.
/// @param RETURN A pointer to a <code>dx_asset_texture*</code> variable.
/// @param p A pointer to the pixels.
/// @param w The width of the pixel rectangle.
/// @param h The height of the pixel rectangle.
/// @default-return
/// @defaut-failure
static inline dx_result dx_assets_extensions_create_texture_from_pixels(dx_asset_texture** RETURN, void* pixels, dx_pixel_format pixel_format, dx_n32 width, dx_n32 height) {
  if (width > DX_SIZE_GREATEST || height > DX_SIZE_GREATEST) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  //
  dx_string* image_name = NULL;
  if (dx_string_create(&image_name, "<temporary>", sizeof("<temporary>") - 1)) {
    return DX_FAILURE;
  }
  dx_asset_image* image = NULL;
  if (dx_asset_image_create(&image, image_name, pixel_format, width, height)) {
    DX_UNREFERENCE(image_name);
    image_name = NULL;
    return DX_FAILURE;
  }
  dx_size number_of_bytes_per_pixel;
  if (dx_pixel_format_get_number_of_bytes_per_pixel(&number_of_bytes_per_pixel, pixel_format)) {
    DX_UNREFERENCE(image);
    image = NULL;
    DX_UNREFERENCE(image_name);
    image_name = NULL;
    return DX_FAILURE;
  }
  dx_size overflow;
  dx_size number_of_pixels = dx_mul_sz(width, height, &overflow);
  if (overflow) {
    DX_UNREFERENCE(image);
    image = NULL;
    DX_UNREFERENCE(image_name);
    image_name = NULL;
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }

  dx_size number_of_bytes = dx_mul_sz(number_of_pixels, number_of_bytes_per_pixel, &overflow);
  if (overflow) {
    DX_UNREFERENCE(image);
    image = NULL;
    DX_UNREFERENCE(image_name);
    image_name = NULL;
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_memory_copy(image->pixels, pixels, number_of_bytes);
  dx_asset_reference* image_reference = NULL;
  if (dx_asset_reference_create(&image_reference, image_name)) {
    DX_UNREFERENCE(image);
    image = NULL;
    DX_UNREFERENCE(image_name);
    image_name = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(image_name);
  image_name = NULL;
  image_reference->object = DX_OBJECT(image);
  //
  dx_string* texture_name = NULL;
  if (dx_string_create(&texture_name, "<temporary>", sizeof("<temporary>") - 1)) {
    DX_UNREFERENCE(image_reference);
    image_reference = NULL;
    return DX_FAILURE;
  }
  dx_asset_texture* texture = NULL;
  if (dx_asset_texture_create(&texture, texture_name, image_reference)) {
    DX_UNREFERENCE(texture_name);
    texture_name = NULL;
    DX_UNREFERENCE(image_reference);
    image_reference = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(texture_name);
  texture_name = NULL;
  DX_UNREFERENCE(image_reference);
  image_reference = NULL;
  //
  *RETURN = texture;
  //
  return DX_SUCCESS;
}

static inline dx_result dx_assets_extensions_create_texture_from_glyph(dx_asset_texture** RETURN, dx_font_glyph* glyph) {
  uint32_t width = 0,
           height = 0;
  if (dx_font_glyph_get_size(glyph, &width, &height)) {
    return DX_FAILURE;
  }
  void* pixels = NULL;
  if (dx_font_glyph_get_pixels(glyph, &pixels)) {
    return DX_FAILURE;
  }
  dx_asset_texture* texture = NULL;
  if (dx_assets_extensions_create_texture_from_pixels(&texture, pixels, dx_pixel_format_ln8, width, height)) {
    return DX_FAILURE;
  }
  *RETURN = texture;
  return DX_SUCCESS;
}


#endif // DX_ENGINE_UTILITIES_2D_H_INCLUDED
