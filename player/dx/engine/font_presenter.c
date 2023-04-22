#include "dx/engine/font_presenter.h"

#include "dx/engine/utilities_2d.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"
#include "dx/val/texture.h"

static dx_result create_font(dx_font** RETURN, dx_font_presenter* SELF, char const* path);

// if @a texture_coordinate_rectangle is null, then left = 0, bottom = 0, right = 1, and top = 1 are used.
static dx_result
present_rectangle
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    dx_f32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    dx_val_texture* texture,
    DX_RGBA_F32 const* color
  );

static dx_result create_text_bounds_presenter(dx_font_presenter* SELF);

static void destroy_text_bounds_presenter(dx_font_presenter* SELF);

static dx_result create_text_presenter(dx_font_presenter* SELF);

static void destroy_text_presenter(dx_font_presenter* SELF);

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
  // configure constant binding
  dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_text_color", color);
  if (texture) {
    dx_val_cbinding_set_texture_index(SELF->val_cbinding, "texture_sampler", 0);
  }

  dx_val_command* command;

  dx_val_command_list_clear(SELF->val_command_list);

  if (texture) {
    DX_RECT2_F32 t = { .left = 0.f, .right = 1.f, .bottom = 0.f, .top = 1.f };
    if (texture_coordinate_rectangle) {
      t = *texture_coordinate_rectangle;
    }
    struct {
      DX_VEC3 xyz;
      DX_VEC2 uv;
    } vertices[] = {
        { target_rectangle->left,  target_rectangle->bottom, target_depth, t.left,  t.left, },
        { target_rectangle->right, target_rectangle->bottom, target_depth, t.right, t.left, },
        { target_rectangle->right, target_rectangle->top,    target_depth, t.right, t.right, },

        { target_rectangle->right, target_rectangle->top,    target_depth, t.right, t.right, },
        { target_rectangle->left,  target_rectangle->top,    target_depth, t.left,  t.right, },
        { target_rectangle->left,  target_rectangle->bottom, target_depth, t.left,  t.left, },
    };
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  } else {
    struct {
      DX_VEC3 xyz;
    } vertices[] = {
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->top,    target_depth, },

        { target_rectangle->right, target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
    };
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  }

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, DX_CULL_MODE_BACK, DX_DEPTH_TEST_FUNCTION_ALWAYS, DX_FALSE)) {
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
  if (texture) {
    if (dx_val_command_create_draw(&command, SELF->text.val_vbinding, texture, SELF->val_cbinding, SELF->text.val_program, 0, 6)) {
      return DX_FAILURE;
    }
  } else {
    if (dx_val_command_create_draw(&command, SELF->text_bounds.val_vbinding, NULL, SELF->val_cbinding, SELF->text_bounds.val_program, 0, 6)) {
      return DX_FAILURE;
    }
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (dx_val_context_execute_commands(SELF->val_context, SELF->val_command_list)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result
present_rectangle
  (
    dx_font_presenter* SELF,
    DX_RECT2_F32 const* target_rectangle,
    dx_f32 target_depth,
    DX_RECT2_F32 const* texture_coordinate_rectangle,
    dx_val_texture* texture,
    DX_RGBA_F32 const* color
  )
{
  // configure constant binding
  dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_rgba", color);
  if (texture) {
    dx_val_cbinding_set_texture_index(SELF->val_cbinding, "texture_sampler", 0);
  }

  dx_val_command* command;

  dx_val_command_list_clear(SELF->val_command_list);

  if (texture) {
    DX_RECT2_F32 t = { .left = 0.f, .right = 1.f, .bottom = 0.f, .top = 1.f };
    if (texture_coordinate_rectangle) {
      t = *texture_coordinate_rectangle;
    }
    struct {
      DX_VEC3 xyz;
      DX_VEC2 uv;
    } vertices[] = {
        { target_rectangle->left,  target_rectangle->bottom, target_depth, t.left,  t.left, },
        { target_rectangle->right, target_rectangle->bottom, target_depth, t.right, t.left, },
        { target_rectangle->right, target_rectangle->top,    target_depth, t.right, t.right, },

        { target_rectangle->right, target_rectangle->top,    target_depth, t.right, t.right, },
        { target_rectangle->left,  target_rectangle->top,    target_depth, t.left,  t.right, },
        { target_rectangle->left,  target_rectangle->bottom, target_depth, t.left,  t.left, },
    };
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  } else {
    struct {
      DX_VEC3 xyz;
    } vertices[] = {
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->top,    target_depth, },

        { target_rectangle->right, target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
    };
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  }

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, DX_CULL_MODE_BACK, DX_DEPTH_TEST_FUNCTION_ALWAYS, DX_FALSE)) {
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
  if (texture) {
    if (dx_val_command_create_draw(&command, SELF->text.val_vbinding, texture, SELF->val_cbinding, SELF->text.val_program, 0, 6)) {
      return DX_FAILURE;
    }
  } else {
    if (dx_val_command_create_draw(&command, SELF->text_bounds.val_vbinding, NULL, SELF->val_cbinding, SELF->text_bounds.val_program, 0, 6)) {
      return DX_FAILURE;
    }
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (dx_val_context_execute_commands(SELF->val_context, SELF->val_command_list)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result create_text_bounds_presenter(dx_font_presenter* SELF) {
  if (dx_engine_utilities_2d_create_program(&SELF->text_bounds.val_program, SELF->val_context, "assets/gl", "rectangle")) {
    return DX_FAILURE;
  }
  if (dx_val_context_create_vbinding(&SELF->text_bounds.val_vbinding, SELF->val_context, DX_VERTEX_FORMAT_POSITION_XYZ, SELF->val_buffer)) {
    //
    DX_UNREFERENCE(SELF->text_bounds.val_program);
    SELF->text_bounds.val_program = NULL;
    //
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static void destroy_text_bounds_presenter(dx_font_presenter* SELF) {
  DX_UNREFERENCE(SELF->text_bounds.val_vbinding);
  SELF->text_bounds.val_vbinding = NULL;
  DX_UNREFERENCE(SELF->text_bounds.val_program);
  SELF->text_bounds.val_program = NULL;
}

static dx_result create_text_presenter(dx_font_presenter* SELF) {
  if (dx_engine_utilities_2d_create_program(&SELF->text.val_program, SELF->val_context, "assets/gl", "font")) {
    return DX_FAILURE;
  }
  if (dx_val_context_create_vbinding(&SELF->text.val_vbinding, SELF->val_context, DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV, SELF->val_buffer)) {
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

static void dx_font_presenter_destruct(dx_font_presenter* SELF) {
  DX_UNREFERENCE(SELF->val_material);
  SELF->val_material = NULL;

  DX_UNREFERENCE(SELF->val_command_list);
  SELF->val_command_list = NULL;

  DX_UNREFERENCE(SELF->val_cbinding);
  SELF->val_cbinding = NULL;

  destroy_text_presenter(SELF);
  destroy_text_bounds_presenter(SELF);

  DX_UNREFERENCE(SELF->val_buffer);
  SELF->val_buffer = NULL;

  DX_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;

  DX_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;

  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_font_presenter_dispatch_construct(dx_font_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_font_presenter_construct(dx_font_presenter* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context) {
  dx_rti_type* TYPE = dx_font_presenter_get_type();
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

  if (dx_val_context_create_buffer(&SELF->val_buffer, SELF->val_context)) {
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    //
    DX_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    //
    return DX_FAILURE;
  }
  if (create_text_bounds_presenter(SELF)) {
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
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
    destroy_text_bounds_presenter(SELF);
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
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
  SELF->val_cbinding = dx_val_cbinding_create();
  if (!SELF->val_cbinding) {
    //
    destroy_text_presenter(SELF);
    //
    destroy_text_bounds_presenter(SELF);
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
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
    destroy_text_bounds_presenter(SELF);
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
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
    if (dx_engine_utilities_2d_create_material(&SELF->val_material, SELF->val_context, "<font-material>")) {
      //
      DX_UNREFERENCE(SELF->val_command_list);
      SELF->val_command_list = NULL;
      //
      DX_UNREFERENCE(SELF->val_cbinding);
      SELF->val_cbinding = NULL;
      //
      destroy_text_presenter(SELF);
      //
      destroy_text_bounds_presenter(SELF);
      //
      DX_UNREFERENCE(SELF->val_buffer);
      SELF->val_buffer = NULL;
      //
      DX_UNREFERENCE(SELF->val_context);
      SELF->val_context = NULL;
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

dx_result dx_font_presenter_create(dx_font_presenter** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context) {
  dx_font_presenter* SELF = DX_FONT_PRESENTER(dx_object_alloc(sizeof(dx_font_presenter)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_font_presenter_construct(SELF, font_manager, rectangle_presenter, val_context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_font_presenter_render_line_string(dx_font_presenter* SELF, dx_f32 left, dx_f32 bottom, dx_string* string, DX_RGBA_F32 const* text_color, dx_font* font) {
  dx_string_iterator* string_iterator = NULL;
  if (dx_string_iterator_impl_create((dx_string_iterator_impl**)&string_iterator, string)) {
    return DX_FAILURE;
  }
  DX_VEC2 position;
  dx_vec2_set(&position, left, bottom);
  if (dx_font_presenter_render_line_string_iterator(SELF, &position, string_iterator, text_color, font)) {
    DX_UNREFERENCE(string_iterator);
    string_iterator = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(string_iterator);
  string_iterator = NULL;
  return DX_SUCCESS;
}

dx_result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF, DX_VEC2 const *position, dx_string_iterator* string_iterator, DX_RGBA_F32 const* text_color, dx_font* font) {
  uint8_t unknown_symbol_policy = dx_font_presenter_unknown_symbol_policy_error;
  //uint8_t presentation_policy = 0;
  uint8_t presentation_policy = 0
                            /*| dx_font_presenter_presentation_policy_ascender_descener*/
                              | dx_font_presenter_presentation_policy_glyph;
  // the distance from the baseline to the maximal extend of any symbol above the baseline.
  dx_f32 ascender;
  dx_font_get_ascender(&ascender, font);
  // the distance from the baseline to the maximal extend of any symbol below the baseline.
  dx_f32 descender;
  dx_font_get_descender(&descender, font);

  DX_RGBA_F32 red, green;
  dx_rgb_n8_to_rgba_f32(&dx_colors_red, 1.f, &red);
  dx_rgb_n8_to_rgba_f32(&dx_colors_green, 1.f, &green);
  dx_f32 startx = position->e[0];

  dx_bool has_value;
  if (dx_string_iterator_has_value(&has_value, string_iterator)) {
    return DX_FAILURE;
  }
  while (has_value) {
    uint32_t code_point;
    if (dx_string_iterator_get_value(&code_point, string_iterator)) {
      return DX_FAILURE;
    }
    dx_font_glyph* glyph = NULL;
    if (dx_font_glyph_create(&glyph, code_point, font)) {
      if (DX_ERROR_NOT_FOUND == dx_get_error() && unknown_symbol_policy == dx_font_presenter_unknown_symbol_policy_placeholder) {
        dx_set_error(DX_NO_ERROR);
        if (dx_font_glyph_create(&glyph, (uint32_t)'_', font)) {
          return DX_FAILURE;
        }
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
    DX_RECT2_F32 texture_coordinates;
    if (dx_font_glyph_get_texture_coordinates(glyph, &texture_coordinates.left, &texture_coordinates.bottom, &texture_coordinates.right, &texture_coordinates.top)) {
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
    }
    //
    dx_asset_texture* asset_texture;
    if (dx_assets_extensions_create_texture_from_glyph(&asset_texture, glyph)) {
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
    }
    dx_val_texture* texture = NULL;
    if (dx_val_context_create_texture(&texture, SELF->val_context)) {
      DX_UNREFERENCE(asset_texture);
      asset_texture = NULL;
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
    }
    if (dx_val_texture_set_data(texture, asset_texture)) {
      DX_UNREFERENCE(texture);
      texture = NULL;
      DX_UNREFERENCE(asset_texture);
      asset_texture = NULL;
      DX_UNREFERENCE(glyph);
      glyph = NULL;
      return DX_FAILURE;
    }
    dx_val_texture_set_texture_address_mode_u(texture, DX_TEXTURE_ADDRESS_MODE_CLAMP_TO_BORDER);
    dx_val_texture_set_texture_address_mode_v(texture, DX_TEXTURE_ADDRESS_MODE_CLAMP_TO_BORDER);
    dx_val_texture_set_texture_minification_filter(texture, DX_TEXTURE_MINIFICATION_FILTER_LINEAR);
    dx_val_texture_set_texture_magnification_filter(texture, DX_TEXTURE_MAGNIFICATION_FILTER_LINEAR);
    DX_VEC4 border_color = { .e[0] = 0.f, .e[1] = 0.f, .e[2] = 0.f, .e[3] = 0.f };
    dx_val_texture_set_texture_border_color(texture, &border_color);
    dx_f32 char_width = DX_ASSET_IMAGE(asset_texture->image_reference->object)->width;
    dx_f32 char_height = DX_ASSET_IMAGE(asset_texture->image_reference->object)->height;
    //
    if (dx_font_presenter_presentation_policy_ascender_descener == (presentation_policy & dx_font_presenter_presentation_policy_ascender_descener)) {
      DX_RECT2_F32 target_rectangle;

      target_rectangle = (DX_RECT2_F32){
        .left = startx + _bearing_x,
        .bottom = position->e[1],
        .right = startx + _bearing_x + char_width,
        .top = position->e[1] + _bearing_y,
      };
      present_rectangle(SELF,
                        &target_rectangle,
                        0.f,
                        NULL,
                        NULL,
                        &red);

      target_rectangle = (DX_RECT2_F32){
        .left = startx + _bearing_x,
        .bottom = position->e[1] - (char_height - _bearing_y),
        .right = startx + _bearing_x + char_width,
        .top = position->e[1],
      };
      present_rectangle(SELF,
                        &target_rectangle,
                        0.f,
                        NULL,
                        NULL,
                        &green);
    }
    if (dx_font_presenter_presentation_policy_glyph == (presentation_policy & dx_font_presenter_presentation_policy_glyph)) {
      DX_RECT2_F32 target_rectangle;
      target_rectangle = (DX_RECT2_F32){
        .left = startx + _bearing_x,
        .bottom = position->e[1] - (char_height - _bearing_y),
        .right = startx + _bearing_x + char_width,
        .top = position->e[1] + _bearing_y,
      };
      present_glyph(SELF,
                    &target_rectangle,
                    0.f,
                    &texture_coordinates,
                    texture,
                    text_color);
    }
    startx += _advance_x; // advance
    DX_UNREFERENCE(texture);
    texture = NULL;
    DX_UNREFERENCE(asset_texture);
    asset_texture = NULL;
    DX_UNREFERENCE(glyph);
    glyph = NULL;
    if (dx_string_iterator_next(string_iterator)) {
      return DX_FAILURE;
    }
    if (dx_string_iterator_has_value(&has_value, string_iterator)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}
