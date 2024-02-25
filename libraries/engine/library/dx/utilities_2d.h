#if !defined(DX_ENGINE_UTILITIES_2D_H_INCLUDED)
#define DX_ENGINE_UTILITIES_2D_H_INCLUDED

#include "dx/val/context.h"
#include "dx/val/material.h"
#include "dx/val/program_text.h"
#include "dx/font_manager.h"
#include <string.h>

static inline Core_Result dx_engine_utilities_2d_create_program_text(dx_val_program_text** RETURN, Core_Visuals_Context* context, Core_String* path, Core_String* filename) {
  dx_val_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_val_program_text* program = NULL;
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.vs", sizeof("${s}/${s}.vs") - 1)) {
      return Core_Failure;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      CORE_UNREFERENCE(format);
      format = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&vertex_program, DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, p)) {
      CORE_UNREFERENCE(p);
      p = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(p);
    p = NULL;
  }
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.fs", sizeof("${s}/${s}.fs") - 1)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      return Core_Failure;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      CORE_UNREFERENCE(format);
      format = NULL;
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&fragment_program, DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT, p)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      CORE_UNREFERENCE(p);
      p = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(p);
    p = NULL;
  }
  {
    if (dx_val_program_text_create(&program, vertex_program, fragment_program)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      CORE_UNREFERENCE(fragment_program);
      fragment_program = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    CORE_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  *RETURN = program;
  return Core_Success;
}

static inline Core_Result dx_engine_utilities_2d_create_program(dx_val_program** RETURN, Core_Visuals_Context* val_context, char* path, char* filename) {
  Core_String* path1 = NULL;
  if (Core_String_create(&path1, path, strlen(path))) {
    return Core_Failure;
  }
  Core_String* filename1 = NULL;
  if (Core_String_create(&filename1, filename, strlen(filename))) {
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    return Core_Failure;
  }
  dx_val_program_text* val_program_text = NULL;
  if (dx_engine_utilities_2d_create_program_text(&val_program_text, val_context, path1, filename1)) {
    CORE_UNREFERENCE(filename1);
    filename1 = NULL;
    //
    CORE_UNREFERENCE(path1);
    path1 = NULL;
    //
    return Core_Failure;
  }
  CORE_UNREFERENCE(filename1);
  filename1 = NULL;
  //
  CORE_UNREFERENCE(path1);
  path1 = NULL;
  //
  dx_val_program* val_program;
  if (Core_Visuals_Context_createProgram(&val_program, val_context, val_program_text)) {
    CORE_UNREFERENCE(val_program_text);
    val_program_text = NULL;
    //
    return Core_Failure;
  }
  CORE_UNREFERENCE(val_program_text);
  val_program_text = NULL;
  *RETURN = val_program;
  return Core_Success;
}

/// @todo Asset materials must be created via the assets manager.
static inline Core_Result dx_engine_utilities_2d_create_material(dx_val_material** RETURN, Core_Visuals_Context* context, char const* name) {
  // create the asset material
  Core_String* name_string = NULL;
  if (Core_String_create(&name_string, name, strlen(name))) {
    return Core_Failure;
  }
  Core_Assets_Material* material_asset = NULL;
  if (Core_Assets_Material_create(&material_asset, name_string)) {
    CORE_UNREFERENCE(name_string);
    name_string = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(name_string);
  name_string = NULL;
  Core_Assets_ColorRgbN8* ambient_color = NULL;
  Core_InlineRgbN8 WHITE = { .r = 255, .g = 255, .b = 255 };
  if (Core_Assets_ColorRgbN8_create(&ambient_color, &WHITE)) {
    CORE_UNREFERENCE(material_asset);
    material_asset = NULL;
    return Core_Failure;
  }
  if (Core_Assets_Material_setAmbientColor(material_asset, ambient_color)) {
    CORE_UNREFERENCE(ambient_color);
    ambient_color = NULL;
    CORE_UNREFERENCE(material_asset);
    material_asset = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(ambient_color);
  ambient_color = NULL;
  // create the val material
  dx_val_material* val_material = NULL;
  if (dx_val_material_create(&val_material, context, material_asset)) {
    CORE_UNREFERENCE(material_asset);
    material_asset = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(material_asset);
  material_asset = NULL;
  *RETURN = val_material;
  return Core_Success;
}

static inline Core_Result Core_Assets_Extensions_createTextureFromGlyph(Core_Assets_Texture** RETURN, Core_Glyph* glyph) {
  Core_Assets_Image* image = NULL;
  if (Core_Glyph_getImage(glyph, &image)) {
    return Core_Failure;
  }
  Core_Assets_Ref* imageReference = NULL;
  if (Core_Assets_Ref_create(&imageReference, image->name)) {
    CORE_UNREFERENCE(image);
    image = NULL;
    return Core_Failure;
  }
  imageReference->object = CORE_OBJECT(image);
  //
  Core_String* textureName = NULL;
  if (Core_String_create(&textureName, "<temporary>", sizeof("<temporary>") - 1)) {
    CORE_UNREFERENCE(imageReference);
    imageReference = NULL;
    return Core_Failure;
  }
  Core_Assets_Texture* texture = NULL;
  if (Core_Assets_Texture_create(&texture, textureName, imageReference)) {
    CORE_UNREFERENCE(textureName);
    textureName = NULL;
    CORE_UNREFERENCE(imageReference);
    imageReference = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(textureName);
  textureName = NULL;
  CORE_UNREFERENCE(imageReference);
  imageReference = NULL;
  //
  *RETURN = texture;
  //
  return Core_Success;
}

#endif // DX_ENGINE_UTILITIES_2D_H_INCLUDED
