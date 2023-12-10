#include "dx/engine/overlay.h"

#include "dx/ui/manager.h"
// strlen
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// The overlay background color.
static DX_RGBA_F32 const OVERLAY_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
/// The text ui background color.
static DX_RGBA_F32 const TEXT_UI_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
/// The text ui text color.
static DX_RGBA_F32 const TEXT_UI_TEXT_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f };

DX_DEFINE_OBJECT_TYPE("dx.overlay",
                      dx_overlay,
                      Core_Object);

static void dx_overlay_destruct(dx_overlay* SELF) {
  DX_UNREFERENCE(SELF->text_field);
  SELF->text_field = NULL;

  DX_UNREFERENCE(SELF->ui_manager);
  SELF->ui_manager = NULL;
}

static void dx_overlay_constructDispatch(dx_overlay_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_overlay_construct(dx_overlay* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CONSTRUCT_PREFIX(dx_overlay);
  SELF->visible = true;
  if (dx_ui_manager_create(&SELF->ui_manager, font_presenter, rectangle_presenter)) {
    return Core_Failure;
  }
  if (dx_ui_text_field_create(&SELF->text_field, SELF->ui_manager)) {
    DX_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_overlay_create(dx_overlay** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_overlay)
  if (dx_overlay_construct(SELF, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_overlay_add_message(dx_overlay* SELF, Core_String* message) {
  return dx_ui_text_field_append_text(SELF->text_field, message);
}

Core_Result dx_overlay_clear_messages(dx_overlay* SELF) {
  Core_String* text = NULL;
  if (Core_String_create(&text, "", strlen(""))) {
    return Core_Failure;
  }
  if (dx_ui_text_field_set_text(SELF->text_field, text)) {
    DX_UNREFERENCE(text);
    text = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(text);
  text = NULL;
  return Core_Success;
}

Core_Result dx_overlay_render(dx_overlay* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_size_horizontal, Core_Integer32 canvas_size_vertical, Core_Integer32 dpi_horizontal, Core_Integer32 dpi_vertical) {
  if (!SELF->visible) {
    return Core_Success;
  }
  // the distance from the baseline to the maximal extend of any symbol above the baseline.
  Core_Real32 ascender;
  dx_font_get_ascender(&ascender, SELF->text_field->font);
  Core_Real32 insets_x = 16.f;
  Core_Real32 insets_y = 16.f;

  // the width of the window.
  Core_Real32 console_width = canvas_size_horizontal / 4.f;
  // the heigh of the window.
  Core_Real32 console_height = canvas_size_vertical - 16.f * 4;
  // the position of the console along the x-axis.
  Core_Real32 console_position_x = canvas_size_horizontal - canvas_size_horizontal / 4.f;
  // the position of the console along the y-axis.
  Core_Real32 console_position_y = canvas_size_vertical - console_height;

  DX_VEC2_F32 temporary;
  {
    dx_vec2_f32_set(&temporary, dpi_horizontal, dpi_vertical);
    dx_ui_manager_set_canvas_dpi(SELF->ui_manager, &temporary);

    dx_vec2_f32_set(&temporary, canvas_size_horizontal, canvas_size_vertical);
    dx_ui_manager_set_canvas_resolution(SELF->ui_manager, &temporary);

    dx_ui_manager_enter_render(SELF->ui_manager);
  }
  //
  {
    Core_Real32 starty = console_position_y + console_height - insets_y - ascender;
    Core_Real32 startx = console_position_x + insets_x;
    dx_vec2_f32_set(&temporary, startx, starty);
    dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->text_field), &temporary);
    dx_ui_text_field_set_background_color(SELF->text_field, &TEXT_UI_BACKGROUND_COLOR);
    dx_ui_text_field_set_text_color(SELF->text_field, &TEXT_UI_TEXT_COLOR);
  }
  // render
  dx_ui_manager_set_root(SELF->ui_manager, DX_UI_WIDGET(SELF->text_field));
  dx_ui_manager_leave_render(SELF->ui_manager);
  dx_ui_manager_set_root(SELF->ui_manager, NULL);

  return Core_Success;
}

Core_Result dx_overlay_set_visible(dx_overlay* SELF, bool visible) {
  SELF->visible = visible;
  return Core_Success;
}

Core_Result dx_overlay_get_visible(bool *RETURN, dx_overlay* SELF) {
  *RETURN = SELF->visible;
  return Core_Success;
}
