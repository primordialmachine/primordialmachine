#include "dx/font_presenter.h"

#include "dx/utilities_2d.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"
#include "dx/val/texture.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.glyph_atlas",
                      dx_glyph_atlas,
                      Core_Object);

static void dx_glyph_atlas_destruct(dx_glyph_atlas* SELF) {
  CORE_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;

  CORE_UNREFERENCE(SELF->fontSystem);
  SELF->fontSystem = NULL;

  CORE_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_glyph_atlas_constructDispatch(dx_glyph_atlas_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_glyph_atlas_construct(dx_glyph_atlas* SELF, Core_FontSystem* fontSystem, dx_rectangle_presenter* rectangle_presenter, Core_Visuals_Context* val_context) {
  DX_CONSTRUCT_PREFIX(dx_glyph_atlas);
  //
  if (!fontSystem) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->fontSystem = fontSystem;
  CORE_REFERENCE(fontSystem);
  //
  if (!rectangle_presenter) {
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;

    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->rectangle_presenter = rectangle_presenter;
  CORE_REFERENCE(rectangle_presenter);
  //
  if (!val_context) {
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;

    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;

    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->val_context = val_context;
  CORE_REFERENCE(SELF->val_context);
  //
  CORE_OBJECT(SELF)->type = TYPE;
  //
  return Core_Success;
}

Core_Result dx_glyph_atlas_create(dx_glyph_atlas** RETURN, Core_FontSystem* fontSystem, dx_rectangle_presenter* rectangle_presenter, Core_Visuals_Context* val_context) {
  DX_CREATE_PREFIX(dx_glyph_atlas)
  if (dx_glyph_atlas_construct(SELF, fontSystem, rectangle_presenter, val_context)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_glyph_atlas_get_texture(dx_glyph_atlas* SELF, Core_Glyph* glyph, Core_Visuals_Texture** val_texture, DX_RECT2_F32* texture_coordinates) {
  DX_RECT2_F32 texture_coordinates_1;
  if (Core_Glyph_getTextureCoordinates(glyph, &texture_coordinates_1)) {
    return Core_Failure;
  }
  Core_Assets_Texture* assets_texture;
  if (Core_Assets_Extensions_createTextureFromGlyph(&assets_texture, glyph)) {
    return Core_Failure;
  }
  Core_Visuals_Texture* val_texture_1 = NULL;
  if (Core_Visuals_Context_createTexture(&val_texture_1, SELF->val_context)) {
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  if (Core_Visuals_Texture_set_data(val_texture_1, assets_texture)) {
    CORE_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  if (Core_Visuals_Texture_set_texture_address_mode_u(val_texture_1, Core_TextureAddressMode_ClampToBorder) ||
      Core_Visuals_Texture_set_texture_address_mode_v(val_texture_1, Core_TextureAddressMode_ClampToBorder) ||
      Core_Visuals_Texture_set_texture_minification_filter(val_texture_1, Core_TextureFilter_Linear) ||
      Core_Visuals_Texture_set_texture_magnification_filter(val_texture_1, Core_TextureFilter_Linear)) {
    CORE_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  Core_InlineRgbaR32 border_color = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
  if (Core_Visuals_Texture_set_texture_border_color(val_texture_1, &border_color)) {
    CORE_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  *texture_coordinates = texture_coordinates_1;
  *val_texture = val_texture_1;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result create_font(Core_Font** RETURN, dx_font_presenter* SELF, char const* path);

static Core_Result
present_glyph
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    Core_Real32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    Core_Visuals_Texture* texture,
    Core_InlineRgbaR32 const* color
  );

static Core_Result create_text_presenter(dx_font_presenter* SELF);

static void destroy_text_presenter(dx_font_presenter* SELF);

static Core_Result on_render_code_point(dx_font_presenter* SELF, Core_InlineVector2R32* position, uint32_t code_point, Core_Font* font, Core_InlineRgbaR32 const* text_color, DX_TEXT_PRESENTATION_OPTIONS const* options);

static Core_Result on_measure_code_point(dx_font_presenter* SELF, Core_InlineVector2R32* position, uint32_t code_point, Core_Font* font,
                                         DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                         DX_RECT2_F32* bounds);

Core_defineObjectType("dx.font_presenter",
                      dx_font_presenter,
                      Core_Object);

static Core_Result create_font(Core_Font** RETURN, dx_font_presenter* SELF, char const* path) {
  Core_String* font_file = NULL;
  if (Core_String_create(&font_file, path, strlen(path))) {
    return Core_Failure;
  }
  Core_Font* font = NULL;
  Core_Result result = Core_FontSystem_getOrCreateFont(&font, SELF->fontSystem, font_file, 12);
  CORE_UNREFERENCE(font_file);
  font_file = NULL;
  if (result) {
    return Core_Failure;
  }
  *RETURN = font;
  return Core_Success;
}

static Core_Result
present_glyph
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    Core_Real32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    Core_Visuals_Texture* texture,
    Core_InlineRgbaR32 const* color
  )
{
  if (!texture || !texture_coordinate_rectangle) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // configure constant binding
  dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_text_color", color);
  dx_val_cbinding_set_texture_index(SELF->val_cbinding, "texture_sampler", 0);
  
  dx_val_command* command;

  dx_val_command_list_clear(SELF->val_command_list);

  struct {
    DX_VEC3 xyz;
    Core_InlineVector2R32 uv;
  } vertices[] = {
      { dx_rect2_f32_get_left(target_rectangle),  dx_rect2_f32_get_bottom(target_rectangle), target_depth, dx_rect2_f32_get_left(texture_coordinate_rectangle),  dx_rect2_f32_get_left(texture_coordinate_rectangle), },
      { dx_rect2_f32_get_right(target_rectangle), dx_rect2_f32_get_bottom(target_rectangle), target_depth, dx_rect2_f32_get_right(texture_coordinate_rectangle), dx_rect2_f32_get_left(texture_coordinate_rectangle), },
      { dx_rect2_f32_get_right(target_rectangle), dx_rect2_f32_get_top(target_rectangle),    target_depth, dx_rect2_f32_get_right(texture_coordinate_rectangle), dx_rect2_f32_get_right(texture_coordinate_rectangle), },

      { dx_rect2_f32_get_right(target_rectangle), dx_rect2_f32_get_top(target_rectangle),    target_depth, dx_rect2_f32_get_right(texture_coordinate_rectangle), dx_rect2_f32_get_right(texture_coordinate_rectangle), },
      { dx_rect2_f32_get_left(target_rectangle),  dx_rect2_f32_get_top(target_rectangle),    target_depth, dx_rect2_f32_get_left(texture_coordinate_rectangle),  dx_rect2_f32_get_right(texture_coordinate_rectangle), },
      { dx_rect2_f32_get_left(target_rectangle),  dx_rect2_f32_get_bottom(target_rectangle), target_depth, dx_rect2_f32_get_left(texture_coordinate_rectangle),  dx_rect2_f32_get_left(texture_coordinate_rectangle), },
  };
  dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, Core_CullMode_Back, Core_DepthCompareMode_Always, Core_False)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // draw command
  if (dx_val_command_create_draw(&command, SELF->text.val_vbinding, texture, SELF->val_cbinding, SELF->text.val_program, 0, 6)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (Core_Visuals_Context_executeCommands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result create_text_presenter(dx_font_presenter* SELF) {
  if (dx_engine_utilities_2d_create_program(&SELF->text.val_program, DX_PRESENTER(SELF)->val_context, "assets/gl", "font")) {
    return Core_Failure;
  }
  if (Core_Visuals_Context_createVariableBinding(&SELF->text.val_vbinding, DX_PRESENTER(SELF)->val_context, Core_VertexFormat_PositionXyzAmbientUv, SELF->val_buffer)) {
    //
    CORE_UNREFERENCE(SELF->text.val_program);
    SELF->text.val_program = NULL;
    //
    return Core_Failure;
  }
  return Core_Success;
}

static void destroy_text_presenter(dx_font_presenter* SELF) {
  CORE_UNREFERENCE(SELF->text.val_vbinding);
  SELF->text.val_vbinding = NULL;
  CORE_UNREFERENCE(SELF->text.val_program);
  SELF->text.val_program = NULL;
}

/// @param [in,out] position A pointer to the position. After the code point was rendered, the position is advanced.
static Core_Result on_render_code_point(dx_font_presenter* SELF, Core_InlineVector2R32* position, uint32_t code_point, Core_Font* font, Core_InlineRgbaR32 const* text_color, DX_TEXT_PRESENTATION_OPTIONS const* options) {
  // The colors "red" and "green".
  // @todo Cache those.
  Core_InlineRgbaR32 red, green;
  dx_rgb_n8_to_rgba_f32(&dx_colors_red, 1.f, &red);
  dx_rgb_n8_to_rgba_f32(&dx_colors_green, 1.f, &green);

  if (code_point == '\n' || code_point == '\r') {
    // the code point is not presentable.
    switch (options->code_point_not_presentable_policy) {
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_ERROR: {
        Core_setError(Core_Error_OperationInvalid);
        return Core_Failure;
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_PLACEHOLDER: {
        code_point = (Core_Natural32)'_';
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP: {
        return Core_Success;
      } break;
    };
  }

  Core_Glyph* glyph = NULL;
  if (Core_DefaultFontSystem_Glyph_create((Core_DefaultFontSystem_Glyph**) & glyph, code_point, CORE_DEFAULTFONTSYSTEM_FONT(font))) {
    // the glyph was not found in the font.
    if (Core_Error_NotFound == Core_getError()) {
      switch (options->glyph_not_availabe_policy) {
        case DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR: {
          Core_setError(Core_Error_NotFound); // redundant
          return Core_Failure;
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER: {
          code_point = (Core_Natural32)'_';
          if (Core_DefaultFontSystem_Glyph_create((Core_DefaultFontSystem_Glyph**) & glyph, code_point, CORE_DEFAULTFONTSYSTEM_FONT(font))) {
            return Core_Failure;
          }
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP: {
          return Core_Success;
        } break;
      };
    } else {
      return Core_Failure;
    }
  }
  Core_Real32 _advance_x, _advance_y;
  if (Core_Glyph_getGlyphAdvance(glyph, &_advance_x, &_advance_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 _bearing_x, _bearing_y;
  if (Core_Glyph_getGlyphBearing(glyph, &_bearing_x, &_bearing_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  uint32_t char_size_x_n32, char_size_y_n32;
  if (Core_Glyph_getSize(glyph, &char_size_x_n32, &char_size_y_n32)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 char_width = char_size_x_n32;
  Core_Real32 char_height = char_size_y_n32;
  //
  if (options->present_glyph_ascender || options->present_glyph_descender) {
    // the ascender
    if (options->present_glyph_ascender) {
      // the positive y axis goes from bottom to top
      DX_RECT2_F32 target_rectangle;
      dx_rect2_f32_set(&target_rectangle, position->e[0] + _bearing_x,
                                          position->e[1],
                                          position->e[0] + _bearing_x + char_width,
                                          position->e[1] + _bearing_y);
      dx_rectangle_presenter_stroke_rectangle(SELF->rectangle_presenter, &target_rectangle, 0.f, &red);
    }
    // the descender
    if (options->present_glyph_descender) {
      // the positive y axis goes from bottom to top
      float d = _bearing_y - char_height;
      if (d < 0) {
        // Only draw this if there is a actual extend below to baseline.
        // Note that some chars like '(' or ')' actually have an extend below the baseline although it is hard to notice.
        DX_RECT2_F32 target_rectangle;
        dx_rect2_f32_set(&target_rectangle,
                         position->e[0] + _bearing_x,
                         position->e[1] + d,
                         position->e[0] + _bearing_x + char_width,
                         position->e[1]);
        dx_rectangle_presenter_stroke_rectangle(SELF->rectangle_presenter, &target_rectangle, 0.f, &green);
      }
    }
  }
  if (options->present_glyph) {
    dx_glyph_atlas* glyph_atlas = NULL;
    if (dx_glyph_atlas_create(&glyph_atlas, SELF->fontSystem, SELF->rectangle_presenter, DX_PRESENTER(SELF)->val_context)) {
      CORE_UNREFERENCE(glyph);
      glyph = NULL;
      return Core_Failure;
    }
    Core_Visuals_Texture* texture;
    DX_RECT2_F32 texture_coordinates;
    if (dx_glyph_atlas_get_texture(glyph_atlas, glyph, &texture, &texture_coordinates)) {
      CORE_UNREFERENCE(glyph_atlas);
      glyph_atlas = NULL;
      CORE_UNREFERENCE(glyph);
      glyph = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(glyph_atlas);
    glyph_atlas = NULL;

    // the positive y axis goes from bottom to top
    DX_RECT2_F32 target_rectangle;
    dx_rect2_f32_set(&target_rectangle,
                     position->e[0],
                     position->e[1] - char_height,
                     position->e[0] + char_width,
                     position->e[1]);
    Core_InlineVector2R32 bearing;
    dx_vec2_f32_set(&bearing, _bearing_x, _bearing_y);
    dx_rect2_f32_translate(&target_rectangle, &bearing);
    present_glyph(SELF,
                  &target_rectangle,
                  0.f,
                  &texture_coordinates,
                  texture,
                  text_color);
    CORE_UNREFERENCE(texture);
    texture = NULL;
  }
  position->e[0] += _advance_x; // advance
  CORE_UNREFERENCE(glyph);
  glyph = NULL;
  return Core_Success;
}

/// @param [in,out] position A pointer to the position. After the code point was measured, the position is advanced.
/// @param [out] bounds A pointer to a rectangle receiving the bounds of the code point's glyph.
/// This is (position,(0,0)) if the code point is not representable and code_point_not_representable_policy is DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP.
/// @method{dx_font_presenter}
static Core_Result on_measure_code_point(dx_font_presenter* SELF,
                                         Core_InlineVector2R32* position,
                                         uint32_t code_point,
                                         Core_Font* font,
                                         DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                         DX_RECT2_F32* bounds) {
  if (code_point == '\n' || code_point == '\r') {
    // the code point is not presentable.
    switch (options->code_point_not_presentable_policy) {
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_ERROR: {
        Core_setError(Core_Error_OperationInvalid);
        return Core_Failure;
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_PLACEHOLDER: {
        code_point = (Core_Natural32)'_';
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP: {
        dx_rect2_f32_set(bounds,
                        position->e[0],
                        position->e[1],
                        position->e[0],
                        position->e[1]);
        return Core_Success;
      } break;
    };
  }

  Core_Glyph* glyph = NULL;
  if (Core_DefaultFontSystem_Glyph_create((Core_DefaultFontSystem_Glyph**) & glyph, code_point, CORE_DEFAULTFONTSYSTEM_FONT(font))) {
    // the glyph was not found in the font.
    if (Core_Error_NotFound == Core_getError()) {
      switch (options->glyph_not_availabe_policy) {
        case DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR: {
          Core_setError(Core_Error_NotFound); // redundant
          return Core_Failure;
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER: {
          code_point = (Core_Natural32)'_';
          if (Core_DefaultFontSystem_Glyph_create((Core_DefaultFontSystem_Glyph**)&glyph, code_point, CORE_DEFAULTFONTSYSTEM_FONT(font))) {
            return Core_Failure;
          }
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP: {
          return Core_Success;
        } break;
      };
    } else {
      return Core_Failure;
    }
  }
  Core_Real32 _advance_x, _advance_y;
  if (Core_Glyph_getGlyphAdvance(glyph, &_advance_x, &_advance_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 _bearing_x, _bearing_y;
  if (Core_Glyph_getGlyphBearing(glyph, &_bearing_x, &_bearing_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  uint32_t char_size_x_n32, char_size_y_n32;
  if (Core_Glyph_getSize(glyph, &char_size_x_n32, &char_size_y_n32)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 char_width = char_size_x_n32;
  Core_Real32 char_height = char_size_y_n32;
  //
  if (options->text_bounds_type == dx_text_bounds_type_logical) {
    Core_Real32 _ascender, _descender;
    if (Core_Font_getAscender(&_ascender, CORE_FONT(font)) || Core_Font_getDescender(&_descender, CORE_FONT(font))) {
      CORE_UNREFERENCE(glyph);
      glyph = NULL;
      return Core_Failure;
    }
    dx_rect2_f32_set(bounds,
                     position->e[0] + _bearing_x,
                     position->e[1] + _descender,
                     position->e[0] + _bearing_x + char_width,
                     position->e[1] + _ascender);
  } else {
    dx_rect2_f32_set(bounds,
                     position->e[0] + _bearing_x,
                     position->e[1] - (char_height - _bearing_y),
                     position->e[0] + _bearing_x + char_width,
                     position->e[1] + _bearing_y);
  }
  position->e[0] += _advance_x; // advance
  CORE_UNREFERENCE(glyph);
  glyph = NULL;
  return Core_Success;
}

static void dx_font_presenter_destruct(dx_font_presenter* SELF) {
  CORE_UNREFERENCE(SELF->val_material);
  SELF->val_material = NULL;

  CORE_UNREFERENCE(SELF->val_command_list);
  SELF->val_command_list = NULL;

  CORE_UNREFERENCE(SELF->val_cbinding);
  SELF->val_cbinding = NULL;

  destroy_text_presenter(SELF);

  CORE_UNREFERENCE(SELF->val_buffer);
  SELF->val_buffer = NULL;

  CORE_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;

  CORE_UNREFERENCE(SELF->fontSystem);
  SELF->fontSystem = NULL;
}

static void dx_font_presenter_constructDispatch(dx_font_presenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_font_presenter_construct(dx_font_presenter* SELF, Core_FontSystem* fontSystem, dx_rectangle_presenter* rectangle_presenter) {
  DX_CONSTRUCT_PREFIX(dx_font_presenter);
  Core_Visuals_Context* val_context = NULL;
  Core_Audials_Context* aal_context = NULL;
  if (dx_presenter_get_val_context(&val_context, DX_PRESENTER(rectangle_presenter))) {
    return Core_Failure;
  }
  if (dx_presenter_get_aal_context(&aal_context, DX_PRESENTER(rectangle_presenter))) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_presenter_construct(DX_PRESENTER(SELF), val_context, aal_context)) {
    CORE_UNREFERENCE(aal_context);
    aal_context = NULL;
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(aal_context);
  aal_context = NULL;
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  //
  if (!fontSystem) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->fontSystem = fontSystem;
  CORE_REFERENCE(fontSystem);
  //
  if (!rectangle_presenter) {
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;

    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->rectangle_presenter = rectangle_presenter;
  CORE_REFERENCE(rectangle_presenter);
  //
  if (Core_Visuals_Context_createBuffer(&SELF->val_buffer, DX_PRESENTER(SELF)->val_context)) {
    //
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    //
    return Core_Failure;
  }
  if (create_text_presenter(SELF)) {
    //
    CORE_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    //
    return Core_Failure;
  }
  //
  if (dx_val_cbinding_create(&SELF->val_cbinding)) {
    //
    destroy_text_presenter(SELF);
    //
    CORE_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    //
    return Core_Failure;
  }
  //
  if (dx_val_command_list_create(&SELF->val_command_list)) {
    //
    CORE_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    destroy_text_presenter(SELF);
    //
    CORE_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    //
    return Core_Failure;
  }
  //
  {
    if (dx_engine_utilities_2d_create_material(&SELF->val_material, DX_PRESENTER(SELF)->val_context, "<font-material>")) {
      //
      CORE_UNREFERENCE(SELF->val_command_list);
      SELF->val_command_list = NULL;
      //
      CORE_UNREFERENCE(SELF->val_cbinding);
      SELF->val_cbinding = NULL;
      //
      destroy_text_presenter(SELF);
      //
      CORE_UNREFERENCE(SELF->val_buffer);
      SELF->val_buffer = NULL;
      //
      CORE_UNREFERENCE(SELF->rectangle_presenter);
      SELF->rectangle_presenter = NULL;
      //
      CORE_UNREFERENCE(SELF->fontSystem);
      SELF->fontSystem = NULL;
      //
      return Core_Failure;
    }
  }
  //
  CORE_OBJECT(SELF)->type = TYPE;
  //
  return Core_Success;
}

Core_Result dx_font_presenter_create(dx_font_presenter** RETURN, Core_FontSystem* fontSystem, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_font_presenter)
  if (dx_font_presenter_construct(SELF, fontSystem, rectangle_presenter)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_font_presenter_render_line_string(dx_font_presenter* SELF, Core_InlineVector2R32 const* position, Core_String* string, Core_InlineRgbaR32 const* text_color, Core_Font* font,
                                                 DX_TEXT_PRESENTATION_OPTIONS const* options) {
  Core_StringIterator* string_iterator = NULL;
  if (Core_String_createIterator(&string_iterator, string)) {
    return Core_Failure;
  }
  Core_InlineVector2R32 position1;
  dx_vec2_f32_set(&position1, position->e[0], position->e[1]);
  if (dx_font_presenter_render_line_string_iterator(SELF, &position1, string_iterator, text_color, font, options)) {
    CORE_UNREFERENCE(string_iterator);
    string_iterator = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(string_iterator);
  string_iterator = NULL;
  return Core_Success;
}

Core_Result dx_font_presenter_measure_line_string(dx_font_presenter* SELF, Core_InlineVector2R32 const* position, Core_String* string, Core_Font* font,
                                                DX_TEXT_MEASUREMENT_OPTIONS const* options, DX_RECT2_F32* bounds) {
  Core_StringIterator* string_iterator = NULL;
  if (Core_String_createIterator(&string_iterator, string)) {
    return Core_Failure;
  }
  Core_InlineVector2R32 position1;
  dx_vec2_f32_set(&position1, position->e[0], position->e[1]);
  if (dx_font_presenter_measure_line_string_iterator(SELF, &position1, string_iterator, font, options, bounds)) {
    CORE_UNREFERENCE(string_iterator);
    string_iterator = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(string_iterator);
  string_iterator = NULL;
  return Core_Success;
}

Core_Result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF, Core_InlineVector2R32 const* position, Core_StringIterator* string_iterator, Core_InlineRgbaR32 const* text_color, Core_Font* font,
                                                          DX_TEXT_PRESENTATION_OPTIONS const* options) {
  // The distance from the baseline to the maximal extend of any symbol above the baseline.
  Core_Real32 ascender;
  Core_Font_getAscender(&ascender, CORE_FONT(font));
  // The distance from the baseline to the maximal extend of any symbol below the baseline.
  Core_Real32 descender;
  Core_Font_getDescender(&descender, CORE_FONT(font));

  Core_InlineRgbaR32 red, green;
  dx_rgb_n8_to_rgba_f32(&dx_colors_red, 1.f, &red);
  dx_rgb_n8_to_rgba_f32(&dx_colors_green, 1.f, &green);
  Core_InlineVector2R32 pos = *position;

  Core_Boolean has_value;
  if (Core_StringIterator_hasValue(&has_value, string_iterator)) {
    return Core_Failure;
  }
  while (has_value) {
    uint32_t code_point;
    if (Core_StringIterator_getValue(&code_point, string_iterator)) {
      return Core_Failure;
    }
    if (on_render_code_point(SELF, &pos, code_point, font, text_color, options)) {
      return Core_Failure;
    }
    if (Core_StringIterator_next(string_iterator)) {
      return Core_Failure;
    }
    if (Core_StringIterator_hasValue(&has_value, string_iterator)) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

Core_Result dx_font_presenter_measure_line_string_iterator(dx_font_presenter* SELF,
                                                           Core_InlineVector2R32 const* position,
                                                           Core_StringIterator* string_iterator,
                                                           Core_Font* font,
                                                           DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                                           DX_RECT2_F32* bounds) {
  // The distance from the baseline to the maximal extend of any symbol above the baseline.
  Core_Real32 ascender;
  Core_Font_getAscender(&ascender, CORE_FONT(font));
  // The distance from the baseline to the maximal extend of any symbol below the baseline.
  Core_Real32 descender;
  Core_Font_getDescender(&descender, CORE_FONT(font));

  DX_RECT2_F32 bounds1;
  dx_rect2_f32_set2(&bounds1, position->e[0], position->e[1], 0.f, 0.f);

  Core_InlineVector2R32 pos = *position;

  Core_Boolean has_value;
  if (Core_StringIterator_hasValue(&has_value, string_iterator)) {
    return Core_Failure;
  }
  while (has_value) {
    uint32_t code_point;
    if (Core_StringIterator_getValue(&code_point, string_iterator)) {
      return Core_Failure;
    }
    DX_RECT2_F32 glyph_bounds;
    if (on_measure_code_point(SELF, &pos, code_point, font, options, &glyph_bounds)) {
      return Core_Failure;
    }
    dx_rect2_f32_union(&bounds1, &bounds1, &glyph_bounds);
    if (Core_StringIterator_next(string_iterator)) {
      return Core_Failure;
    }
    if (Core_StringIterator_hasValue(&has_value, string_iterator)) {
      return Core_Failure;
    }
  }
  *bounds = bounds1;
  return Core_Success;
}
