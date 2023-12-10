// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "font-loader-plugin.h"

#include "dx/core.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// LoadLibrary, GetProcAddress, FreeLibrary
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static void* allocate(void* context, size_t n) {
  int old_error = Core_getError();
  void* p = NULL;
  if (Core_Memory_allocate(&p, n)) {
    Core_setError(old_error);
    return NULL;
  }
  return p;
}

static void deallocate(void* context, void* p) {
  if (p) {
    free(p);
  }
}

static HMODULE g_library = NULL;
static DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC* g_font_loader_plugin_reference_font = NULL;
static DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC* g_font_loader_plugin_unreference_font = NULL;
static DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC* g_font_loader_plugin_create_font = NULL;
static DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC* g_font_loader_plugin_get_baseline_distance = NULL;
static DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC* g_font_loader_plugin_get_font_size = NULL;
static DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC* g_font_loader_plugin_reference_glyph = NULL;
static DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC* g_font_loader_plugin_unreference_glyph = NULL;
static DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC* g_font_loader_plugin_get_glyph = NULL;
static DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC* g_font_loader_plugin_get_glyph_pixels = NULL;
static DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC* g_font_loader_plugin_get_glyph_advance = NULL;
static DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC* g_font_loader_plugin_get_glyph_bearing = NULL;

static Core_Result startup();

static void shutdown();

static Core_Result startup() {
  shutdown();
  g_library = LoadLibraryA("./" "freetype-plugin" ".dll");
  if (NULL == g_library) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_reference_font = (DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC_NAME);
  if (!g_font_loader_plugin_reference_font) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_unreference_font = (DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC_NAME);
  if (!g_font_loader_plugin_unreference_font) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_create_font = (DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC_NAME);
  if (!g_font_loader_plugin_create_font) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_get_font_size = (DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC_NAME);
  if (!g_font_loader_plugin_get_font_size) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_get_baseline_distance = (DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC_NAME);
  if (!g_font_loader_plugin_get_baseline_distance) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_reference_glyph = (DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC_NAME);
  if (!g_font_loader_plugin_reference_glyph) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_unreference_glyph = (DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC_NAME);
  if (!g_font_loader_plugin_unreference_glyph) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_get_glyph = (DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC_NAME);
  if (!g_font_loader_plugin_get_glyph) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_get_glyph_pixels = (DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC_NAME);
  if (!g_font_loader_plugin_get_glyph_pixels) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_get_glyph_advance = (DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC_NAME);
  if (!g_font_loader_plugin_get_glyph_advance) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  g_font_loader_plugin_get_glyph_bearing = (DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC*)GetProcAddress(g_library, DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC_NAME);
  if (!g_font_loader_plugin_get_glyph_bearing) {
    shutdown();
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static void shutdown() {
  g_font_loader_plugin_get_glyph_bearing = NULL;
  g_font_loader_plugin_get_glyph_advance = NULL;
  g_font_loader_plugin_get_glyph_pixels = NULL;
  g_font_loader_plugin_get_glyph = NULL;
  g_font_loader_plugin_unreference_glyph = NULL;
  g_font_loader_plugin_reference_glyph = NULL;
  g_font_loader_plugin_get_baseline_distance = NULL;
  g_font_loader_plugin_get_font_size = NULL;
  g_font_loader_plugin_create_font = NULL;
  g_font_loader_plugin_unreference_font = NULL;
  g_font_loader_plugin_reference_font = NULL;
  if (g_library) {
    FreeLibrary(g_library);
    g_library = NULL;
  }
}

static Core_Result test_read_font() {
  if (startup()) {
    return Core_Failure;
  }
  dx_font_loader_plugin_font* font = NULL;
  if (g_font_loader_plugin_create_font(&font, "./assets/firacode-regular.ttf", 12, NULL, &allocate, &deallocate)) {
    shutdown();
    return Core_Failure;
  }
  Core_Real32 font_size;
  if (g_font_loader_plugin_get_font_size(&font_size, font)){
    g_font_loader_plugin_unreference_font(font);
    font = NULL;
    shutdown();
    return Core_Failure;
  }
  Core_Real32 baseline_distance;
  if (g_font_loader_plugin_get_baseline_distance(&baseline_distance, font)) {
    g_font_loader_plugin_unreference_font(font);
    font = NULL;
    shutdown();
    return Core_Failure;
  }
  {
    dx_font_loader_plugin_glyph* glyph = NULL;
    if (g_font_loader_plugin_get_glyph(&glyph, font, 'a')) {
      g_font_loader_plugin_unreference_font(font);
      font = NULL;
      shutdown();
      return Core_Failure;
    }
    g_font_loader_plugin_unreference_glyph(glyph);
    glyph = NULL;
  }
  g_font_loader_plugin_unreference_font(font);
  font = NULL;
  shutdown();
  return Core_Success;
}

int main(int argc, char** argv) {
  if (test_read_font()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
