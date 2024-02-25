#if !defined(CORE_DEFAULTFONTSYSTEM_FONT_H_INCLUDED)
#define CORE_DEFAULTFONTSYSTEM_FONT_H_INCLUDED

#include "Core/DefaultFontSystem/Key.h"
#include "Core/Font.h"
typedef struct Core_DefaultFontSystem Core_DefaultFontSystem;

/*@internal*/
Core_declareObjectType("Core.DefaultFontSystem.Font",
                       Core_DefaultFontSystem_Font,
                       Core_Font);

/*@internal*/
static inline Core_DefaultFontSystem_Font* CORE_DEFAULTFONTSYSTEM_FONT(void* p) {
  return (Core_DefaultFontSystem_Font*)p;
}

/*@internal*/
struct Core_DefaultFontSystem_Font {
  Core_Font _parent;
  Core_DefaultFontSystem_Key* key;
  Core_DefaultFontSystem* fontSystem;
  void* font_pimpl;
};

/*@internal*/
static inline Core_DefaultFontSystem_Font_Dispatch* CORE_DEFAULTFONTSYSTEM_FONT_DISPATCH(void* p) {
  return (Core_DefaultFontSystem_Font_Dispatch*)p;
}

/*@internal*/
struct Core_DefaultFontSystem_Font_Dispatch {
  Core_Font_Dispatch _parent;
};

/*@internal*/
Core_Result Core_DefaultFontSystem_Font_construct(Core_DefaultFontSystem_Font* SELF, Core_DefaultFontSystem_Key* key, Core_DefaultFontSystem* font_manager);

/*@internal*/
Core_Result Core_DefaultFontSystem_Font_create(Core_DefaultFontSystem_Font** RETURN, Core_DefaultFontSystem_Key* key, Core_DefaultFontSystem* font_manager);

#endif // CORE_DEFAULTFONTSYSTEM_FONT_H_INCLUDED
