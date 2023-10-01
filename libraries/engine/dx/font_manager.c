#include "dx/font_manager.h"

#include <malloc.h>

#if DX_OPERATING_SYSTEM_WINDOWS == DX_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

  #include "font-loader-plugin.h"

  #define LIBRARY_FILENAME "./" "freetype-plugin" ".dll"

  static void* load_library(char const* path) {
    return (void*)LoadLibraryA(path);
  }

  static void unload_library(void* handle) {
    FreeLibrary((HMODULE)handle);
  }

  static void* link_function(void* library, char const* name) {
    return GetProcAddress(library, name);
  }

#else

  #error("environment not (yet) supported")

#endif

typedef struct _backend_impl {
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
} _backend_impl;

static dx_result _backend_impl_uninitialize(_backend_impl* SELF) {

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

  return DX_SUCCESS;
}

static dx_result _backend_impl_initialize(_backend_impl* SELF) {

  SELF->library = load_library(LIBRARY_FILENAME);
  if (!SELF->library) {
    return DX_FAILURE;
  }

#define LINK(V,F) \
  (SELF)->V = (F*)link_function(SELF->library, F##_NAME); \
  if (!(SELF)->V) { \
    unload_library(SELF->library); \
    SELF->library = NULL; \
    return DX_FAILURE; \
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

  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.font_key",
                      dx_font_key,
                      dx_object);

static void dx_font_key_destruct(dx_font_key* SELF) {
  DX_UNREFERENCE(SELF->font_file);
  SELF->font_file = NULL;
}

static void dx_font_key_dispatch_construct(dx_font_key_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_font_key_construct(dx_font_key* SELF, dx_string* font_file, dx_n8 font_size) {
  dx_rti_type* TYPE = dx_font_key_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  
  SELF->font_file = font_file;
  DX_REFERENCE(font_file);
  SELF->font_size = font_size;

  SELF->hash_value = dx_string_get_hash_value(font_file);
  SELF->hash_value = (SELF->hash_value * 37) + (dx_size)SELF->font_size;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_font_key_create(dx_font_key** RETURN, dx_string* font_file, dx_n8 font_size) {
  DX_CREATE_PREFIX(dx_font_key)
  if (dx_font_key_construct(SELF, font_file, font_size)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_font_key_is_equal_to(dx_bool* RETURN, dx_font_key* SELF, dx_font_key* other) {
  if (SELF == other) {
    *RETURN = true;
    return DX_SUCCESS;
  }
  if (SELF->font_size == other->font_size) {
    if (dx_string_is_equal_to(SELF->font_file, other->font_file)) {
      *RETURN = true;
      return DX_SUCCESS;
    }
  }
  *RETURN = false;
  return DX_SUCCESS;
}

dx_result dx_font_key_get_hash_value(dx_size* RETURN, dx_font_key* SELF) {
  *RETURN = SELF->hash_value;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.font_glyph",
                      dx_font_glyph,
                      dx_object);

static void dx_font_glyph_destruct(dx_font_glyph* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  pimpl->plugin_unreference_glyph((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl);
  SELF->glyph_pimpl = NULL;
  DX_UNREFERENCE(SELF->font);
  SELF->font = NULL;
}

static void dx_font_glyph_dispatch_construct(dx_font_glyph_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_font_glyph_construct(dx_font_glyph* SELF, uint32_t code_point, dx_font* font) {
  dx_rti_type* TYPE = dx_font_glyph_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  SELF->code_point = code_point;

  SELF->font = font;
  DX_REFERENCE(font);

  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph(&SELF->glyph_pimpl, (dx_font_loader_plugin_font*)SELF->font->font_pimpl, SELF->code_point)) {
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_font_glyph_create(dx_font_glyph** RETURN, uint32_t code_point, dx_font* font) {
  DX_CREATE_PREFIX(dx_font_glyph)
  if (dx_font_glyph_construct(SELF, code_point, font)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_font_glyph_get_glyph_advance(dx_font_glyph* SELF, dx_f32* advance_x, dx_f32* advance_y) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_advance((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, advance_x, advance_y)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_font_glyph_get_glyph_bearing(dx_font_glyph* SELF, dx_f32* bearing_x, dx_f32* bearing_y) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_bearing((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, bearing_x, bearing_y)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_font_glyph_get_size(dx_font_glyph* SELF, uint32_t* size_x, uint32_t* size_y) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_size((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, size_x, size_y)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}


dx_result dx_font_glyph_get_pixels(dx_font_glyph* SELF, void** pixels) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_pixels((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, pixels)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_font_glyph_get_texture_coordinates(dx_font_glyph* SELF, DX_RECT2_F32 *texture_coordinates) {
  dx_rect2_f32_set(texture_coordinates, 0.f, 0.f, 1.f, 1.f);
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.font",
                      dx_font,
                      dx_object);

static void dx_font_destruct(dx_font* SELF) {
  dx_inline_pointer_hashmap_remove(&SELF->font_manager->fonts, SELF->key);
  
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  pimpl->plugin_unreference_font((dx_font_loader_plugin_font*)SELF->font_pimpl);
  SELF->font_pimpl = NULL;

  DX_UNREFERENCE(SELF->key);
  SELF->key = NULL;

  DX_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;
}

static void dx_font_dispatch_construct(dx_font_dispatch* SELF)
{/*Intentionally empty.*/}

static void* allocate_callback(void* context, size_t number_of_bytes) {
  return malloc(number_of_bytes > 0 ? number_of_bytes : 1);
}

static void deallocate_callback(void* context, void* bytes) {
  if (bytes) {
    free(bytes);
  }
}

dx_result dx_font_construct(dx_font* SELF, dx_font_key* key, dx_font_manager* font_manager) {
  dx_rti_type* TYPE = dx_font_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (dx_string_contains_symbol(key->font_file, '\0')) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_string* zero_terminator = NULL;
  if (dx_string_create(&zero_terminator, "", sizeof(""))) {
    return DX_FAILURE;
  }
  dx_string* format = NULL;
  if (dx_string_create(&format, "${s}${s}", sizeof("${s}${s}") - 1)) {
    DX_UNREFERENCE(zero_terminator);
    zero_terminator = NULL;
    return DX_FAILURE;
  }
  dx_string *path = dx_string_printf(format, key->font_file, zero_terminator);
  DX_UNREFERENCE(zero_terminator);
  zero_terminator = NULL;
  DX_UNREFERENCE(format);
  format = NULL;
  if (!path) {
    return DX_FAILURE;
  }
  void const* path_bytes = NULL;
  if (dx_string_get_bytes(&path_bytes, path)) {
    DX_UNREFERENCE(path);
    path = NULL;
    return DX_FAILURE;
  }
  _backend_impl* pimpl = (_backend_impl*)font_manager->pimpl;
  if (pimpl->plugin_create_font((dx_font_loader_plugin_font**)&SELF->font_pimpl, path_bytes, key->font_size, NULL, &allocate_callback, &deallocate_callback)) {
    DX_UNREFERENCE(path);
    path = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(path);
  path = NULL;

  SELF->key = key;
  DX_REFERENCE(key);

  SELF->font_manager = font_manager;
  DX_REFERENCE(font_manager);

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_font_create(dx_font** RETURN, dx_font_key* key, dx_font_manager* font_manager) {
  DX_CREATE_PREFIX(dx_font)
  if (dx_font_construct(SELF, key, font_manager)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_font_get_baseline_distance(dx_f32* RETURN, dx_font* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  if (pimpl->plugin_get_baseline_distance(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_font_get_ascender(dx_f32* RETURN, dx_font* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  if (pimpl->plugin_get_ascender(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_font_get_descender(dx_f32* RETURN, dx_font* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  if (pimpl->plugin_get_descender(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_font_manager* g_font_manager = NULL;

DX_DEFINE_OBJECT_TYPE("dx.font_manager",
                      dx_font_manager,
                      dx_object);

static void dx_font_manager_destruct(dx_font_manager* SELF) {

  _backend_impl_uninitialize((_backend_impl*)SELF->pimpl);
  free(SELF->pimpl);
  SELF->pimpl = NULL;

  DX_UNREFERENCE(SELF->context);
  SELF->context = NULL;

  g_font_manager = NULL;
}

static void dx_font_manager_dispatch_construct(dx_font_manager_dispatch* SELF)
{/*Intentionally empty.*/}

static dx_result compare_font_key(dx_bool* RETURN, dx_font_key**a, dx_font_key** b) {
  return dx_font_key_is_equal_to(RETURN, *a, *b);
}

static dx_result hash_font_key(dx_size* RETURN, dx_font_key * *a) {
  return dx_font_key_get_hash_value(RETURN, *a);
}

static void font_key_added(dx_font_key** a) {
  DX_REFERENCE(*a);
}

static void font_key_removed(dx_font_key** a) {
  DX_UNREFERENCE(*a);
}

dx_result dx_font_manager_construct(dx_font_manager* SELF, dx_val_context* context) {
  dx_rti_type* TYPE = dx_font_manager_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  DX_INLINE_POINTER_HASHMAP_CONFIGURATION configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*) & compare_font_key,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*) & hash_font_key,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*) & font_key_added,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*) & font_key_removed,
    .value_added_callback = NULL,
    .value_removed_callback = NULL,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->fonts, &configuration)) {
    return DX_FAILURE;
  }

  SELF->pimpl = malloc(sizeof(_backend_impl));
  if (!SELF->pimpl) {
    dx_inline_pointer_hashmap_uninitialize(&SELF->fonts);
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  if (_backend_impl_initialize((_backend_impl*)SELF->pimpl)) {
    free(SELF->pimpl);
    SELF->pimpl = NULL;
    dx_inline_pointer_hashmap_uninitialize(&SELF->fonts);
    return DX_FAILURE;
  }

  SELF->context = context;
  DX_REFERENCE(context);

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_font_manager_create(dx_font_manager** RETURN, dx_val_context* context) {
  if (g_font_manager) {
    DX_REFERENCE(g_font_manager);
    *RETURN = g_font_manager;
    return DX_SUCCESS;
  } else {
    DX_CREATE_PREFIX(dx_font_manager)
    if (dx_font_manager_construct(SELF, context)) {
      DX_UNREFERENCE(SELF);
      SELF = NULL;
      return DX_FAILURE;
    }
    g_font_manager = SELF;
    *RETURN = SELF;
    return DX_SUCCESS;
  }
}

dx_result dx_font_manager_get_or_create_font(dx_font** RETURN, dx_font_manager* SELF, dx_string* font_file, dx_n8 font_size) {
  dx_font_key* font_key = NULL;
  if (dx_font_key_create(&font_key, font_file, font_size)) {
    return DX_FAILURE;
  }
  dx_font* font = NULL;
  if (dx_inline_pointer_hashmap_get(&font, &SELF->fonts, font_key)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
    }
  }
  if (!font) {
    if (dx_font_create(&font, font_key, SELF)) {
      return DX_FAILURE;
    }
    if (dx_inline_pointer_hashmap_set(&SELF->fonts, font_key, font)) {
      DX_UNREFERENCE(font_key);
      font_key = NULL;
      DX_UNREFERENCE(font);
      font = NULL;
      return DX_FAILURE;
    }
  }
  DX_UNREFERENCE(font_key);
  font_key = NULL;
  DX_REFERENCE(font);
  *RETURN = font;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
