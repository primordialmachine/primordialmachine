#include "Core/DefaultFontSystem/Font.h"

#include "Core/DefaultFontSystem.h"
#include "Core/DefaultFontSystem/FontLoaderPlugin.h"
#include <malloc.h>

Core_defineObjectType("Core.DefaultFontSystem.Font",
                      Core_DefaultFontSystem_Font,
                      Core_Font);

static Core_Result Core_DefaultFontSystem_Font_getBaselineDistance(Core_Real32* RETURN, Core_DefaultFontSystem_Font* SELF);

static Core_Result Core_DefaultFontSystem_Font_getAscender(Core_Real32* RETURN, Core_DefaultFontSystem_Font* SELF);

static Core_Result Core_DefaultFontSystem_Font_getDescender(Core_Real32* RETURN, Core_DefaultFontSystem_Font* SELF);

static void Core_DefaultFontSystem_Font_destruct(Core_DefaultFontSystem_Font* SELF) {
  Core_InlineHashMapPP_remove(&SELF->fontSystem->fonts, SELF->key);

  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->fontSystem->pimpl;
  pimpl->plugin_unreference_font((dx_font_loader_plugin_font*)SELF->font_pimpl);
  SELF->font_pimpl = NULL;

  CORE_UNREFERENCE(SELF->key);
  SELF->key = NULL;

  CORE_UNREFERENCE(SELF->fontSystem);
  SELF->fontSystem = NULL;
}

static void Core_DefaultFontSystem_Font_constructDispatch(Core_DefaultFontSystem_Font_Dispatch* SELF) {
  CORE_FONT_DISPATCH(SELF)->getBaselineDistance = (Core_Result(*)(Core_Real32*, Core_Font*)) & Core_DefaultFontSystem_Font_getBaselineDistance;
  CORE_FONT_DISPATCH(SELF)->getAscender = (Core_Result(*)(Core_Real32*, Core_Font*)) & Core_DefaultFontSystem_Font_getAscender;
  CORE_FONT_DISPATCH(SELF)->getDescender = (Core_Result(*)(Core_Real32*, Core_Font*)) & Core_DefaultFontSystem_Font_getDescender;
}

static void* allocate_callback(void* context, size_t number_of_bytes) {
  return malloc(number_of_bytes > 0 ? number_of_bytes : 1);
}

static void deallocate_callback(void* context, void* bytes) {
  if (bytes) {
    free(bytes);
  }
}

Core_Result Core_DefaultFontSystem_Font_construct(Core_DefaultFontSystem_Font* SELF, Core_DefaultFontSystem_Key* key, Core_DefaultFontSystem* fontSystem) {
  Core_BeginConstructor(Core_DefaultFontSystem_Font);

  Core_Boolean containsSymbol;
  if (Core_String_containsSymbol(&containsSymbol, key->fontFile, '\0')) {
    return Core_Failure;
  }
  if (containsSymbol) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_String* zero_terminator = NULL;
  if (Core_String_create(&zero_terminator, "", sizeof(""))) {
    return Core_Failure;
  }
  Core_String* format = NULL;
  if (Core_String_create(&format, "${s}${s}", sizeof("${s}${s}") - 1)) {
    CORE_UNREFERENCE(zero_terminator);
    zero_terminator = NULL;
    return Core_Failure;
  }
  Core_String* path = NULL;
  if (Core_String_printf(&path, format, key->fontFile, zero_terminator)) {
    CORE_UNREFERENCE(zero_terminator);
    zero_terminator = NULL;
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(zero_terminator);
  zero_terminator = NULL;
  CORE_UNREFERENCE(format);
  format = NULL;
  void const* path_bytes = NULL;
  if (Core_String_getBytes(&path_bytes, path)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)fontSystem->pimpl;
  if (pimpl->plugin_create_font((dx_font_loader_plugin_font**)&SELF->font_pimpl, path_bytes, key->fontSize, NULL, &allocate_callback, &deallocate_callback)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path);
  path = NULL;

  SELF->key = key;
  CORE_REFERENCE(key);

  SELF->fontSystem = fontSystem;
  CORE_REFERENCE(fontSystem);

  Core_EndConstructor(Core_DefaultFontSystem_Font);
}

Core_Result Core_DefaultFontSystem_Font_create(Core_DefaultFontSystem_Font** RETURN, Core_DefaultFontSystem_Key* key, Core_DefaultFontSystem* fontSystem) {
  DX_CREATE_PREFIX(Core_DefaultFontSystem_Font);
  if (Core_DefaultFontSystem_Font_construct(SELF, key, fontSystem)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Font_getBaselineDistance(Core_Real32* RETURN, Core_DefaultFontSystem_Font* SELF) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->fontSystem->pimpl;
  if (pimpl->plugin_get_baseline_distance(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Font_getAscender(Core_Real32* RETURN, Core_DefaultFontSystem_Font* SELF) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->fontSystem->pimpl;
  if (pimpl->plugin_get_ascender(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_DefaultFontSystem_Font_getDescender(Core_Real32* RETURN, Core_DefaultFontSystem_Font* SELF) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->fontSystem->pimpl;
  if (pimpl->plugin_get_descender(RETURN, (dx_font_loader_plugin_font*)SELF->font_pimpl)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}
