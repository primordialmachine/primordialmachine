#if !defined(CORE_FONT_H_INCLUDED)
#define CORE_FONT_H_INCLUDED

#include "dx/core/Object.h"

Core_declareObjectType("Core.Font",
                       Core_Font,
                       Core_Object);

static inline Core_Font* CORE_FONT(void* p) {
  return (Core_Font*)p;
}

struct Core_Font {
  Core_Object _parent;
};

static inline Core_Font_Dispatch* CORE_FONT_DISPATCH(void* p) {
  return (Core_Font_Dispatch*)p;
}

struct Core_Font_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*getBaselineDistance)(Core_Real32* RETURN, Core_Font* SELF);
  Core_Result(*getAscender)(Core_Real32* RETURN, Core_Font* SELF);
  Core_Result(*getDescender)(Core_Real32* RETURN, Core_Font* SELF);
};

Core_Result Core_Font_construct(Core_Font* SELF);

/// @brief Get the baseline distance of this font.
/// @param RETURN A pointer to a <code>Core_Real32</code> variable.
/// @success <code>*RETURN</code> was assigned the baseline distance of this font.
/// @method{Core_Font}
static inline Core_Result Core_Font_getBaselineDistance(Core_Real32* RETURN, Core_Font* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Font, getBaselineDistance, RETURN, SELF);
}

/// @brief Get the ascender of this font.
/// @param RETURN A pointer to a <code>Core_Real32</code> variable.
/// @success <code>*RETURN</code> was assigned the ascender of this font.
/// @method{Core_Font}
static inline Core_Result Core_Font_getAscender(Core_Real32* RETURN, Core_Font* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Font, getAscender, RETURN, SELF);
}

/// @brief Get the descender of this font.
/// @param RETURN A pointer to a <code>Core_Real32</code> variable.
/// @success <code>*RETURN</code> was assigned the descender of this font.
/// @method{Core_Font}
static inline Core_Result Core_Font_getDescender(Core_Real32* RETURN, Core_Font* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Font, getDescender, RETURN, SELF);
}

#endif // CORE_FONT_H_INCLUDED
