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

  CORE_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;

  CORE_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_glyph_atlas_constructDispatch(dx_glyph_atlas_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_glyph_atlas_construct(dx_glyph_atlas* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context) {
  DX_CONSTRUCT_PREFIX(dx_glyph_atlas);
  //
  if (!font_manager) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->font_manager = font_manager;
  CORE_REFERENCE(font_manager);
  //
  if (!rectangle_presenter) {
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;

    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->rectangle_presenter = rectangle_presenter;
  CORE_REFERENCE(rectangle_presenter);
  //
  if (!val_context) {
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;

    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;

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

Core_Result dx_glyph_atlas_create(dx_glyph_atlas** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context) {
  DX_CREATE_PREFIX(dx_glyph_atlas)
  if (dx_glyph_atlas_construct(SELF, font_manager, rectangle_presenter, val_context)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_glyph_atlas_get_texture(dx_glyph_atlas* SELF, dx_font_glyph *glyph, dx_val_texture** val_texture, DX_RECT2_F32* texture_coordinates) {
  DX_RECT2_F32 texture_coordinates_1;
  if (dx_font_glyph_get_texture_coordinates(glyph, &texture_coordinates_1)) {
    return Core_Failure;
  }
  dx_assets_texture* assets_texture;
  if (dx_assets_extensions_create_texture_from_glyph(&assets_texture, glyph)) {
    return Core_Failure;
  }
  dx_val_texture* val_texture_1 = NULL;
  if (dx_val_context_create_texture(&val_texture_1, SELF->val_context)) {
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  if (dx_val_texture_set_data(val_texture_1, assets_texture)) {
    CORE_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  if (dx_val_texture_set_texture_address_mode_u(val_texture_1, Core_TextureAddressMode_ClampToBorder) ||
      dx_val_texture_set_texture_address_mode_v(val_texture_1, Core_TextureAddressMode_ClampToBorder) ||
      dx_val_texture_set_texture_minification_filter(val_texture_1, Core_TextureMinificationFilter_Linear) ||
      dx_val_texture_set_texture_magnification_filter(val_texture_1, Core_TextureMagnificationFilter_Linear)) {
    CORE_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    CORE_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return Core_Failure;
  }
  DX_VEC4 border_color = { .e[0] = 0.f, .e[1] = 0.f, .e[2] = 0.f, .e[3] = 0.f };
  if (dx_val_texture_set_texture_border_color(val_texture_1, &border_color)) {
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

static Core_Result create_font(dx_font** RETURN, dx_font_presenter* SELF, char const* path);

static Core_Result
present_glyph
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    Core_Real32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    dx_val_texture* texture,
    Core_InlineRgbaR32 const* color
  );

static Core_Result create_text_presenter(dx_font_presenter* SELF);

static void destroy_text_presenter(dx_font_presenter* SELF);

static Core_Result on_render_code_point(dx_font_presenter* SELF, DX_VEC2_F32* position, uint32_t code_point, dx_font* font, Core_InlineRgbaR32 const* text_color, DX_TEXT_PRESENTATION_OPTIONS const* options);

static Core_Result on_measure_code_point(dx_font_presenter* SELF, DX_VEC2_F32* position, uint32_t code_point, dx_font* font,
                                       DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                       DX_RECT2_F32* bounds);

Core_defineObjectType("dx.font_presenter",
                      dx_font_presenter,
                      Core_Object);

static Core_Result create_font(dx_font** RETURN, dx_font_presenter* SELF, char const* path) {
  Core_String* font_file = NULL;
  if (Core_String_create(&font_file, path, strlen(path))) {
    return Core_Failure;
  }
  dx_font* font = NULL;
  Core_Result result = dx_font_manager_get_or_create_font(&font, SELF->font_manager, font_file, 12);
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
    dx_val_texture* texture,
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
    DX_VEC2_F32 uv;
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
  if (dx_val_command_create_pipeline_state(&command, Core_CullMode_Back, Core_DepthCompareFunction_Always, Core_False)) {
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
  if (dx_val_context_execute_commands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result create_text_presenter(dx_font_presenter* SELF) {
  if (dx_engine_utilities_2d_create_program(&SELF->text.val_program, DX_PRESENTER(SELF)->val_context, "assets/gl", "font")) {
    return Core_Failure;
  }
  if (dx_val_context_create_vbinding(&SELF->text.val_vbinding, DX_PRESENTER(SELF)->val_context, Core_VertexFormat_position_xyz_ambient_uv, SELF->val_buffer)) {
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
static Core_Result on_render_code_point(dx_font_presenter* SELF, DX_VEC2_F32* position, uint32_t code_point, dx_font* font, Core_InlineRgbaR32 const* text_color, DX_TEXT_PRESENTATION_OPTIONS const* options) {
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

  dx_font_glyph* glyph = NULL;
  if (dx_font_glyph_create(&glyph, code_point, font)) {
    // the glyph was not found in the font.
    if (Core_Error_NotFound == Core_getError()) {
      switch (options->glyph_not_availabe_policy) {
        case DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR: {
          Core_setError(Core_Error_NotFound); // redundant
          return Core_Failure;
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER: {
          code_point = (Core_Natural32)'_';
          if (dx_font_glyph_create(&glyph, code_point, font)) {
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
  if (dx_font_glyph_get_glyph_advance(glyph, &_advance_x, &_advance_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 _bearing_x, _bearing_y;
  if (dx_font_glyph_get_glyph_bearing(glyph, &_bearing_x, &_bearing_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  uint32_t char_size_x_n32, char_size_y_n32;
  if (dx_font_glyph_get_size(glyph, &char_size_x_n32, &char_size_y_n32)) {
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
    if (dx_glyph_atlas_create(&glyph_atlas, SELF->font_manager, SELF->rectangle_presenter, DX_PRESENTER(SELF)->val_context)) {
      CORE_UNREFERENCE(glyph);
      glyph = NULL;
      return Core_Failure;
    }
    dx_val_texture* texture;
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
    DX_VEC2_F32 bearing;
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
                                       DX_VEC2_F32* position,
                                       uint32_t code_point,
                                       dx_font* font,
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

  dx_font_glyph* glyph = NULL;
  if (dx_font_glyph_create(&glyph, code_point, font)) {
    // the glyph was not found in the font.
    if (Core_Error_NotFound == Core_getError()) {
      switch (options->glyph_not_availabe_policy) {
        case DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR: {
          Core_setError(Core_Error_NotFound); // redundant
          return Core_Failure;
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER: {
          code_point = (Core_Natural32)'_';
          if (dx_font_glyph_create(&glyph, code_point, font)) {
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
  if (dx_font_glyph_get_glyph_advance(glyph, &_advance_x, &_advance_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 _bearing_x, _bearing_y;
  if (dx_font_glyph_get_glyph_bearing(glyph, &_bearing_x, &_bearing_y)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  uint32_t char_size_x_n32, char_size_y_n32;
  if (dx_font_glyph_get_size(glyph, &char_size_x_n32, &char_size_y_n32)) {
    CORE_UNREFERENCE(glyph);
    glyph = NULL;
    return Core_Failure;
  }
  Core_Real32 char_width = char_size_x_n32;
  Core_Real32 char_height = char_size_y_n32;
  //
  if (options->text_bounds_type == dx_text_bounds_type_logical) {
    Core_Real32 _ascender, _descender;
    if (dx_font_get_ascender(&_ascender, font) || dx_font_get_descender(&_descender, font)) {
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

  CORE_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;
}

static void dx_font_presenter_constructDispatch(dx_font_presenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_font_presenter_construct(dx_font_presenter* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter) {
  DX_CONSTRUCT_PREFIX(dx_font_presenter);
  dx_val_context* val_context = NULL;
  dx_aal_context* aal_context = NULL;
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
  if (!font_manager) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->font_manager = font_manager;
  CORE_REFERENCE(font_manager);
  //
  if (!rectangle_presenter) {
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;

    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->rectangle_presenter = rectangle_presenter;
  CORE_REFERENCE(rectangle_presenter);
  //
  if (dx_val_context_create_buffer(&SELF->val_buffer, DX_PRESENTER(SELF)->val_context)) {
    //
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
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
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
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
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
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
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
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
      CORE_UNREFERENCE(SELF->font_manager);
      SELF->font_manager = NULL;
      //
      return Core_Failure;
    }
  }
  //
  CORE_OBJECT(SELF)->type = TYPE;
  //
  return Core_Success;
}

Core_Result dx_font_presenter_create(dx_font_presenter** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_font_presenter)
  if (dx_font_presenter_construct(SELF, font_manager, rectangle_presenter)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_font_presenter_render_line_string(dx_font_presenter* SELF, DX_VEC2_F32 const* position, Core_String* string, Core_InlineRgbaR32 const* text_color, dx_font* font,
                                               DX_TEXT_PRESENTATION_OPTIONS const* options) {
  dx_string_iterator* string_iterator = NULL;
  if (Core_String_create_iterator(&string_iterator, string)) {
    return Core_Failure;
  }
  DX_VEC2_F32 position1;
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

Core_Result dx_font_presenter_measure_line_string(dx_font_presenter* SELF, DX_VEC2_F32 const* position, Core_String* string, dx_font* font,
                                                DX_TEXT_MEASUREMENT_OPTIONS const* options, DX_RECT2_F32* bounds) {
  dx_string_iterator* string_iterator = NULL;
  if (Core_String_create_iterator(&string_iterator, string)) {
    return Core_Failure;
  }
  DX_VEC2_F32 position1;
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

Core_Result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF, DX_VEC2_F32 const* position, dx_string_iterator* string_iterator, Core_InlineRgbaR32 const* text_color, dx_font* font,
                                                        DX_TEXT_PRESENTATION_OPTIONS const* options) {
  // The distance from the baseline to the maximal extend of any symbol above the baseline.
  Core_Real32 ascender;
  dx_font_get_ascender(&ascender, font);
  // The distance from the baseline to the maximal extend of any symbol below the baseline.
  Core_Real32 descender;
  dx_font_get_descender(&descender, font);

  Core_InlineRgbaR32 red, green;
  dx_rgb_n8_to_rgba_f32(&dx_colors_red, 1.f, &red);
  dx_rgb_n8_to_rgba_f32(&dx_colors_green, 1.f, &green);
  DX_VEC2_F32 pos = *position;

  Core_Boolean has_value;
  if (dx_string_iterator_has_value(&has_value, string_iterator)) {
    return Core_Failure;
  }
  while (has_value) {
    uint32_t code_point;
    if (dx_string_iterator_get_value(&code_point, string_iterator)) {
      return Core_Failure;
    }
    if (on_render_code_point(SELF, &pos, code_point, font, text_color, options)) {
      return Core_Failure;
    }
    if (dx_string_iterator_next(string_iterator)) {
      return Core_Failure;
    }
    if (dx_string_iterator_has_value(&has_value, string_iterator)) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

Core_Result dx_font_presenter_measure_line_string_iterator(dx_font_presenter* SELF,
                                                         DX_VEC2_F32 const* position,
                                                         dx_string_iterator* string_iterator,
                                                         dx_font* font,
                                                         DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                                         DX_RECT2_F32* bounds) {
  // The distance from the baseline to the maximal extend of any symbol above the baseline.
  Core_Real32 ascender;
  dx_font_get_ascender(&ascender, font);
  // The distance from the baseline to the maximal extend of any symbol below the baseline.
  Core_Real32 descender;
  dx_font_get_descender(&descender, font);

  DX_RECT2_F32 bounds1;
  dx_rect2_f32_set2(&bounds1, position->e[0], position->e[1], 0.f, 0.f);

  DX_VEC2_F32 pos = *position;

  Core_Boolean has_value;
  if (dx_string_iterator_has_value(&has_value, string_iterator)) {
    return Core_Failure;
  }
  while (has_value) {
    uint32_t code_point;
    if (dx_string_iterator_get_value(&code_point, string_iterator)) {
      return Core_Failure;
    }
    DX_RECT2_F32 glyph_bounds;
    if (on_measure_code_point(SELF, &pos, code_point, font, options, &glyph_bounds)) {
      return Core_Failure;
    }
    dx_rect2_f32_union(&bounds1, &bounds1, &glyph_bounds);
    if (dx_string_iterator_next(string_iterator)) {
      return Core_Failure;
    }
    if (dx_string_iterator_has_value(&has_value, string_iterator)) {
      return Core_Failure;
    }
  }
  *bounds = bounds1;
  return Core_Success;
}
