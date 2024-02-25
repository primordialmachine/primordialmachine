#if !defined(CORE_DEFAULTFONTSYSTEM_KEY_H_INCLUDED)
#define CORE_DEFAULTFONTSYSTEM_KEY_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("Core.DefaultFontSystem.Key",
                       Core_DefaultFontSystem_Key,
                       Core_Object);

static inline Core_DefaultFontSystem_Key* CORE_DEFAULTFONTSYSTEM_KEY(void* p) {
  return (Core_DefaultFontSystem_Key*)p;
}

struct Core_DefaultFontSystem_Key {
  Core_Object _parent;
  Core_String* fontFile;
  Core_Natural8 fontSize;
  Core_Size hashValue;
};

static inline Core_DefaultFontSystem_Key_Dispatch* CORE_DEFAULTFONTSYSTEM_KEY_DISPATCH(void* p) {
  return (Core_DefaultFontSystem_Key_Dispatch*)p;
}

struct Core_DefaultFontSystem_Key_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_DefaultFontSystem_Key_construct(Core_DefaultFontSystem_Key* SELF, Core_String* font_file, Core_Natural8 font_size);

Core_Result Core_DefaultFontSystem_Key_create(Core_DefaultFontSystem_Key** RETURN, Core_String* font_file, Core_Natural8 font_size);

Core_Result Core_DefaultFontSystem_Key_isEqualTo(Core_Boolean* RETURN, Core_DefaultFontSystem_Key* SELF, Core_DefaultFontSystem_Key* other);

Core_Result Core_DefaultFontSystem_Key_getFontFile(Core_String** RETURN, Core_DefaultFontSystem_Key* SELF);

Core_Result Core_DefaultFontSystem_Key_getFontSize(Core_Size* RETURN, Core_DefaultFontSystem_Key* SELF);

Core_Result Core_DefaultFontSystem_Key_getHashValue(Core_Size* RETURN, Core_DefaultFontSystem_Key* SELF);

#endif // CORE_DEFAULTFONTSYSTEM_KEY_H_INCLUDED
