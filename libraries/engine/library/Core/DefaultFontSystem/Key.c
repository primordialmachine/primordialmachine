#include "Core/DefaultFontSystem/Key.h"

Core_defineObjectType("Core.DefaultFontSystem.Key",
                      Core_DefaultFontSystem_Key,
                      Core_Object);

static void Core_DefaultFontSystem_Key_destruct(Core_DefaultFontSystem_Key* SELF) {
  CORE_UNREFERENCE(SELF->fontFile);
  SELF->fontFile = NULL;
}

static void Core_DefaultFontSystem_Key_constructDispatch(Core_DefaultFontSystem_Key_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_DefaultFontSystem_Key_construct(Core_DefaultFontSystem_Key* SELF, Core_String* fontFile, Core_Natural8 fontSize) {
  DX_CONSTRUCT_PREFIX(Core_DefaultFontSystem_Key);

  SELF->fontFile = fontFile;
  CORE_REFERENCE(fontFile);

  SELF->fontSize = fontSize;

  if (Core_String_getHashValue(&SELF->hashValue, fontFile)) {
    CORE_UNREFERENCE(SELF->fontFile);
    SELF->fontFile = NULL;
    return Core_Failure;
  }

  SELF->hashValue = (SELF->hashValue * 37) + (Core_Size)SELF->fontSize;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Key_create(Core_DefaultFontSystem_Key** RETURN, Core_String* fontFile, Core_Natural8 fontSize) {
  DX_CREATE_PREFIX(Core_DefaultFontSystem_Key)
  if (Core_DefaultFontSystem_Key_construct(SELF, fontFile, fontSize)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Key_isEqualTo(Core_Boolean* RETURN, Core_DefaultFontSystem_Key* SELF, Core_DefaultFontSystem_Key* other) {
  if (SELF == other) {
    *RETURN = true;
    return Core_Success;
  }
  if (SELF->fontSize == other->fontSize) {
    Core_Boolean isEqualTo = Core_False;
    if (Core_String_isEqualTo(&isEqualTo, SELF->fontFile, other->fontFile)) {
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

Core_Result Core_DefaultFontSystem_Key_getFontFile(Core_String** RETURN, Core_DefaultFontSystem_Key* SELF) {
  CORE_REFERENCE(SELF->fontFile);
  *RETURN = SELF->fontFile;
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Key_getFontSize(Core_Size* RETURN, Core_DefaultFontSystem_Key* SELF) {
  *RETURN = SELF->fontSize;
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Key_getHashValue(Core_Size* RETURN, Core_DefaultFontSystem_Key* SELF) {
  *RETURN = SELF->hashValue;
  return Core_Success;
}