#if !defined(CORE_GLYPH_H_INCLUDED)
#define CORE_GLYPH_H_INCLUDED

#include "dx/core.h"
#include "Core/Assets/Image.h"

Core_declareObjectType("Core.Glyph",
                       Core_Glyph,
                       Core_Object);

static inline Core_Glyph* CORE_GLYPH(void* p) {
  return (Core_Glyph*)p;
}

struct Core_Glyph {
  Core_Object _parent;
  uint32_t codePoint;
};

static inline Core_Glyph_Dispatch* CORE_GLYPH_DISPATCH(void* p) {
  return (Core_Glyph_Dispatch*)p;
}

struct Core_Glyph_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*getGlyphAdvance)(Core_Glyph* SELF, Core_Real32* advanceX, Core_Real32* advanceY);
  Core_Result(*getGlyphBearing)(Core_Glyph* SELF, Core_Real32* bearingX, Core_Real32* bearingY);
  Core_Result(*getGlyphSize)(Core_Glyph* SELF, uint32_t* sizeX, uint32_t* sizeY);
  Core_Result(*getImage)(Core_Glyph* SELF, Core_Assets_Image** image);
  Core_Result(*getTextureCoordinates)(Core_Glyph* SELF, DX_RECT2_F32* textureCoordinates);
};

Core_Result Core_Glyph_construct(Core_Glyph* SELF, uint32_t codePoint);

static inline Core_Result Core_Glyph_getGlyphAdvance(Core_Glyph* SELF, Core_Real32* advanceX, Core_Real32* advanceY) {
  DX_OBJECT_VIRTUALCALL(Core_Glyph, getGlyphAdvance, SELF, advanceX, advanceY);
}

static inline Core_Result Core_Glyph_getGlyphBearing(Core_Glyph* SELF, Core_Real32* bearingX, Core_Real32* bearingY) {
  DX_OBJECT_VIRTUALCALL(Core_Glyph, getGlyphBearing, SELF, bearingX, bearingY);
}

static inline Core_Result Core_Glyph_getSize(Core_Glyph* SELF, uint32_t* sizeX, uint32_t* sizeY) {
  DX_OBJECT_VIRTUALCALL(Core_Glyph, getGlyphSize, SELF, sizeX, sizeY);
}

static inline Core_Result Core_Glyph_getImage(Core_Glyph* SELF, Core_Assets_Image** image) {
  DX_OBJECT_VIRTUALCALL(Core_Glyph, getImage, SELF, image);
}

static inline Core_Result Core_Glyph_getTextureCoordinates(Core_Glyph* SELF, DX_RECT2_F32* textureCoordinates) {
  DX_OBJECT_VIRTUALCALL(Core_Glyph, getTextureCoordinates, SELF, textureCoordinates);
}

#endif // CORE_GLYPH_H_INCLUDED
