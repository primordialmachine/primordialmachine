#if !defined(DX_FONT_MANAGER_H_INCLUDED)
#define DX_FONT_MANAGER_H_INCLUDED

#include "dx/core.h"
#include "dx/val/context.h"
typedef struct dx_font dx_font;
typedef struct dx_font_key dx_font_key;
typedef struct dx_font_manager dx_font_manager;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.font_key",
                       dx_font_key,
                       Core_Object);

static inline dx_font_key* DX_FONT_KEY(void* p) {
  return (dx_font_key*)p;
}

struct dx_font_key {
  Core_Object _parent;
  Core_String* font_file;
  Core_Natural8 font_size;
  Core_Size hash_value;
};

static inline dx_font_key_Dispatch* DX_FONT_KEY_DISPATCH(void* p) {
  return (dx_font_key_Dispatch*)p;
}

struct dx_font_key_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_font_key_construct(dx_font_key* SELF, Core_String* font_file, Core_Natural8 font_size);

Core_Result dx_font_key_create(dx_font_key** RETURN, Core_String* font_file, Core_Natural8 font_size);

Core_Result dx_font_key_is_equal_to(Core_Boolean* RETURN, dx_font_key* SELF, dx_font_key* other);

Core_Result dx_font_key_get_hash_value(Core_Size* RETURN, dx_font_key* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.font_glyph",
                       dx_font_glyph,
                       Core_Object);

static inline dx_font_glyph* DX_FONT_GLYPH(void* p) {
  return (dx_font_glyph*)p;
}

struct dx_font_glyph {
  Core_Object _parent;
  uint32_t code_point;
  void* glyph_pimpl;
  dx_font* font;
};

static inline dx_font_glyph_Dispatch* DX_FONT_GLYPH_DISPATCH(void* p) {
  return (dx_font_glyph_Dispatch*)p;
}

struct dx_font_glyph_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_font_glyph_construct(dx_font_glyph* SELF, uint32_t code_point, dx_font* font);

Core_Result dx_font_glyph_create(dx_font_glyph** RETURN, uint32_t code_point, dx_font* font);

Core_Result dx_font_glyph_get_glyph_advance(dx_font_glyph* SELF, Core_Real32* advance_x, Core_Real32* advance_y);

Core_Result dx_font_glyph_get_glyph_bearing(dx_font_glyph* SELF, Core_Real32* bearing_x, Core_Real32* bearing_y);

Core_Result dx_font_glyph_get_size(dx_font_glyph* SELF, uint32_t* size_x, uint32_t* size_y);

Core_Result dx_font_glyph_get_pixels(dx_font_glyph* SELF, void** pixels);

Core_Result dx_font_glyph_get_texture_coordinates(dx_font_glyph* SELF, DX_RECT2_F32 *texture_coordinates);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.font",
                       dx_font,
                       Core_Object);

static inline dx_font* DX_FONT(void* p) {
  return (dx_font*)p;
}

struct dx_font {
  Core_Object _parent;
  dx_font_key* key;
  dx_font_manager* font_manager;
  void* font_pimpl;
};

static inline dx_font_Dispatch* DX_FONT_DISPATCH(void* p) {
  return (dx_font_Dispatch*)p;
}

struct dx_font_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_font_construct(dx_font* SELF, dx_font_key* key, dx_font_manager* font_manager);

Core_Result dx_font_create(dx_font** RETURN, dx_font_key* key, dx_font_manager* font_manager);

Core_Result dx_font_get_baseline_distance(Core_Real32* RETURN, dx_font* SELF);

Core_Result dx_font_get_ascender(Core_Real32* RETURN, dx_font* SELF);

Core_Result dx_font_get_descender(Core_Real32* RETURN, dx_font* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// when a font object (for a font) is created, it is added to the cache of the font manager.
// when a font object is destroyed, it is removed from the cache of the font manager.
// if an entity requests a font and its font object is in the cache, then the cached font object is returned.
Core_declareObjectType("dx.font_manager",
                       dx_font_manager,
                       Core_Object);

static inline dx_font_manager* DX_FONT_MANAGER(void* p) {
  return (dx_font_manager*)p;
}

struct dx_font_manager {
  Core_Object _parent;
  /// @brief The hidden implementation.
  void* pimpl;
  /// @brief The VAL context.
  Core_Visuals_Context* context;
  /// @brief Map from names to font objects.
  Core_InlineHashMapPP fonts;
};

static inline dx_font_manager_Dispatch* DX_FONT_MANAGER_DISPATCH(void* p) {
  return (dx_font_manager_Dispatch*)p;
}

struct dx_font_manager_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_font_manager_construct(dx_font_manager* SELF, Core_Visuals_Context* context);

Core_Result dx_font_manager_create(dx_font_manager** RETURN, Core_Visuals_Context* context);

Core_Result dx_font_manager_get_or_create_font(dx_font** RETURN, dx_font_manager* SELF, Core_String* font_file, Core_Natural8 font_size);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_FONT_MANAGER_H_INCLUDED
