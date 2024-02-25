#include "Core/DefaultFontSystem/Glyph.h"

#include "Core/DefaultFontSystem/Font.h"
#include "Core/DefaultFontSystem.h"
#include "Core/DefaultFontSystem/FontLoaderPlugin.h"

Core_defineObjectType("Core.DefaultFontSystem.Glyph",
                      Core_DefaultFontSystem_Glyph,
                      Core_Glyph);

static Core_Result Core_DefaultFontSystem_Glyph_getGlyphAdvance(Core_DefaultFontSystem_Glyph* SELF, Core_Real32* advanceX, Core_Real32* advanceY);

static Core_Result Core_DefaultFontSystem_Glyph_getGlyphBearing(Core_DefaultFontSystem_Glyph* SELF, Core_Real32* bearingX, Core_Real32* bearingY);

static Core_Result Core_DefaultFontSystem_Glyph_getSize(Core_DefaultFontSystem_Glyph* SELF, uint32_t* sizeX, uint32_t* sizeY);

static Core_Result Core_DefaultFontSystem_Glyph_getImage(Core_DefaultFontSystem_Glyph* SELF, Core_Assets_Image** image);

static Core_Result Core_DefaultFontSystem_Glyph_getTextureCoordinates(Core_DefaultFontSystem_Glyph* SELF, DX_RECT2_F32* textureCoordinates);

static void Core_DefaultFontSystem_Glyph_destruct(Core_DefaultFontSystem_Glyph* SELF) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->font->fontSystem->pimpl;
  pimpl->plugin_unreference_glyph((dx_font_loader_plugin_glyph*)SELF->glyphPimpl);
  SELF->glyphPimpl = NULL;
  CORE_UNREFERENCE(SELF->font);
  SELF->font = NULL;
}

static void Core_DefaultFontSystem_Glyph_constructDispatch(Core_DefaultFontSystem_Glyph_Dispatch* SELF) {
  CORE_GLYPH_DISPATCH(SELF)->getGlyphAdvance = (Core_Result(*)(Core_Glyph*, Core_Real32*, Core_Real32*)) & Core_DefaultFontSystem_Glyph_getGlyphAdvance;
  CORE_GLYPH_DISPATCH(SELF)->getGlyphBearing = (Core_Result(*)(Core_Glyph*, Core_Real32*, Core_Real32*)) & Core_DefaultFontSystem_Glyph_getGlyphBearing;
  CORE_GLYPH_DISPATCH(SELF)->getGlyphSize = (Core_Result(*)(Core_Glyph*, uint32_t*, uint32_t*)) & Core_DefaultFontSystem_Glyph_getSize;
  CORE_GLYPH_DISPATCH(SELF)->getImage = (Core_Result(*)(Core_Glyph*, Core_Assets_Image**)) & Core_DefaultFontSystem_Glyph_getImage;
  CORE_GLYPH_DISPATCH(SELF)->getTextureCoordinates = (Core_Result(*)(Core_Glyph*, DX_RECT2_F32*)) & Core_DefaultFontSystem_Glyph_getTextureCoordinates;
}

Core_Result Core_DefaultFontSystem_Glyph_construct(Core_DefaultFontSystem_Glyph* SELF, uint32_t codePoint, Core_DefaultFontSystem_Font* font) {
  Core_BeginConstructor(Core_DefaultFontSystem_Glyph);

  if (Core_Glyph_construct(CORE_GLYPH(SELF), codePoint)) {
    return Core_Failure;
  }

  SELF->font = font;
  CORE_REFERENCE(font);

  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->font->fontSystem->pimpl;
  if (pimpl->plugin_get_glyph(&SELF->glyphPimpl, (dx_font_loader_plugin_font*)SELF->font->font_pimpl, CORE_GLYPH(SELF)->codePoint)) {
    CORE_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    return Core_Failure;
  }

  Core_EndConstructor(Core_DefaultFontSystem_Glyph);
}

Core_Result Core_DefaultFontSystem_Glyph_create(Core_DefaultFontSystem_Glyph** RETURN, uint32_t codePoint, Core_DefaultFontSystem_Font* font) {
  DX_CREATE_PREFIX(Core_DefaultFontSystem_Glyph)
    if (Core_DefaultFontSystem_Glyph_construct(SELF, codePoint, font)) {
      CORE_UNREFERENCE(SELF);
      SELF = NULL;
      return Core_Failure;
    }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result Core_DefaultFontSystem_Glyph_getGlyphAdvance(Core_DefaultFontSystem_Glyph* SELF, Core_Real32* advance_x, Core_Real32* advance_y) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->font->fontSystem->pimpl;
  if (pimpl->plugin_get_glyph_advance((dx_font_loader_plugin_glyph*)SELF->glyphPimpl, advance_x, advance_y)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result Core_DefaultFontSystem_Glyph_getGlyphBearing(Core_DefaultFontSystem_Glyph* SELF, Core_Real32* bearing_x, Core_Real32* bearing_y) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->font->fontSystem->pimpl;
  if (pimpl->plugin_get_glyph_bearing((dx_font_loader_plugin_glyph*)SELF->glyphPimpl, bearing_x, bearing_y)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result Core_DefaultFontSystem_Glyph_getSize(Core_DefaultFontSystem_Glyph* SELF, uint32_t* size_x, uint32_t* size_y) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->font->fontSystem->pimpl;
  if (pimpl->plugin_get_glyph_size((dx_font_loader_plugin_glyph*)SELF->glyphPimpl, size_x, size_y)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result Core_DefaultFontSystem_Glyph_getImage(Core_DefaultFontSystem_Glyph* SELF, Core_Assets_Image** image) {
  FontLoaderPlugin* pimpl = (FontLoaderPlugin*)SELF->font->fontSystem->pimpl;
  void* pixels = NULL;
  if (pimpl->plugin_get_glyph_pixels((dx_font_loader_plugin_glyph*)SELF->glyphPimpl, &pixels)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  uint32_t width = 0, height = 0;
  if (pimpl->plugin_get_glyph_size((dx_font_loader_plugin_glyph*)SELF->glyphPimpl, &width, &height)) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  Core_String* imageName = NULL;
  if (Core_String_create(&imageName, "<temporary>", sizeof("<temporary>") - 1)) {
    return Core_Failure;
  }
  Core_Assets_Image* image1 = NULL;
  if (Core_Assets_Image_create(&image1, imageName, Core_PixelFormat_L8, width, height)) {
    CORE_UNREFERENCE(imageName);
    imageName = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(imageName);
  imageName = NULL;
  Core_Size numberOfBytesPerPixel;
  if (Core_PixelFormat_getNumberOfBytes(&numberOfBytesPerPixel, Core_PixelFormat_L8)) {
    CORE_UNREFERENCE(image1);
    image1 = NULL;
    return Core_Failure;
  }
  Core_Size overflow;
  Core_Size numberOfPixels;
  Core_safeMulSz(&numberOfPixels, width, height, &overflow); // must succeed
  if (overflow) {
    CORE_UNREFERENCE(image1);
    image1 = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  //
  Core_Size numberOfBytes;
  Core_safeMulSz(&numberOfBytes, numberOfPixels, numberOfBytesPerPixel, &overflow); // must succeed
  if (overflow) {
    CORE_UNREFERENCE(image1);
    image1 = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Memory_copy(image1->backing.pixels, pixels, numberOfBytes);
  *image = image1;
  return Core_Success;
}

static Core_Result Core_DefaultFontSystem_Glyph_getTextureCoordinates(Core_DefaultFontSystem_Glyph* SELF, DX_RECT2_F32* texture_coordinates) {
  dx_rect2_f32_set(texture_coordinates, 0.f, 0.f, 1.f, 1.f);
  return Core_Success;
}
