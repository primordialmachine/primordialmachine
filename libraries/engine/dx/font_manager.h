#if !defined(DX_FONT_MANAGER_H_INCLUDED)
#define DX_FONT_MANAGER_H_INCLUDED

#include "dx/core.h"
#include "dx/val/context.h"
typedef struct dx_font dx_font;
typedef struct dx_font_key dx_font_key;
typedef struct dx_font_manager dx_font_manager;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.font_key",
                       dx_font_key,
                       dx_object);

static inline dx_font_key* DX_FONT_KEY(void* p) {
  return (dx_font_key*)p;
}

struct dx_font_key {
  dx_object _parent;
  dx_string* font_file;
  dx_n8 font_size;
  dx_size hash_value;
};

static inline dx_font_key_dispatch* DX_FONT_KEY_DISPATCH(void* p) {
  return (dx_font_key_dispatch*)p;
}

struct dx_font_key_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_font_key_construct(dx_font_key* SELF, dx_string* font_file, dx_n8 font_size);

dx_result dx_font_key_create(dx_font_key** RETURN, dx_string* font_file, dx_n8 font_size);

dx_result dx_font_key_is_equal_to(dx_bool* RETURN, dx_font_key* SELF, dx_font_key* other);

dx_result dx_font_key_get_hash_value(dx_size* RETURN, dx_font_key* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.font_glyph",
                       dx_font_glyph,
                       dx_object);

static inline dx_font_glyph* DX_FONT_GLYPH(void* p) {
  return (dx_font_glyph*)p;
}

struct dx_font_glyph {
  dx_object _parent;
  uint32_t code_point;
  void* glyph_pimpl;
  dx_font* font;
};

static inline dx_font_glyph_dispatch* DX_FONT_GLYPH_DISPATCH(void* p) {
  return (dx_font_glyph_dispatch*)p;
}

struct dx_font_glyph_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_font_glyph_construct(dx_font_glyph* SELF, uint32_t code_point, dx_font* font);

dx_result dx_font_glyph_create(dx_font_glyph** RETURN, uint32_t code_point, dx_font* font);

dx_result dx_font_glyph_get_glyph_advance(dx_font_glyph* SELF, dx_f32* advance_x, dx_f32* advance_y);

dx_result dx_font_glyph_get_glyph_bearing(dx_font_glyph* SELF, dx_f32* bearing_x, dx_f32* bearing_y);

dx_result dx_font_glyph_get_size(dx_font_glyph* SELF, uint32_t* size_x, uint32_t* size_y);

dx_result dx_font_glyph_get_pixels(dx_font_glyph* SELF, void** pixels);

dx_result dx_font_glyph_get_texture_coordinates(dx_font_glyph* SELF, dx_f32* left, dx_f32* bottom, dx_f32* right, dx_f32* top);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.font",
                       dx_font,
                       dx_object);

static inline dx_font* DX_FONT(void* p) {
  return (dx_font*)p;
}

struct dx_font {
  dx_object _parent;
  dx_font_key* key;
  dx_font_manager* font_manager;
  void* font_pimpl;
};

static inline dx_font_dispatch* DX_FONT_DISPATCH(void* p) {
  return (dx_font_dispatch*)p;
}

struct dx_font_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_font_construct(dx_font* SELF, dx_font_key* key, dx_font_manager* font_manager);

dx_result dx_font_create(dx_font** RETURN, dx_font_key* key, dx_font_manager* font_manager);

dx_result dx_font_get_baseline_distance(dx_f32* RETURN, dx_font* SELF);

dx_result dx_font_get_ascender(dx_f32* RETURN, dx_font* SELF);

dx_result dx_font_get_descender(dx_f32* RETURN, dx_font* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// when a font object (for a font) is created, it is added to the cache of the font manager.
// when a font object is destroyed, it is removed from the cache of the font manager.
// if an entity requests a font and its font object is in the cache, then the cached font object is returned.
DX_DECLARE_OBJECT_TYPE("dx.font_manager",
                       dx_font_manager,
                       dx_object);

static inline dx_font_manager* DX_FONT_MANAGER(void* p) {
  return (dx_font_manager*)p;
}

struct dx_font_manager {
  dx_object _parent;
  /// @brief The hidden implementation.
  void* pimpl;
  /// @brief The VAL context.
  dx_val_context* context;
  /// @brief Map from names to font objects.
  dx_inline_pointer_hashmap fonts;
};

static inline dx_font_manager_dispatch* DX_FONT_MANAGER_DISPATCH(void* p) {
  return (dx_font_manager_dispatch*)p;
}

struct dx_font_manager_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_font_manager_construct(dx_font_manager* SELF, dx_val_context* context);

dx_result dx_font_manager_create(dx_font_manager** RETURN, dx_val_context* context);

dx_result dx_font_manager_get_or_create_font(dx_font** RETURN, dx_font_manager* SELF, dx_string* font_file, dx_n8 font_size);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_FONT_MANAGER_H_INCLUDED
