#if !defined(CORE_DEFAULTFONTSYSTEM_GLYPH_H_INCLUDED)
#define CORE_DEFAULTFONTSYSTEM_GLYPH_H_INCLUDED

#include "Core/Glyph.h"
typedef struct Core_DefaultFontSystem_Font Core_DefaultFontSystem_Font;

/*@internal*/
Core_declareObjectType("Core.DefaultFontSystem.Glyph",
                       Core_DefaultFontSystem_Glyph,
                       Core_Glyph);

/*@internal*/
static inline Core_DefaultFontSystem_Glyph* CORE_DEFAULTFONTSYSTEM_GLYPH(void* p) {
  return (Core_DefaultFontSystem_Glyph*)p;
}

/*@internal*/
struct Core_DefaultFontSystem_Glyph {
  Core_Glyph _parent;
  void* glyphPimpl;
  Core_DefaultFontSystem_Font* font;
};

/*@internal*/
static inline Core_DefaultFontSystem_Glyph_Dispatch* CORE_DEFAULTFONTSYSTEM_GLYPH_DISPATCH(void* p) {
  return (Core_DefaultFontSystem_Glyph_Dispatch*)p;
}

/*@internal*/
struct Core_DefaultFontSystem_Glyph_Dispatch {
  Core_Glyph_Dispatch _parent;
};

/*@internal*/
Core_Result Core_DefaultFontSystem_Glyph_construct(Core_DefaultFontSystem_Glyph* SELF, uint32_t codePoint, Core_DefaultFontSystem_Font* font);

/*@internal*/
Core_Result Core_DefaultFontSystem_Glyph_create(Core_DefaultFontSystem_Glyph** RETURN, uint32_t codePoint, Core_DefaultFontSystem_Font* font);

#endif // CORE_DEFAULTFONTSYSTEM_GLYPH_H_INCLUDED
