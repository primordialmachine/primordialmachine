#include "Core/Glyph.h"

Core_defineObjectType("Core.Glyph",
                      Core_Glyph,
                      Core_Object);

static void Core_Glyph_destruct(Core_Glyph* SELF)
{}

static void Core_Glyph_constructDispatch(Core_Glyph_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Glyph_construct(Core_Glyph* SELF, uint32_t codePoint) {
  Core_BeginConstructor(Core_Glyph);
  SELF->codePoint = codePoint;
  Core_EndConstructor(Core_Glyph);
}
