// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "common.h"
#include "font_loader_plugin.h"

#include <ft2build.h>
#include FT_FREETYPE_H

static float f_26_6_to_f(FT_F26Dot6 x) {
  return ((float)x) / 64.f;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct FONT {
  _reference_counter reference_count;
  struct {
    DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context;
    DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate;
    DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate;
  } memory_management;
  FT_Library library;
  FT_Face face;
  struct {
    float font_size;
    /// @brief Distance from one baseline to the next baseline.
    float baseline_distance;
    /// @brief Distance (non-negative) from the baseline to the greatest extend above the baseline of any character.
    float ascender;
    /// @brief Distance (non-positive) from the baseline to the greatest extend below the baseline of any character.
    float descender;
  } cache;
} FONT;

static inline void reference_font_impl(FONT* font_impl) {
  _reference_counter_increment(&font_impl->reference_count);
}

static inline void unreference_font_impl(FONT* font_impl) {
  if (0 == _reference_counter_decrement(&font_impl->reference_count)) {
    FT_Done_Face(font_impl->face);
    FT_Done_FreeType(font_impl->library);
    font_impl->memory_management.deallocate(font_impl->memory_management.context, font_impl);
  }
}

__declspec(dllexport) void
reference_font
  (
    dx_font_loader_plugin_font* font
  )
{
  FONT* font_impl = (FONT*)font;
  reference_font_impl(font_impl);
}

__declspec(dllexport) void
unreference_font
  (
    dx_font_loader_plugin_font* font
  )
{
  FONT* font_impl = (FONT*)font;
  unreference_font_impl(font_impl);
}

static dx_font_loader_plugin_result create_face(FONT* font_impl, char const *path, uint8_t font_size) {
  FT_Error error;
  error = FT_New_Face(font_impl->library, path, 0, &font_impl->face);
  if (error) {
    return DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_FONT_FILE;
  }
  error = FT_Set_Pixel_Sizes(font_impl->face, 0, font_size);
  if (error) {
    FT_Done_Face(font_impl->face);
    return DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_FONT_FILE;
  }
  // cache
  // - font size,
  // - baseline distance
  font_impl->cache.font_size = (float)font_size;
  font_impl->cache.baseline_distance = f_26_6_to_f(font_impl->face->size->metrics.height);
  font_impl->cache.ascender = f_26_6_to_f(font_impl->face->size->metrics.ascender);
  font_impl->cache.descender = f_26_6_to_f(font_impl->face->size->metrics.descender);
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
create_font
  (
    dx_font_loader_plugin_font** font,
    char const* path,
    uint8_t font_size,
    DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context,
    DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate,
    DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate
  )
{
  FONT* font_impl = allocate(context, sizeof(FONT));
  if (!font_impl) {
    return DX_FONT_LOADER_PLUGIN_ALLOCATION_FAILED;
  }
  font_impl->memory_management.context = context;
  font_impl->memory_management.allocate = allocate;
  font_impl->memory_management.deallocate = deallocate;
  FT_Error error = FT_Init_FreeType(&font_impl->library);
  if (error) {
    deallocate(context, font_impl);
    font_impl = NULL;
    return DX_FONT_LOADER_PLUGIN_FAILED_TO_INITIALIZE_BACKEND_LIBRARY;
  }
  dx_font_loader_plugin_result result = create_face(font_impl, path, font_size);
  if (result) {
    FT_Done_FreeType(font_impl->library);
    deallocate(context, font_impl);
    font_impl = NULL;
    return result;
  }
  font_impl->reference_count = 1;
  *font = (dx_font_loader_plugin_font*)font_impl;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_font_size
  (
    float* _return,
    dx_font_loader_plugin_font* font
  )
{
  if (!_return || !font) {
    return DX_FONT_LOADER_PLUGIN_INVALID_ARGUMENT;
  }
  FONT* font_impl = (FONT*)font;
  *_return = font_impl->cache.font_size;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_baseline_distance
  (
    float* _return,
    dx_font_loader_plugin_font* font
  ) 
{ 
  if (!_return || !font) {
    return DX_FONT_LOADER_PLUGIN_INVALID_ARGUMENT;
  }
  FONT* font_impl = (FONT*)font;
  *_return = font_impl->cache.baseline_distance;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_ascender
  (
    float* _return,
    dx_font_loader_plugin_font* font
  )
{
  if (!_return || !font) {
    return DX_FONT_LOADER_PLUGIN_INVALID_ARGUMENT;
  }
  FONT* font_impl = (FONT*)font;
  *_return = font_impl->cache.ascender;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_descender
  (
    float* _return,
    dx_font_loader_plugin_font* font
  )
{
  if (!_return || !font) {
    return DX_FONT_LOADER_PLUGIN_INVALID_ARGUMENT;
  }
  FONT* font_impl = (FONT*)font;
  *_return = font_impl->cache.descender;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct GLYPH {
  _reference_counter reference_count;
  
  FONT* font;
  
  /// @brief A pointer to an array of width*height pixels where width is the width, in pixels, of the bitmap and height is the height, in pixels, of the bitmap.
  void* pixels;
  /// @brief The width, in pixels, of the bitmap.
  uint32_t width;
  /// @brief The height, in pixels, of the bitmap.
  uint32_t height;

  /// @brief Unicode code point.
  uint32_t codepoint;

  /// @brief Add bearingx pixels to the cursor position before rendering.
  float bearingx;
  /// @brief Add height - bearingy pixels to the cursor position before rendering.
  float bearingy;
  /// @brief Advance in pixels.
  float advancex;
  /// @brief Advance in pixels.
  float advancey;

} GLYPH;

__declspec(dllexport) void
reference_glyph
  (
    dx_font_loader_plugin_glyph* glyph
  )
{
  GLYPH* glyph_impl = (GLYPH*)glyph;
  _reference_counter_increment(&glyph_impl->reference_count);
}

__declspec(dllexport) void
unreference_glyph
  (
    dx_font_loader_plugin_glyph* glyph
  )
{
  GLYPH* glyph_impl = (GLYPH*)glyph;
  if (0 == _reference_counter_decrement(&glyph_impl->reference_count)) {
    FONT* font_impl = glyph_impl->font;
    font_impl->memory_management.deallocate(font_impl->memory_management.context, glyph_impl->pixels);
    glyph_impl->pixels = NULL;
    font_impl->memory_management.deallocate(font_impl->memory_management.context, glyph_impl);
    unreference_font_impl(font_impl);
  }
}

__declspec(dllexport) dx_font_loader_plugin_result
get_glyph
  (
    dx_font_loader_plugin_glyph** _return,
    dx_font_loader_plugin_font* font,
    dx_font_loader_plugin_utf8_codepoint codepoint
  )
{
  FONT* font_impl = (FONT*)font;
  GLYPH* glyph_impl = font_impl->memory_management.allocate(font_impl->memory_management.context, sizeof(GLYPH));
  if (!glyph_impl) {
    return DX_FONT_LOADER_PLUGIN_ALLOCATION_FAILED;
  }
  FT_Error error = FT_Load_Char(font_impl->face, codepoint, FT_LOAD_RENDER);
  if (error) {
    font_impl->memory_management.deallocate(font_impl->memory_management.context, glyph_impl);
    glyph_impl = NULL;
    return DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_GLYPH;
  }
  // convert_F26Dot6_to_float
  glyph_impl->advancex = f_26_6_to_f(font_impl->face->glyph->advance.x);
  glyph_impl->advancey = f_26_6_to_f(font_impl->face->glyph->advance.y);
  glyph_impl->bearingx = font_impl->face->glyph->bitmap_left;
  glyph_impl->bearingy = font_impl->face->glyph->bitmap_top;
  glyph_impl->width = font_impl->face->glyph->bitmap.width;
  glyph_impl->height = font_impl->face->glyph->bitmap.rows;
  
  // if (NOT (a == 0 || a * b <= M))
  // if (NOT (a == 0 || b <= M / a))
  // if (a != 0 && (b > M / a)) 
  if (font_impl->face->glyph->bitmap.width > SIZE_MAX || font_impl->face->glyph->bitmap.rows > SIZE_MAX) {
    font_impl->memory_management.deallocate(font_impl->memory_management.context, glyph_impl);
    glyph_impl = NULL;
    return DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_GLYPH;
  }
  size_t w = (size_t)font_impl->face->glyph->bitmap.width;
  size_t h = (size_t)font_impl->face->glyph->bitmap.rows;
  if (h != 0 && SIZE_MAX / h < w) {
    font_impl->memory_management.deallocate(font_impl->memory_management.context, glyph_impl);
    glyph_impl = NULL;
    return DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_GLYPH;  
  }
  size_t number_of_pixels = w * h;
  glyph_impl->pixels =  malloc(number_of_pixels > 0 ? number_of_pixels : 1);
  if (!glyph_impl->pixels) {
    font_impl->memory_management.deallocate(font_impl->memory_management.context, glyph_impl);
    glyph_impl = NULL;    
  }
  memcpy(glyph_impl->pixels, font_impl->face->glyph->bitmap.buffer, number_of_pixels);
  // 
  glyph_impl->font = font;
  reference_font_impl(font);
  
  glyph_impl->reference_count = 1;
  *_return = (dx_font_loader_plugin_glyph*)glyph_impl;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_glyph_pixels
  (
    dx_font_loader_plugin_glyph* glyph,
    void** pixels,
    uint32_t *width,
    uint32_t *height
  )
{
  GLYPH* glyph_impl = (GLYPH*)glyph;
  *pixels = glyph_impl->pixels;
  *width = glyph_impl->width;
  *height = glyph_impl->height;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_glyph_advance
  (
    dx_font_loader_plugin_glyph* glyph,
    float* advance_x,
    float* advance_y
  )
{
  GLYPH* glyph_impl = (GLYPH*)glyph;
  *advance_x = glyph_impl->advancex;
  *advance_y = glyph_impl->advancey;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

__declspec(dllexport) dx_font_loader_plugin_result
get_glyph_bearing
  (
    dx_font_loader_plugin_glyph* glyph,
    float* bearing_x,
    float* bearing_y
  )
{
  GLYPH* glyph_impl = (GLYPH*)glyph;
  *bearing_x = glyph_impl->bearingx;
  *bearing_y = glyph_impl->bearingy;
  return DX_FONT_LOADER_PLUGIN_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
