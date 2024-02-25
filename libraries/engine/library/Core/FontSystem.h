#if !defined(CORE_FONTSYSTEM_H_INCLUDED)
#define CORE_FONTSYSTEM_H_INCLUDED

#include "Core/System.h"
#include "Core/String.h"
typedef struct Core_Font Core_Font;

// <b>Font Creation and Destruction</b>
// When a font object is created, it is added to the font system object with a reference count of 1.
// The font object holds a reference to the font system.
// We obviously have a reference counting cycle.
// All font objects of a reference count of 1 are destroyed when shutting down the font manager object.
// This breaks the reference counting cycle.
// Consequently: Fonts of a reference count of 1 are currently not removed until the font manager shuts down.
// 
// <b>Font Retrieval</b>
// If a caller requests a font object, then the font is created if it does not exist (see Font Creation and Destruction).
// The font object is returned and the caller acquires a reference to the font object.
Core_declareObjectType("Core.FontSystem",
                       Core_FontSystem,
                       Core_System);

static inline Core_FontSystem* CORE_FONTSYSTEM(void* p) {
  return (Core_FontSystem*)p;
}

struct Core_FontSystem {
  Core_System _parent;
};

static inline Core_FontSystem_Dispatch* CORE_FONTSYSTEM_DISPATCH(void* p) {
  return (Core_FontSystem_Dispatch*)p;
}

struct Core_FontSystem_Dispatch {
  Core_System_Dispatch _parent;
  Core_Result(*getOrCreateFont)(Core_Font** RETURN, Core_FontSystem* SELF, Core_String* fontFile, Core_Natural8 fontSize);
};

Core_Result Core_FontSystem_construct(Core_FontSystem* SELF);

/// @brief Get or create a font.
/// @param RETURN A Pointer to a <code>Core_Font</code> variable.
/// @param fontFile The pathname of the font file.
/// @param fontSize The size, in pixels, of the font.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the <code>Core_Font</code> object.
/// The caller acquired a reference to that object.
/// @method{Core_FontSystem}
static Core_Result Core_FontSystem_getOrCreateFont(Core_Font** RETURN, Core_FontSystem* SELF, Core_String* fontFile, Core_Natural8 fontSize) {
  DX_OBJECT_VIRTUALCALL(Core_FontSystem, getOrCreateFont, RETURN, SELF, fontFile, fontSize);
}

#endif // CORE_FONTSYSTEM_H_INCLUDED
