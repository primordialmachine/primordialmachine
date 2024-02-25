#if !defined(CORE_DEFAULTFONTSYSTEM_FONTLOADERPLUGIN_H_INCLUDED)
#define CORE_DEFAULTFONTSYSTEM_FONTLOADERPLUGIN_H_INCLUDED

#include "dx/core.h"

#if Core_OperatingSystem_Windows == Core_OperatingSystem

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "font-loader-plugin.h"

#define LIBRARY_FILENAME "./" "freetype-plugin" ".dll"

static inline void* load_library(char const* path) {
  return (void*)LoadLibraryA(path);
}

static inline void unload_library(void* handle) {
  FreeLibrary((HMODULE)handle);
}

static inline void* link_function(void* library, char const* name) {
  return GetProcAddress(library, name);
}

#else

#error("environment not (yet) supported")

#endif

typedef struct FontLoaderPlugin {
  void* library;
  DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC* plugin_reference_font;
  DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC* plugin_unreference_font;
  DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC* plugin_create_font;
  DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC* plugin_get_baseline_distance;
  DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC* plugin_get_font_size;
  DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC* plugin_reference_glyph;
  DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC* plugin_unreference_glyph;
  DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC* plugin_get_glyph;
  DX_FONT_LOADER_PLUGIN_GET_GLYPH_SIZE_PROC* plugin_get_glyph_size;
  DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC* plugin_get_glyph_pixels;
  DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC* plugin_get_glyph_advance;
  DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC* plugin_get_glyph_bearing;
  DX_FONT_LOADER_PLUGIN_GET_ASCENDER_PROC* plugin_get_ascender;
  DX_FONT_LOADER_PLUGIN_GET_DESCENDER_PROC* plugin_get_descender;
} FontLoaderPlugin;

static inline Core_Result FontLoaderPlugin_uninitialize(FontLoaderPlugin* SELF) {

#define LINK(V,F) \
  (SELF)->V = NULL;

  LINK(plugin_reference_font, DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC);
  LINK(plugin_unreference_font, DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC);
  LINK(plugin_create_font, DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC);
  LINK(plugin_get_baseline_distance, DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC);
  LINK(plugin_get_font_size, DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC);
  LINK(plugin_reference_glyph, DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC);
  LINK(plugin_unreference_glyph, DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC);
  LINK(plugin_get_glyph, DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC);
  LINK(plugin_get_glyph_pixels, DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC);
  LINK(plugin_get_glyph_advance, DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC);
  LINK(plugin_get_glyph_bearing, DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC);
  LINK(plugin_get_ascender, DX_FONT_LOADER_PLUGIN_GET_ASCENDER_PROC);
  LINK(plugin_get_descender, DX_FONT_LOADER_PLUGIN_GET_DESCENDER_PROC);

#undef LINK

  unload_library(SELF->library);
  SELF->library = NULL;

  return Core_Success;
}

static inline Core_Result FontLoaderPlugin_initialize(FontLoaderPlugin* SELF) {

  SELF->library = load_library(LIBRARY_FILENAME);
  if (!SELF->library) {
    return Core_Failure;
  }

#define LINK(V,F) \
  (SELF)->V = (F*)link_function(SELF->library, F##_NAME); \
  if (!(SELF)->V) { \
    unload_library(SELF->library); \
    SELF->library = NULL; \
    return Core_Failure; \
  }

  LINK(plugin_reference_font, DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC);
  LINK(plugin_unreference_font, DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC);
  LINK(plugin_create_font, DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC);
  LINK(plugin_get_baseline_distance, DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC);
  LINK(plugin_get_font_size, DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC);
  LINK(plugin_reference_glyph, DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC);
  LINK(plugin_unreference_glyph, DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC);
  LINK(plugin_get_glyph, DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC);
  LINK(plugin_get_glyph_size, DX_FONT_LOADER_PLUGIN_GET_GLYPH_SIZE_PROC);
  LINK(plugin_get_glyph_pixels, DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC);
  LINK(plugin_get_glyph_advance, DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC);
  LINK(plugin_get_glyph_bearing, DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC);
  LINK(plugin_get_ascender, DX_FONT_LOADER_PLUGIN_GET_ASCENDER_PROC);
  LINK(plugin_get_descender, DX_FONT_LOADER_PLUGIN_GET_DESCENDER_PROC)

  #undef LINK

    return Core_Success;
}

#endif // CORE_DEFAULTFONTSYSTEM_FONTLOADERPLUGIN_H_INCLUDED
