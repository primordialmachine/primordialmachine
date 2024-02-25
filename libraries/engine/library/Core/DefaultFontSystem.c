#include "Core/DefaultFontSystem.h"

#include "Core/DefaultFontSystem/FontLoaderPlugin.h"
#include "Core/DefaultFontSystem/Font.h"
#include "Core/DefaultFontSystem/Key.h"

static Core_DefaultFontSystem* g_font_manager = NULL;

Core_defineObjectType("Core.DefaultFontSystem",
                      Core_DefaultFontSystem,
                      Core_FontSystem);

static Core_Result Core_DefaultFontSystem_compareKeys(Core_Boolean* RETURN, Core_DefaultFontSystem_Key** a, Core_DefaultFontSystem_Key** b);

static Core_Result Core_DefaultFontSystem_hashKey(Core_Size* RETURN, Core_DefaultFontSystem_Key** a);

static void Core_DefaultFontSystem_keyAdded(Core_DefaultFontSystem_Key** a);

static void Core_DefaultFontSystem_keyRemoved(Core_DefaultFontSystem_Key** a);

static void Core_DefaultFontSystem_valueAdded(Core_DefaultFontSystem_Font** a);

static void Core_DefaultFontSystem_valueRemoved(Core_DefaultFontSystem_Font** a);

static Core_Result Core_DefaultFontSystem_startup(Core_DefaultFontSystem* SELF);

static Core_Result Core_DefaultFontSystem_shutdown(Core_DefaultFontSystem* SELF);

static Core_Result Core_DefaultFontSystem_startup(Core_DefaultFontSystem* SELF);

static Core_Result Core_DefaultFontSystem_shutdown(Core_DefaultFontSystem* SELF);

static Core_Result Core_DefaultFontSystem_getOrCreateFont(Core_DefaultFontSystem_Font** RETURN, Core_DefaultFontSystem* SELF, Core_String* fontFile, Core_Natural8 fontSize);

static void Core_DefaultFontSystem_destruct(Core_DefaultFontSystem* SELF) {
  FontLoaderPlugin_uninitialize((FontLoaderPlugin*)SELF->pimpl);

  Core_Memory_deallocate(SELF->pimpl);
  SELF->pimpl = NULL;

  CORE_UNREFERENCE(SELF->context);
  SELF->context = NULL;

  g_font_manager = NULL;
}

static void Core_DefaultFontSystem_constructDispatch(Core_DefaultFontSystem_Dispatch* SELF) {
  CORE_SYSTEM_DISPATCH(SELF)->startup = (Core_Result(*)(Core_System*)) & Core_DefaultFontSystem_startup;
  CORE_SYSTEM_DISPATCH(SELF)->shutdown = (Core_Result(*)(Core_System*)) & Core_DefaultFontSystem_shutdown;
  CORE_FONTSYSTEM_DISPATCH(SELF)->getOrCreateFont = (Core_Result(*)(Core_Font**, Core_FontSystem*, Core_String*, Core_Natural8)) & Core_DefaultFontSystem_getOrCreateFont;
}

static Core_Result Core_DefaultFontSystem_compareKeys(Core_Boolean* RETURN, Core_DefaultFontSystem_Key** a, Core_DefaultFontSystem_Key** b) {
  return Core_DefaultFontSystem_Key_isEqualTo(RETURN, *a, *b);
}

static Core_Result Core_DefaultFontSystem_hashKey(Core_Size* RETURN, Core_DefaultFontSystem_Key** a) {
  return Core_DefaultFontSystem_Key_getHashValue(RETURN, *a);
}

static void Core_DefaultFontSystem_keyAdded(Core_DefaultFontSystem_Key** a) {
  CORE_REFERENCE(*a);
}

static void Core_DefaultFontSystem_keyRemoved(Core_DefaultFontSystem_Key** a) {
  CORE_UNREFERENCE(*a);
}

static void Core_DefaultFontSystem_valueAdded(Core_DefaultFontSystem_Font** a) {
  CORE_REFERENCE(*a);
}

static void Core_DefaultFontSystem_valueRemoved(Core_DefaultFontSystem_Font** a) {
  CORE_UNREFERENCE(*a);
}

static Core_Result Core_DefaultFontSystem_startup(Core_DefaultFontSystem* SELF) {
  return Core_Success;
}

static Core_Result Core_DefaultFontSystem_shutdown(Core_DefaultFontSystem* SELF) {
  if (Core_InlineHashMapPP_clear(&SELF->fonts)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_construct(Core_DefaultFontSystem* SELF, Core_Visuals_Context* context) {
  Core_BeginConstructor(Core_DefaultFontSystem);
  if (Core_FontSystem_construct(CORE_FONTSYSTEM(SELF))) {
    return Core_Failure;
  }

  Core_InlineHashMapPP_Configuration configuration = {
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&Core_DefaultFontSystem_compareKeys,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&Core_DefaultFontSystem_hashKey,
    .keyAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*)&Core_DefaultFontSystem_keyAdded,
    .keyRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*)&Core_DefaultFontSystem_keyRemoved,
    .valueAddedCallback = (Core_InlineHashMapPP_ValueAddedCallback*)&Core_DefaultFontSystem_valueAdded,
    .valueRemovedCallback = (Core_InlineHashMapPP_ValueRemovedCallback*)&Core_DefaultFontSystem_valueRemoved,
  };
  if (Core_InlineHashMapPP_initialize(&SELF->fonts, &configuration)) {
    return Core_Failure;
  }

  if (Core_Memory_allocate(&SELF->pimpl, sizeof(FontLoaderPlugin))) {
    Core_InlineHashMapPP_uninitialize(&SELF->fonts);
    return Core_Failure;
  }

  if (FontLoaderPlugin_initialize((FontLoaderPlugin*)SELF->pimpl)) {
    Core_Memory_deallocate(SELF->pimpl);
    SELF->pimpl = NULL;
    Core_InlineHashMapPP_uninitialize(&SELF->fonts);
    return Core_Failure;
  }

  SELF->context = context;
  CORE_REFERENCE(context);

  Core_EndConstructor(Core_DefaultFontSystem);
}

Core_Result Core_DefaultFontSystem_create(Core_DefaultFontSystem** RETURN, Core_Visuals_Context* context) {
  if (g_font_manager) {
    CORE_REFERENCE(g_font_manager);
    *RETURN = g_font_manager;
    return Core_Success;
  } else {
    DX_CREATE_PREFIX(Core_DefaultFontSystem)
      if (Core_DefaultFontSystem_construct(SELF, context)) {
        CORE_UNREFERENCE(SELF);
        SELF = NULL;
        return Core_Failure;
      }
    g_font_manager = SELF;
    *RETURN = SELF;
    return Core_Success;
  }
}

static Core_Result Core_DefaultFontSystem_getOrCreateFont(Core_DefaultFontSystem_Font** RETURN, Core_DefaultFontSystem* SELF, Core_String* fontFile, Core_Natural8 fontSize) {
  Core_DefaultFontSystem_Key* fontKey = NULL;
  if (Core_DefaultFontSystem_Key_create(&fontKey, fontFile, fontSize)) {
    return Core_Failure;
  }
  Core_DefaultFontSystem_Font* font = NULL;
  if (Core_InlineHashMapPP_get(&font, &SELF->fonts, fontKey)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  }
  if (!font) {
    if (Core_DefaultFontSystem_Font_create(&font, fontKey, SELF)) {
      return Core_Failure;
    }
    if (Core_InlineHashMapPP_set(&SELF->fonts, fontKey, font)) {
      CORE_UNREFERENCE(fontKey);
      fontKey = NULL;
      CORE_UNREFERENCE(font);
      font = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(fontKey);
    fontKey = NULL;
    // reference count is 2
    *RETURN = font;
  } else {
    CORE_UNREFERENCE(fontKey);
    fontKey = NULL;
    CORE_REFERENCE(font);
    // reference count is old + 1
    *RETURN = font;
  }
  return Core_Success;
}
