#if !defined(DX_ENGINE_OVERLAY_H_INCLUDED)
#define DX_ENGINE_OVERLAY_H_INCLUDED

#include "dx/font_presenter.h"
#include "dx/rectangle_presenter.h"
#include "dx/ui/text_field.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.overlay",
                       dx_overlay,
                       dx_object);

static inline dx_overlay* DX_OVERLAY(void* p) {
  return (dx_overlay*)p;
}

struct dx_overlay {
  dx_object _parent;
  /// @brief The UI manager.
  dx_ui_manager* ui_manager;
  /// @brief The UI text field for overlay text.
  dx_ui_text_field* text_field;
  /// If the overlay is visible or not.
  dx_bool visible;
};

static inline dx_overlay_dispatch* DX_OVERLAY_DISPATCH(void* p) {
  return (dx_overlay_dispatch*)p;
}

struct dx_overlay_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_overlay_construct(dx_overlay* SELf, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

dx_result dx_overlay_create(dx_overlay** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

/// @brief Render this overlay.
/// @param SELF A pointer to this overlay.
/// @method-call
dx_result dx_overlay_render(dx_overlay* SELF, dx_f32 delta_seconds, dx_i32 canvas_size_horizontal, dx_i32 canvas_size_vertical, dx_i32 dpi_horizontal, dx_i32 dpi_vertical);

dx_result dx_overlay_add_message(dx_overlay* SELF, dx_string* message);
dx_result dx_overlay_clear_messages(dx_overlay* SELF);

dx_result dx_overlay_set_visible(dx_overlay* SELF, bool visible);

dx_result dx_overlay_get_visible(bool *RETURN, dx_overlay* SELF);

#endif // DX_ENGINE_OVERLAY_H_INCLUDED
