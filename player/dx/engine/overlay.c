#include "dx/engine/overlay.h"


#include "dx/val/command.h"
#include "dx/val/program_text.h"
#include "dx/val/texture.h"
#include "dx/engine/utilities_2d.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// the font file
static char* const FONT_FILE = "./assets/fonts/SourceCodePro-Regular.ttf";
/// @brief The font size of the console in points.
/// @todo
/// The font size is 12pt. However, a DPI scaling of 225% is currently computed in such that the font size is 27pt = 12pt * 225%.
/// Font sizes in GUIs should be specified without incorporating some machine/monitor-specific DPI scalings.
static dx_size const FONT_SIZE = 27;
// the console background color
static DX_RGBA_F32 const BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
// the text color
static DX_RGBA_F32 const TEXT_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f };

DX_DEFINE_OBJECT_TYPE("dx.overlay",
                      dx_overlay,
                      dx_object);

/// @Ensure that SELF->font is pointing to a suitable font.
/// @param SELF A pointer to this console.
/// @default-return
/// @default-failure
static dx_result create_font(dx_overlay* SELF) {
  dx_string* font_file = NULL;
  if (dx_string_create(&font_file, FONT_FILE, strlen(FONT_FILE))) {
    return DX_FAILURE;
  }
  dx_font* font = NULL;
  dx_result result = dx_font_manager_get_or_create_font(&font, SELF->font_presenter->font_manager, font_file, FONT_SIZE);
  DX_UNREFERENCE(font_file);
  font_file = NULL;
  if (result) {
    return DX_FAILURE;
  }
  SELF->font = font;
  return DX_SUCCESS;
}

static void dx_overlay_destruct(dx_overlay* SELF) {
  DX_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;

  DX_UNREFERENCE(SELF->command_list);
  SELF->command_list = NULL;

  DX_UNREFERENCE(SELF->font);
  SELF->font = NULL;

  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;

  dx_inline_pointer_array_uninitialize(&SELF->lines);
}

static void dx_overlay_dispatch_construct(dx_overlay_dispatch* SELF)
{/*Intentionally empty.*/}

static void line_added_callback(dx_string** element) {
  DX_REFERENCE(*element);
}

static void line_removed_callback(dx_string** element) {
  DX_UNREFERENCE(*element);
}

dx_result dx_overlay_construct(dx_overlay* SELF, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* val_context) {
  dx_rti_type* TYPE = dx_overlay_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  static DX_INLINE_POINTER_ARRAY_CONFIGURATION const configuration = {
    .added_callback = (void(*)(void**)) & line_added_callback,
    .removed_callback = (void(*)(void**)) & line_removed_callback,
  };
  if (dx_inline_pointer_array_initialize(&SELF->lines, 0, &configuration)) {
    return DX_FAILURE;
  }
  if (!val_context) {
    dx_inline_pointer_array_uninitialize(&SELF->lines);
    return DX_FAILURE;
  }
  SELF->val_context = val_context;
  DX_REFERENCE(SELF->val_context);
  if (dx_val_command_list_create(&SELF->command_list)) {
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    dx_inline_pointer_array_uninitialize(&SELF->lines);
    return DX_FAILURE;
  }
  SELF->visible = true;
  if (!font_presenter) {
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    dx_inline_pointer_array_uninitialize(&SELF->lines);
    return DX_FAILURE;
  }
  SELF->font_presenter = font_presenter;
  DX_REFERENCE(SELF->font_presenter);
  if (create_font(SELF)) {
    DX_UNREFERENCE(SELF->font_presenter);
    SELF->font_presenter = NULL;
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    dx_inline_pointer_array_uninitialize(&SELF->lines);
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_overlay_create(dx_overlay** RETURN, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* context) {
  dx_overlay* SELF = DX_OVERLAY(dx_object_alloc(sizeof(dx_overlay)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_overlay_construct(SELF, font_presenter, font_manager, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_overlay_add_message(dx_overlay* SELF, dx_string* message) {
  return dx_inline_pointer_array_append(&SELF->lines, message);
  return DX_SUCCESS;
}

dx_result dx_overlay_clear_messages(dx_overlay* SELF) {
  return dx_inline_pointer_array_clear(&SELF->lines);
}

dx_result dx_overlay_render(dx_overlay* SELF, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  if (!SELF->visible) {
    return DX_SUCCESS;
  }
  // this is basically the standard distance from one baseline to another.
  // usually, you want this value to be multiplied by 1.5.
  dx_f32 baseline_distance;
  dx_font_get_baseline_distance(&baseline_distance, SELF->font);
  // the distance from the baseline to the maximal extend of any symbol above the baseline.
  dx_f32 ascender;
  dx_font_get_ascender(&ascender, SELF->font);
  // the distance from the baseline to the maximal extend of any symbol below the baseline.
  dx_f32 descender;
  dx_font_get_descender(&descender, SELF->font);
  // the distance of the baselines of two lines.
  baseline_distance = baseline_distance * 1.5f;
  dx_f32 insets_x = 16.f;
  dx_f32 insets_y = 16.f;
  // Make a viewport that starts at (0,0) and extends to (canvas_width, canvas_height).
  // The viewport width is fixed to canvas width. 
  dx_f32 viewport_width = canvas_width;
  // The viewport height is fixed to canvas height.
  dx_f32 viewport_height = canvas_height;
  // The viewport left is fixed to 0.
  dx_f32 viewport_left = 0.f;
  // The viewport bottom is fixed to 0.
  dx_f32 viewport_bottom = 0.f;

  dx_f32 max_line_height = ascender - descender;

  // the width of the window.
  dx_f32 console_width = canvas_width / 4.f;
  // the heigh of the window.
  dx_f32 console_height = canvas_height - 16.f * 4;
  // the position of the console along the x-axis.
  dx_f32 console_position_x = canvas_width - canvas_width / 4.f;
  // the position of the console along the y-axis.
  dx_f32 console_position_y = canvas_height - console_height;

  dx_val_command* command;

  dx_val_command_list_clear(SELF->command_list);

  // set viewport command
  if (dx_val_command_create_viewport(&command, viewport_left, viewport_bottom, viewport_width, viewport_height)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  //
  // execute the commands
  if (dx_val_context_execute_commands(SELF->val_context, SELF->command_list)) {
    return DX_FAILURE;
  }
  {
    DX_MAT4 world_matrix;
    dx_mat4_set_identity(&world_matrix);
    dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
    dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
    DX_MAT4 view_matrix;
    dx_mat4_set_identity(&view_matrix);
    dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
    dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
    DX_MAT4 projection_matrix;
    dx_mat4_set_ortho(&projection_matrix, 0, canvas_width, 0, canvas_height, -1, +1);
    dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
    dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
  }
  //
  {
    DX_RECT2_F32 target_rectangle = {
      .left = console_position_x,
      .bottom = console_position_y,
      .right = console_position_x + console_width,
      .top = console_position_y + console_height,
    };
    dx_rectangle_presenter_fill_rectangle(SELF->font_presenter->rectangle_presenter,
                                          &target_rectangle,
                                          0.f,
                                          &BACKGROUND_COLOR);
  }
  //
  {
    // render the lines in order
    dx_f32 starty = console_position_y + console_height - insets_y - ascender;
    dx_f32 startx = console_position_x + insets_x;
    dx_size n;
    if (dx_inline_pointer_array_get_size(&n, &SELF->lines)) {
      return DX_FAILURE;
    }
    for (dx_size i = 0; i < n; ++i) {
      dx_string* line = NULL;
      if (dx_inline_pointer_array_get_at(&line, &SELF->lines, i)) {
        return DX_FAILURE;
      }
      dx_font_presenter_render_line_string(SELF->font_presenter, console_position_x + insets_x, starty, line, &TEXT_COLOR, SELF->font);
      starty -= baseline_distance;
    }
  }

  return DX_SUCCESS;
}

dx_result dx_overlay_set_visible(dx_overlay* SELF, bool visible) {
  SELF->visible = visible;
  return DX_SUCCESS;
}

dx_result dx_overlay_get_visible(bool *RETURN, dx_overlay* SELF) {
  *RETURN = SELF->visible;
  return DX_SUCCESS;
}
