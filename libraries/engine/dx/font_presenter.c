#include "dx/font_presenter.h"

#include "dx/utilities_2d.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"
#include "dx/val/texture.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.glyph_atlas",
                      dx_glyph_atlas,
                      dx_object);

static void dx_glyph_atlas_destruct(dx_glyph_atlas* SELF) {
  DX_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;

  DX_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;

  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_glyph_atlas_dispatch_construct(dx_glyph_atlas_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_glyph_atlas_construct(dx_glyph_atlas* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context) {
  dx_rti_type* TYPE = dx_glyph_atlas_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  //
  if (!font_manager) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->font_manager = font_manager;
  DX_REFERENCE(font_manager);
  //
  if (!rectangle_presenter) {
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;

    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->rectangle_presenter = rectangle_presenter;
  DX_REFERENCE(rectangle_presenter);
  //
  if (!val_context) {
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;

    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;

    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->val_context = val_context;
  DX_REFERENCE(SELF->val_context);
  //
  DX_OBJECT(SELF)->type = TYPE;
  //
  return DX_SUCCESS;
}

dx_result dx_glyph_atlas_create(dx_glyph_atlas** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context) {
  DX_CREATE_PREFIX(dx_glyph_atlas)
  if (dx_glyph_atlas_construct(SELF, font_manager, rectangle_presenter, val_context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_glyph_atlas_get_texture(dx_glyph_atlas* SELF, dx_font_glyph *glyph, dx_val_texture** val_texture, DX_RECT2_F32* texture_coordinates) {
  DX_RECT2_F32 texture_coordinates_1;
  if (dx_font_glyph_get_texture_coordinates(glyph, &texture_coordinates_1)) {
    return DX_FAILURE;
  }
  dx_assets_texture* assets_texture;
  if (dx_assets_extensions_create_texture_from_glyph(&assets_texture, glyph)) {
    return DX_FAILURE;
  }
  dx_val_texture* val_texture_1 = NULL;
  if (dx_val_context_create_texture(&val_texture_1, SELF->val_context)) {
    DX_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return DX_FAILURE;
  }
  if (dx_val_texture_set_data(val_texture_1, assets_texture)) {
    DX_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    DX_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return DX_FAILURE;
  }
  if (dx_val_texture_set_texture_address_mode_u(val_texture_1, dx_texture_address_mode_clamp_to_border) ||
      dx_val_texture_set_texture_address_mode_v(val_texture_1, dx_texture_address_mode_clamp_to_border) ||
      dx_val_texture_set_texture_minification_filter(val_texture_1, dx_texture_minification_filter_linear) ||
      dx_val_texture_set_texture_magnification_filter(val_texture_1, dx_texture_magnification_filter_linear)) {
    DX_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    DX_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return DX_FAILURE;
  }
  DX_VEC4 border_color = { .e[0] = 0.f, .e[1] = 0.f, .e[2] = 0.f, .e[3] = 0.f };
  if (dx_val_texture_set_texture_border_color(val_texture_1, &border_color)) {
    DX_UNREFERENCE(val_texture_1);
    val_texture_1 = NULL;
    DX_UNREFERENCE(assets_texture);
    assets_texture = NULL;
    return DX_FAILURE;
  }
  *texture_coordinates = texture_coordinates_1;
  *val_texture = val_texture_1;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result create_font(dx_font** RETURN, dx_font_presenter* SELF, char const* path);

static dx_result
present_glyph
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    dx_f32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    dx_val_texture* texture,
    DX_RGBA_F32 const* color
  );

static dx_result create_text_presenter(dx_font_presenter* SELF);

static void destroy_text_presenter(dx_font_presenter* SELF);

static dx_result on_render_code_point(dx_font_presenter* SELF, DX_VEC2_F32* position, uint32_t code_point, dx_font* font, DX_RGBA_F32 const* text_color, DX_TEXT_PRESENTATION_OPTIONS const* options);

static dx_result on_measure_code_point(dx_font_presenter* SELF, DX_VEC2_F32* position, uint32_t code_point, dx_font* font,
                                       DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                       DX_RECT2_F32* bounds);

DX_DEFINE_OBJECT_TYPE("dx.font_presenter",
                      dx_font_presenter,
                      dx_object);

static dx_result create_font(dx_font** RETURN, dx_font_presenter* SELF, char const* path) {
  dx_string* font_file = NULL;
  if (dx_string_create(&font_file, path, strlen(path))) {
    return DX_FAILURE;
  }
  dx_font* font = NULL;
  dx_result result = dx_font_manager_get_or_create_font(&font, SELF->font_manager, font_file, 12);
  DX_UNREFERENCE(font_file);
  font_file = NULL;
  if (result) {
    return DX_FAILURE;
  }
  *RETURN = font;
  return DX_SUCCESS;
}

static dx_result
present_glyph
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    dx_f32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    dx_val_texture* texture,
    DX_RGBA_F32 const* color
  )
{
  if (!texture || !texture_coordinate_rectangle) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
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
  if (dx_val_command_create_pipeline_state(&command, dx_cull_mode_back, dx_depth_test_function_always, DX_FALSE)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // draw command
  if (dx_val_command_create_draw(&command, SELF->text.val_vbinding, texture, SELF->val_cbinding, SELF->text.val_program, 0, 6)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (dx_val_context_execute_commands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result create_text_presenter(dx_font_presenter* SELF) {
  if (dx_engine_utilities_2d_create_program(&SELF->text.val_program, DX_PRESENTER(SELF)->val_context, "assets/gl", "font")) {
    return DX_FAILURE;
  }
  if (dx_val_context_create_vbinding(&SELF->text.val_vbinding, DX_PRESENTER(SELF)->val_context, dx_vertex_format_position_xyz_ambient_uv, SELF->val_buffer)) {
    //
    DX_UNREFERENCE(SELF->text.val_program);
    SELF->text.val_program = NULL;
    //
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static void destroy_text_presenter(dx_font_presenter* SELF) {
  DX_UNREFERENCE(SELF->text.val_vbinding);
  SELF->text.val_vbinding = NULL;
  DX_UNREFERENCE(SELF->text.val_program);
  SELF->text.val_program = NULL;
}

/// @param [in,out] position A pointer to the position. After the code point was rendered, the position is advanced.
static dx_result on_render_code_point(dx_font_presenter* SELF, DX_VEC2_F32* position, uint32_t code_point, dx_font* font, DX_RGBA_F32 const* text_color, DX_TEXT_PRESENTATION_OPTIONS const* options) {
  // The colors "red" and "green".
  // @todo Cache those.
  DX_RGBA_F32 red, green;
  dx_rgb_n8_to_rgba_f32(&dx_colors_red, 1.f, &red);
  dx_rgb_n8_to_rgba_f32(&dx_colors_green, 1.f, &green);

  if (code_point == '\n' || code_point == '\r') {
    // the code point is not presentable.
    switch (options->code_point_not_presentable_policy) {
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_ERROR: {
        dx_set_error(DX_ERROR_INVALID_OPERATION);
        return DX_FAILURE;
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_PLACEHOLDER: {
        code_point = (dx_n32)'_';
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP: {
        return DX_SUCCESS;
      } break;
    };
  }

  dx_font_glyph* glyph = NULL;
  if (dx_font_glyph_create(&glyph, code_point, font)) {
    // the glyph was not found in the font.
    if (DX_ERROR_NOT_FOUND == dx_get_error()) {
      switch (options->glyph_not_availabe_policy) {
        case DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR: {
          dx_set_error(DX_ERROR_NOT_FOUND); // redundant
          return DX_FAILURE;
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER: {
          code_point = (dx_n32)'_';
          if (dx_font_glyph_create(&glyph, code_point, font)) {
            return DX_FAILURE;
          }
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP: {
          return DX_SUCCESS;
        } break;
      };
    } else {
      return DX_FAILURE;
    }
  }
  dx_f32 _advance_x, _advance_y;
  if (dx_font_glyph_get_glyph_advance(glyph, &_advance_x, &_advance_y)) {
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    return DX_FAILURE;
  }
  dx_f32 _bearing_x, _bearing_y;
  if (dx_font_glyph_get_glyph_bearing(glyph, &_bearing_x, &_bearing_y)) {
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    return DX_FAILURE;
  }
  uint32_t char_size_x_n32, char_size_y_n32;
  if (dx_font_glyph_get_size(glyph, &char_size_x_n32, &char_size_y_n32)) {
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    return DX_FAILURE;
  }
  dx_f32 char_width = char_size_x_n32;
  dx_f32 char_height = char_size_y_n32;
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
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
    }
    dx_val_texture* texture;
    DX_RECT2_F32 texture_coordinates;
    if (dx_glyph_atlas_get_texture(glyph_atlas, glyph, &texture, &texture_coordinates)) {
      DX_UNREFERENCE(glyph_atlas);
      glyph_atlas = NULL;
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(glyph_atlas);
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
    DX_UNREFERENCE(texture);
    texture = NULL;
  }
  position->e[0] += _advance_x; // advance
  DX_UNREFERENCE(glyph);
  glyph = NULL;
  return DX_SUCCESS;
}

/// @param [in,out] position A pointer to the position. After the code point was measured, the position is advanced.
/// @param [out] bounds A pointer to a rectangle receiving the bounds of the code point's glyph.
/// This is (position,(0,0)) if the code point is not representable and code_point_not_representable_policy is DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP.
/// @method{dx_font_presenter}
static dx_result on_measure_code_point(dx_font_presenter* SELF,
                                       DX_VEC2_F32* position,
                                       uint32_t code_point,
                                       dx_font* font,
                                       DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                       DX_RECT2_F32* bounds) {
  if (code_point == '\n' || code_point == '\r') {
    // the code point is not presentable.
    switch (options->code_point_not_presentable_policy) {
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_ERROR: {
        dx_set_error(DX_ERROR_INVALID_OPERATION);
        return DX_FAILURE;
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_PLACEHOLDER: {
        code_point = (dx_n32)'_';
      } break;
      case DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP: {
        return DX_SUCCESS;
      } break;
    };
  }

  dx_font_glyph* glyph = NULL;
  if (dx_font_glyph_create(&glyph, code_point, font)) {
    // the glyph was not found in the font.
    if (DX_ERROR_NOT_FOUND == dx_get_error()) {
      switch (options->glyph_not_availabe_policy) {
        case DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR: {
          dx_set_error(DX_ERROR_NOT_FOUND); // redundant
          return DX_FAILURE;
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER: {
          code_point = (dx_n32)'_';
          if (dx_font_glyph_create(&glyph, code_point, font)) {
            return DX_FAILURE;
          }
        } break;
        case DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP: {
          return DX_SUCCESS;
        } break;
      };
    } else {
      return DX_FAILURE;
    }
  }
  dx_f32 _advance_x, _advance_y;
  if (dx_font_glyph_get_glyph_advance(glyph, &_advance_x, &_advance_y)) {
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    return DX_FAILURE;
  }
  dx_f32 _bearing_x, _bearing_y;
  if (dx_font_glyph_get_glyph_bearing(glyph, &_bearing_x, &_bearing_y)) {
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    return DX_FAILURE;
  }
  uint32_t char_size_x_n32, char_size_y_n32;
  if (dx_font_glyph_get_size(glyph, &char_size_x_n32, &char_size_y_n32)) {
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    return DX_FAILURE;
  }
  dx_f32 char_width = char_size_x_n32;
  dx_f32 char_height = char_size_y_n32;
  //
  if (options->text_bounds_type == dx_text_bounds_type_logical) {
    dx_f32 _ascender, _descender;
    if (dx_font_get_ascender(&_ascender, font) || dx_font_get_descender(&_descender, font)) {
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
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
  DX_UNREFERENCE(glyph);
  glyph = NULL;
  return DX_SUCCESS;
}

static void dx_font_presenter_destruct(dx_font_presenter* SELF) {
  DX_UNREFERENCE(SELF->val_material);
  SELF->val_material = NULL;

  DX_UNREFERENCE(SELF->val_command_list);
  SELF->val_command_list = NULL;

  DX_UNREFERENCE(SELF->val_cbinding);
  SELF->val_cbinding = NULL;

  destroy_text_presenter(SELF);

  DX_UNREFERENCE(SELF->val_buffer);
  SELF->val_buffer = NULL;

  DX_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;

  DX_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;
}

static void dx_font_presenter_dispatch_construct(dx_font_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_font_presenter_construct(dx_font_presenter* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter) {
  dx_rti_type* TYPE = dx_font_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  dx_val_context* val_context = NULL;
  dx_aal_context* aal_context = NULL;
  if (dx_presenter_get_val_context(&val_context, DX_PRESENTER(rectangle_presenter))) {
    return DX_FAILURE;
  }
  if (dx_presenter_get_aal_context(&aal_context, DX_PRESENTER(rectangle_presenter))) {
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return DX_FAILURE;
  }
  if (dx_presenter_construct(DX_PRESENTER(SELF), val_context, aal_context)) {
    DX_UNREFERENCE(aal_context);
    aal_context = NULL;
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(aal_context);
  aal_context = NULL;
  DX_UNREFERENCE(val_context);
  val_context = NULL;
  //
  if (!font_manager) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->font_manager = font_manager;
  DX_REFERENCE(font_manager);
  //
  if (!rectangle_presenter) {
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;

    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->rectangle_presenter = rectangle_presenter;
  DX_REFERENCE(rectangle_presenter);
  //
  if (dx_val_context_create_buffer(&SELF->val_buffer, DX_PRESENTER(SELF)->val_context)) {
    //
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    //
    return DX_FAILURE;
  }
  if (create_text_presenter(SELF)) {
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    //
    return DX_FAILURE;
  }
  //
  if (dx_val_cbinding_create(&SELF->val_cbinding)) {
    //
    destroy_text_presenter(SELF);
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    //
    return DX_FAILURE;
  }
  //
  if (dx_val_command_list_create(&SELF->val_command_list)) {
    //
    DX_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    destroy_text_presenter(SELF);
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    //
    return DX_FAILURE;
  }
  //
  {
    if (dx_engine_utilities_2d_create_material(&SELF->val_material, DX_PRESENTER(SELF)->val_context, "<font-material>")) {
      //
      DX_UNREFERENCE(SELF->val_command_list);
      SELF->val_command_list = NULL;
      //
      DX_UNREFERENCE(SELF->val_cbinding);
      SELF->val_cbinding = NULL;
      //
      destroy_text_presenter(SELF);
      //
      DX_UNREFERENCE(SELF->val_buffer);
      SELF->val_buffer = NULL;
      //
      DX_UNREFERENCE(SELF->rectangle_presenter);
      SELF->rectangle_presenter = NULL;
      //
      DX_UNREFERENCE(SELF->font_manager);
      SELF->font_manager = NULL;
      //
      return DX_FAILURE;
    }
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  //
  return DX_SUCCESS;
}

dx_result dx_font_presenter_create(dx_font_presenter** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_font_presenter)
  if (dx_font_presenter_construct(SELF, font_manager, rectangle_presenter)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_font_presenter_render_line_string(dx_font_presenter* SELF, DX_VEC2_F32 const* position, dx_string* string, DX_RGBA_F32 const* text_color, dx_font* font,
                                               DX_TEXT_PRESENTATION_OPTIONS const* options) {
  dx_string_iterator* string_iterator = NULL;
  if (dx_string_create_iterator(&string_iterator, string)) {
    return DX_FAILURE;
  }
  DX_VEC2_F32 position1;
  dx_vec2_f32_set(&position1, position->e[0], position->e[1]);
  if (dx_font_presenter_render_line_string_iterator(SELF, &position1, string_iterator, text_color, font, options)) {
    DX_UNREFERENCE(string_iterator);
    string_iterator = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(string_iterator);
  string_iterator = NULL;
  return DX_SUCCESS;
}

dx_result dx_font_presenter_measure_line_string(dx_font_presenter* SELF, DX_VEC2_F32 const* position, dx_string* string, dx_font* font,
                                                DX_TEXT_MEASUREMENT_OPTIONS const* options, DX_RECT2_F32* bounds) {
  dx_string_iterator* string_iterator = NULL;
  if (dx_string_create_iterator(&string_iterator, string)) {
    return DX_FAILURE;
  }
  DX_VEC2_F32 position1;
  dx_vec2_f32_set(&position1, position->e[0], position->e[1]);
  if (dx_font_presenter_measure_line_string_iterator(SELF, &position1, string_iterator, font, options, bounds)) {
    DX_UNREFERENCE(string_iterator);
    string_iterator = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(string_iterator);
  string_iterator = NULL;
  return DX_SUCCESS;
}

dx_result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF, DX_VEC2_F32 const* position, dx_string_iterator* string_iterator, DX_RGBA_F32 const* text_color, dx_font* font,
                                                        DX_TEXT_PRESENTATION_OPTIONS const* options) {
  // The distance from the baseline to the maximal extend of any symbol above the baseline.
  dx_f32 ascender;
  dx_font_get_ascender(&ascender, font);
  // The distance from the baseline to the maximal extend of any symbol below the baseline.
  dx_f32 descender;
  dx_font_get_descender(&descender, font);

  DX_RGBA_F32 red, green;
  dx_rgb_n8_to_rgba_f32(&dx_colors_red, 1.f, &red);
  dx_rgb_n8_to_rgba_f32(&dx_colors_green, 1.f, &green);
  DX_VEC2_F32 pos = *position;

  dx_bool has_value;
  if (dx_string_iterator_has_value(&has_value, string_iterator)) {
    return DX_FAILURE;
  }
  while (has_value) {
    uint32_t code_point;
    if (dx_string_iterator_get_value(&code_point, string_iterator)) {
      return DX_FAILURE;
    }
    if (on_render_code_point(SELF, &pos, code_point, font, text_color, options)) {
      return DX_FAILURE;
    }
    if (dx_string_iterator_next(string_iterator)) {
      return DX_FAILURE;
    }
    if (dx_string_iterator_has_value(&has_value, string_iterator)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

dx_result dx_font_presenter_measure_line_string_iterator(dx_font_presenter* SELF,
                                                         DX_VEC2_F32 const* position,
                                                         dx_string_iterator* string_iterator,
                                                         dx_font* font,
                                                         DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                                         DX_RECT2_F32* bounds) {
  // The distance from the baseline to the maximal extend of any symbol above the baseline.
  dx_f32 ascender;
  dx_font_get_ascender(&ascender, font);
  // The distance from the baseline to the maximal extend of any symbol below the baseline.
  dx_f32 descender;
  dx_font_get_descender(&descender, font);

  DX_RECT2_F32 bounds1;
  dx_rect2_f32_set2(&bounds1, position->e[0], position->e[1], 0.f, 0.f);

  DX_VEC2_F32 pos = *position;

  dx_bool has_value;
  if (dx_string_iterator_has_value(&has_value, string_iterator)) {
    return DX_FAILURE;
  }
  while (has_value) {
    uint32_t code_point;
    if (dx_string_iterator_get_value(&code_point, string_iterator)) {
      return DX_FAILURE;
    }
    DX_RECT2_F32 glyph_bounds;
    if (on_measure_code_point(SELF, &pos, code_point, font, options, &glyph_bounds)) {
      return DX_FAILURE;
    }
    dx_rect2_f32_union(&bounds1, &bounds1, &glyph_bounds);
    if (dx_string_iterator_next(string_iterator)) {
      return DX_FAILURE;
    }
    if (dx_string_iterator_has_value(&has_value, string_iterator)) {
      return DX_FAILURE;
    }
  }
  *bounds = bounds1;
  return DX_SUCCESS;
}
