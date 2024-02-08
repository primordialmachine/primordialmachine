#include "dx/engine/overlay.h"

#include "dx/ui/manager.h"
// strlen
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// The overlay background color.
static Core_InlineRgbaR32 const OVERLAY_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
/// The text ui background color.
static Core_InlineRgbaR32 const TEXT_UI_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.2f };
/// The text ui text color.
static Core_InlineRgbaR32 const TEXT_UI_TEXT_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f };

Core_defineObjectType("dx.overlay",
                      dx_overlay,
                      Core_Object);

static void dx_overlay_destruct(dx_overlay* SELF) {
  CORE_UNREFERENCE(SELF->text_field);
  SELF->text_field = NULL;

  CORE_UNREFERENCE(SELF->ui_manager);
  SELF->ui_manager = NULL;
}

static void dx_overlay_constructDispatch(dx_overlay_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_overlay_construct(dx_overlay* SELF, dx_ui_manager* ui_manager) {
  DX_CONSTRUCT_PREFIX(dx_overlay);
  SELF->ui_manager = ui_manager;
  CORE_REFERENCE(ui_manager);
  SELF->visible = true;
  if (dx_ui_text_field_create(&SELF->text_field, SELF->ui_manager)) {
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  // we start at the top
  SELF->text_field->vertical_text_anchor = dx_text_anchor_vertical_top;
  if (dx_ui_text_field_set_background_color(SELF->text_field, &TEXT_UI_BACKGROUND_COLOR)) {
    CORE_UNREFERENCE(SELF->text_field);
    SELF->text_field = NULL;
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  if (dx_ui_text_field_set_text_color(SELF->text_field, &TEXT_UI_TEXT_COLOR)) {
    CORE_UNREFERENCE(SELF->text_field);
    SELF->text_field = NULL;
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_overlay_create(dx_overlay** RETURN, dx_ui_manager* ui_manager) {
  DX_CREATE_PREFIX(dx_overlay);
  if (dx_overlay_construct(SELF, ui_manager)) {
    CORE_UNREFERENCE(SELF);
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
    CORE_UNREFERENCE(text);
    text = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(text);
  text = NULL;
  return Core_Success;
}

static Core_Result on_layout(dx_overlay* SELF, Core_Real32 canvas_size_horizontal, Core_Real32 canvas_size_vertical) {
  // the insets along the top-border
  Core_Real32 insets_top = 16.f;
  // the insets along the bottom border
  Core_Real32 insets_bottom = 16.f;
  // the insets along the right border
  Core_Real32 insets_right = 16.f;

  // the width of the overlay's text field
  Core_Real32 overlay_width = canvas_size_horizontal * 0.25f - insets_right;
  // the height of the overlay's text field
  Core_Real32 overlay_height = canvas_size_vertical - insets_top - insets_bottom;
  // the position of the overlay along the x-axis
  Core_Real32 overlay_position_x = canvas_size_horizontal * 0.75f;
  // the position of the overlay along the y-axis
  // (positive y-axis goes from bottom to top)
  Core_Real32 overlay_position_y = insets_bottom;;

  Core_InlineVector2R32 temporary;

  dx_vec2_f32_set(&temporary, overlay_position_x, overlay_position_y);
  dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->text_field), &temporary);

  dx_vec2_f32_set(&temporary, overlay_width, overlay_height);
  dx_ui_widget_set_relative_size(DX_UI_WIDGET(SELF->text_field), &temporary);

  return Core_Success;
}

Core_Result dx_overlay_render(dx_overlay* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_size_horizontal, Core_Integer32 canvas_size_vertical, Core_Integer32 dpi_horizontal, Core_Integer32 dpi_vertical) {
  if (!SELF->visible) {
    return Core_Success;
  }
  if (on_layout(SELF, canvas_size_horizontal, canvas_size_vertical)) {
    return Core_Failure;
  }
  Core_InlineVector2R32 temporary;
  {
    dx_vec2_f32_set(&temporary, dpi_horizontal, dpi_vertical);
    dx_ui_manager_set_canvas_dpi(SELF->ui_manager, &temporary);

    dx_vec2_f32_set(&temporary, canvas_size_horizontal, canvas_size_vertical);
    dx_ui_manager_set_canvas_resolution(SELF->ui_manager, &temporary);

    dx_ui_manager_enter_render(SELF->ui_manager);
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
