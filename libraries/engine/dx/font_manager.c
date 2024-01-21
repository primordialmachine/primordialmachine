#include "dx/font_manager.h"

#include <malloc.h>

#if Core_OperatingSystem_Windows == Core_OperatingSystem

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

static Core_Result _backend_impl_uninitialize(_backend_impl* SELF) {

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

static Core_Result _backend_impl_initialize(_backend_impl* SELF) {

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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.font_key",
                      dx_font_key,
                      Core_Object);

static void dx_font_key_destruct(dx_font_key* SELF) {
  CORE_UNREFERENCE(SELF->font_file);
  SELF->font_file = NULL;
}

static void dx_font_key_constructDispatch(dx_font_key_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_font_key_construct(dx_font_key* SELF, Core_String* font_file, Core_Natural8 font_size) {
  DX_CONSTRUCT_PREFIX(dx_font_key);
  
  SELF->font_file = font_file;
  CORE_REFERENCE(font_file);

  SELF->font_size = font_size;

  if (Core_String_getHashValue(&SELF->hash_value, font_file)) {
    CORE_UNREFERENCE(SELF->font_file);
    SELF->font_file = NULL;
    return Core_Failure;
  }

  SELF->hash_value = (SELF->hash_value * 37) + (Core_Size)SELF->font_size;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_font_key_create(dx_font_key** RETURN, Core_String* font_file, Core_Natural8 font_size) {
  DX_CREATE_PREFIX(dx_font_key)
  if (dx_font_key_construct(SELF, font_file, font_size)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_font_key_is_equal_to(Core_Boolean* RETURN, dx_font_key* SELF, dx_font_key* other) {
  if (SELF == other) {
    *RETURN = true;
    return Core_Success;
  }
  if (SELF->font_size == other->font_size) {
    Core_Boolean isEqualTo = Core_False;
    if (Core_String_isEqualTo(&isEqualTo, SELF->font_file, other->font_file)) {
      return Core_Failure;
    }
    if (isEqualTo) {
      *RETURN = true;
      return Core_Success;
    }
  }
  *RETURN = false;
  return Core_Success;
}

Core_Result dx_font_key_get_hash_value(Core_Size* RETURN, dx_font_key* SELF) {
  *RETURN = SELF->hash_value;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.font_glyph",
                      dx_font_glyph,
                      Core_Object);

static void dx_font_glyph_destruct(dx_font_glyph* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  pimpl->plugin_unreference_glyph((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl);
  SELF->glyph_pimpl = NULL;
  CORE_UNREFERENCE(SELF->font);
  SELF->font = NULL;
}

static void dx_font_glyph_constructDispatch(dx_font_glyph_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_font_glyph_construct(dx_font_glyph* SELF, uint32_t code_point, dx_font* font) {
  DX_CONSTRUCT_PREFIX(dx_font_glyph);

  SELF->code_point = code_point;

  SELF->font = font;
  CORE_REFERENCE(font);

  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph(&SELF->glyph_pimpl, (dx_font_loader_plugin_font*)SELF->font->font_pimpl, SELF->code_point)) {
    CORE_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_font_glyph_create(dx_font_glyph** RETURN, uint32_t code_point, dx_font* font) {
  DX_CREATE_PREFIX(dx_font_glyph)
  if (dx_font_glyph_construct(SELF, code_point, font)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_font_glyph_get_glyph_advance(dx_font_glyph* SELF, Core_Real32* advance_x, Core_Real32* advance_y) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_advance((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, advance_x, advance_y)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_font_glyph_get_glyph_bearing(dx_font_glyph* SELF, Core_Real32* bearing_x, Core_Real32* bearing_y) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_bearing((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, bearing_x, bearing_y)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_font_glyph_get_size(dx_font_glyph* SELF, uint32_t* size_x, uint32_t* size_y) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_size((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, size_x, size_y)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}


Core_Result dx_font_glyph_get_pixels(dx_font_glyph* SELF, void** pixels) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font->font_manager->pimpl;
  if (pimpl->plugin_get_glyph_pixels((dx_font_loader_plugin_glyph*)SELF->glyph_pimpl, pixels)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_font_glyph_get_texture_coordinates(dx_font_glyph* SELF, DX_RECT2_F32 *texture_coordinates) {
  dx_rect2_f32_set(texture_coordinates, 0.f, 0.f, 1.f, 1.f);
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.font",
                      dx_font,
                      Core_Object);

static void dx_font_destruct(dx_font* SELF) {
  Core_InlinePointerHashmap_remove(&SELF->font_manager->fonts, SELF->key);
  
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  pimpl->plugin_unreference_font((dx_font_loader_plugin_font*)SELF->font_pimpl);
  SELF->font_pimpl = NULL;

  CORE_UNREFERENCE(SELF->key);
  SELF->key = NULL;

  CORE_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;
}

static void dx_font_constructDispatch(dx_font_Dispatch* SELF)
{/*Intentionally empty.*/}

static void* allocate_callback(void* context, size_t number_of_bytes) {
  return malloc(number_of_bytes > 0 ? number_of_bytes : 1);
}

static void deallocate_callback(void* context, void* bytes) {
  if (bytes) {
    free(bytes);
  }
}

Core_Result dx_font_construct(dx_font* SELF, dx_font_key* key, dx_font_manager* font_manager) {
  DX_CONSTRUCT_PREFIX(dx_font);
  Core_Boolean containsSymbol;
  containsSymbol = dx_string_contains_symbol(key->font_file, '\0');
  if (containsSymbol) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_String* zero_terminator = NULL;
  if (Core_String_create(&zero_terminator, "", sizeof(""))) {
    return Core_Failure;
  }
  Core_String* format = NULL;
  if (Core_String_create(&format, "${s}${s}", sizeof("${s}${s}") - 1)) {
    CORE_UNREFERENCE(zero_terminator);
    zero_terminator = NULL;
    return Core_Failure;
  }
  Core_String* path = NULL;
  if (Core_String_printf(&path, format, key->font_file, zero_terminator)) {
    CORE_UNREFERENCE(zero_terminator);
    zero_terminator = NULL;
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(zero_terminator);
  zero_terminator = NULL;
  CORE_UNREFERENCE(format);
  format = NULL;
  void const* path_bytes = NULL;
  if (Core_String_getBytes(&path_bytes, path)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  _backend_impl* pimpl = (_backend_impl*)font_manager->pimpl;
  if (pimpl->plugin_create_font((dx_font_loader_plugin_font**)&SELF->font_pimpl, path_bytes, key->font_size, NULL, &allocate_callback, &deallocate_callback)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;

  SELF->key = key;
  CORE_REFERENCE(key);

  SELF->font_manager = font_manager;
  CORE_REFERENCE(font_manager);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_font_create(dx_font** RETURN, dx_font_key* key, dx_font_manager* font_manager) {
  DX_CREATE_PREFIX(dx_font)
  if (dx_font_construct(SELF, key, font_manager)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_font_get_baseline_distance(Core_Real32* RETURN, dx_font* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  if (pimpl->plugin_get_baseline_distance(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_font_get_ascender(Core_Real32* RETURN, dx_font* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  if (pimpl->plugin_get_ascender(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_font_get_descender(Core_Real32* RETURN, dx_font* SELF) {
  _backend_impl* pimpl = (_backend_impl*)SELF->font_manager->pimpl;
  if (pimpl->plugin_get_descender(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_font_manager* g_font_manager = NULL;

Core_defineObjectType("dx.font_manager",
                      dx_font_manager,
                      Core_Object);

static void dx_font_manager_destruct(dx_font_manager* SELF) {

  _backend_impl_uninitialize((_backend_impl*)SELF->pimpl);
  free(SELF->pimpl);
  SELF->pimpl = NULL;

  CORE_UNREFERENCE(SELF->context);
  SELF->context = NULL;

  g_font_manager = NULL;
}

static void dx_font_manager_constructDispatch(dx_font_manager_Dispatch* SELF)
{/*Intentionally empty.*/}

static Core_Result compare_font_key(Core_Boolean* RETURN, dx_font_key**a, dx_font_key** b) {
  return dx_font_key_is_equal_to(RETURN, *a, *b);
}

static Core_Result hash_font_key(Core_Size* RETURN, dx_font_key * *a) {
  return dx_font_key_get_hash_value(RETURN, *a);
}

static void font_key_added(dx_font_key** a) {
  CORE_REFERENCE(*a);
}

static void font_key_removed(dx_font_key** a) {
  CORE_UNREFERENCE(*a);
}

Core_Result dx_font_manager_construct(dx_font_manager* SELF, dx_val_context* context) {
  DX_CONSTRUCT_PREFIX(dx_font_manager);

  Core_InlinePointerHashMap_Configuration configuration = {
    .compareKeysCallback = (Core_InlinePointerHashmap_CompareKeysCallback*) & compare_font_key,
    .hashKeyCallback = (Core_InlinePointerHashmap_HashKeyCallback*) & hash_font_key,
    .keyAddedCallback = (Core_InlinePointerHashMap_KeyAddedCallback*) & font_key_added,
    .keyRemovedCallback = (Core_InlinePointerHashMap_KeyRemovedCallback*) & font_key_removed,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
  };
  if (Core_InlinePointerHashmap_initialize(&SELF->fonts, &configuration)) {
    return Core_Failure;
  }

  SELF->pimpl = malloc(sizeof(_backend_impl));
  if (!SELF->pimpl) {
    Core_InlinePointerHashmap_uninitialize(&SELF->fonts);
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  if (_backend_impl_initialize((_backend_impl*)SELF->pimpl)) {
    free(SELF->pimpl);
    SELF->pimpl = NULL;
    Core_InlinePointerHashmap_uninitialize(&SELF->fonts);
    return Core_Failure;
  }

  SELF->context = context;
  CORE_REFERENCE(context);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_font_manager_create(dx_font_manager** RETURN, dx_val_context* context) {
  if (g_font_manager) {
    CORE_REFERENCE(g_font_manager);
    *RETURN = g_font_manager;
    return Core_Success;
  } else {
    DX_CREATE_PREFIX(dx_font_manager)
    if (dx_font_manager_construct(SELF, context)) {
      CORE_UNREFERENCE(SELF);
      SELF = NULL;
      return Core_Failure;
    }
    g_font_manager = SELF;
    *RETURN = SELF;
    return Core_Success;
  }
}

Core_Result dx_font_manager_get_or_create_font(dx_font** RETURN, dx_font_manager* SELF, Core_String* font_file, Core_Natural8 font_size) {
  dx_font_key* font_key = NULL;
  if (dx_font_key_create(&font_key, font_file, font_size)) {
    return Core_Failure;
  }
  dx_font* font = NULL;
  if (Core_InlinePointerHashmap_get(&font, &SELF->fonts, font_key)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  }
  if (!font) {
    if (dx_font_create(&font, font_key, SELF)) {
      return Core_Failure;
    }
    if (Core_InlinePointerHashmap_set(&SELF->fonts, font_key, font)) {
      CORE_UNREFERENCE(font_key);
      font_key = NULL;
      CORE_UNREFERENCE(font);
      font = NULL;
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(font_key);
  font_key = NULL;
  CORE_REFERENCE(font);
  *RETURN = font;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
