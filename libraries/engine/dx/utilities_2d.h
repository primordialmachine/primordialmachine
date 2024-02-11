#if !defined(DX_ENGINE_UTILITIES_2D_H_INCLUDED)
#define DX_ENGINE_UTILITIES_2D_H_INCLUDED

#include "dx/val/context.h"
#include "dx/val/material.h"
#include "dx/val/program_text.h"
#include "dx/font_manager.h"
#include <string.h>

static inline Core_Result dx_engine_utilities_2d_create_program_text(dx_val_program_text** RETURN, dx_val_context* context, Core_String* path, Core_String* filename) {
  dx_val_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_val_program_text* program = NULL;
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.vs", sizeof("${s}/${s}.vs") - 1)) {
      return Core_Failure;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      CORE_UNREFERENCE(format);
      format = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&vertex_program, DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, p)) {
      CORE_UNREFERENCE(p);
      p = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(p);
    p = NULL;
  }
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.fs", sizeof("${s}/${s}.fs") - 1)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      return Core_Failure;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      CORE_UNREFERENCE(format);
      format = NULL;
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&fragment_program, DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT, p)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      CORE_UNREFERENCE(p);
      p = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(p);
    p = NULL;
  }
  {
    if (dx_val_program_text_create(&program, vertex_program, fragment_program)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      CORE_UNREFERENCE(fragment_program);
      fragment_program = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    CORE_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  *RETURN = program;
  return Core_Success;
}

static inline Core_Result dx_engine_utilities_2d_create_program(dx_val_program** RETURN, dx_val_context* val_context, char* path, char* filename) {
  Core_String* path1 = NULL;
  if (Core_String_create(&path1, path, strlen(path))) {
    return Core_Failure;
  }
  Core_String* filename1 = NULL;
  if (Core_String_create(&filename1, filename, strlen(filename))) {
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    return Core_Failure;
  }
  dx_val_program_text* val_program_text = NULL;
  if (dx_engine_utilities_2d_create_program_text(&val_program_text, val_context, path1, filename1)) {
    CORE_UNREFERENCE(filename1);
    filename1 = NULL;
    //
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    //
    return Core_Failure;
  }
  CORE_UNREFERENCE(filename1);
  filename1 = NULL;
  //
  CORE_UNREFERENCE(path1);
  path1 = NULL;
  //
  dx_val_program* val_program;
  if (dx_val_context_create_program(&val_program, val_context, val_program_text)) {
    CORE_UNREFERENCE(val_program_text);
    val_program_text = NULL;
    //
    return Core_Failure;
  }
  CORE_UNREFERENCE(val_program_text);
  val_program_text = NULL;
  *RETURN = val_program;
  return Core_Success;
}

/// @todo Asset materials must be created via the assets manager.
static inline Core_Result dx_engine_utilities_2d_create_material(dx_val_material** RETURN, dx_val_context* context, char const* name) {
  // create the asset material
  Core_String* name_string = NULL;
  if (Core_String_create(&name_string, name, strlen(name))) {
    return Core_Failure;
  }
  dx_assets_material* material_asset = NULL;
  if (dx_assets_material_create(&material_asset, name_string)) {
    CORE_UNREFERENCE(name_string);
    name_string = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(name_string);
  name_string = NULL;
  Core_Assets_ColorRgbN8* ambient_color = NULL;
  Core_InlineRgbN8 WHITE = { .r = 255, .g = 255, .b = 255 };
  if (Core_Assets_ColorRgbN8_create(&ambient_color, &WHITE)) {
    CORE_UNREFERENCE(material_asset);
    material_asset = NULL;
    return Core_Failure;
  }
  if (dx_assets_material_set_ambient_color(material_asset, ambient_color)) {
    CORE_UNREFERENCE(ambient_color);
    ambient_color = NULL;
    CORE_UNREFERENCE(material_asset);
    material_asset = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(ambient_color);
  ambient_color = NULL;
  // create the val material
  dx_val_material* val_material = NULL;
  if (dx_val_material_create(&val_material, context, material_asset)) {
    CORE_UNREFERENCE(material_asset);
    material_asset = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(material_asset);
  material_asset = NULL;
  *RETURN = val_material;
  return Core_Success;
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
static inline Core_Result dx_assets_extensions_create_texture_from_pixels(dx_assets_texture** RETURN, void* pixels, Core_PixelFormat pixel_format, Core_Natural32 width, Core_Natural32 height) {
  if (width > Core_Size_Greatest || height > Core_Size_Greatest) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  //
  Core_String* image_name = NULL;
  if (Core_String_create(&image_name, "<temporary>", sizeof("<temporary>") - 1)) {
    return Core_Failure;
  }
  dx_assets_image* image = NULL;
  if (dx_assets_image_create(&image, image_name, pixel_format, width, height)) {
    CORE_UNREFERENCE(image_name);
    image_name = NULL;
    return Core_Failure;
  }
  Core_Size number_of_bytes_per_pixel;
  if (Core_PixelFormat_getNumberOfBytes(&number_of_bytes_per_pixel, pixel_format)) {
    CORE_UNREFERENCE(image);
    image = NULL;
    CORE_UNREFERENCE(image_name);
    image_name = NULL;
    return Core_Failure;
  }
  Core_Size overflow;
  Core_Size number_of_pixels;
  Core_safeMulSz(&number_of_pixels, width, height, &overflow); // must succeed
  if (overflow) {
    CORE_UNREFERENCE(image);
    image = NULL;
    CORE_UNREFERENCE(image_name);
    image_name = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }

  Core_Size number_of_bytes;
  Core_safeMulSz(&number_of_bytes, number_of_pixels, number_of_bytes_per_pixel, &overflow); // must succeed
  if (overflow) {
    CORE_UNREFERENCE(image);
    image = NULL;
    CORE_UNREFERENCE(image_name);
    image_name = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Memory_copy(image->pixels, pixels, number_of_bytes);
  Core_Assets_Ref* image_reference = NULL;
  if (Core_Assets_Ref_create(&image_reference, image_name)) {
    CORE_UNREFERENCE(image);
    image = NULL;
    CORE_UNREFERENCE(image_name);
    image_name = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(image_name);
  image_name = NULL;
  image_reference->object = CORE_OBJECT(image);
  //
  Core_String* texture_name = NULL;
  if (Core_String_create(&texture_name, "<temporary>", sizeof("<temporary>") - 1)) {
    CORE_UNREFERENCE(image_reference);
    image_reference = NULL;
    return Core_Failure;
  }
  dx_assets_texture* texture = NULL;
  if (dx_assets_texture_create(&texture, texture_name, image_reference)) {
    CORE_UNREFERENCE(texture_name);
    texture_name = NULL;
    CORE_UNREFERENCE(image_reference);
    image_reference = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(texture_name);
  texture_name = NULL;
  CORE_UNREFERENCE(image_reference);
  image_reference = NULL;
  //
  *RETURN = texture;
  //
  return Core_Success;
}

static inline Core_Result dx_assets_extensions_create_texture_from_glyph(dx_assets_texture** RETURN, dx_font_glyph* glyph) {
  uint32_t width = 0,
           height = 0;
  if (dx_font_glyph_get_size(glyph, &width, &height)) {
    return Core_Failure;
  }
  void* pixels = NULL;
  if (dx_font_glyph_get_pixels(glyph, &pixels)) {
    return Core_Failure;
  }
  dx_assets_texture* texture = NULL;
  if (dx_assets_extensions_create_texture_from_pixels(&texture, pixels, Core_PixelFormat_L8, width, height)) {
    return Core_Failure;
  }
  *RETURN = texture;
  return Core_Success;
}


#endif // DX_ENGINE_UTILITIES_2D_H_INCLUDED
