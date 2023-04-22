#if !defined(DX_ENGINE_OVERLAY_H_INCLUDED)
#define DX_ENGINE_OVERLAY_H_INCLUDED

#include "dx/engine/font_presenter.h"
#include "dx/engine/rectangle_presenter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.overlay",
                       dx_overlay,
                       dx_object);

static inline dx_overlay* DX_OVERLAY(void* p) {
  return (dx_overlay*)p;
}

struct dx_overlay {
  dx_object _parent;
  /// The font presenter.
  dx_font_presenter* font_presenter;
  /// The default font of the console.
  dx_font* font;
  /// @brief A list of dx_string objects. Each representing a line.
  /// '\n', '\r' as well as any non-printable characters are ignored.
  dx_inline_pointer_array lines;
  dx_val_context* val_context;
  dx_val_command_list* command_list;

  /// If the overlay is visible or not.
  dx_bool visible;
};

static inline dx_overlay_dispatch* DX_OVERLAY_DISPATCH(void* p) {
  return (dx_overlay_dispatch*)p;
}

struct dx_overlay_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_overlay_construct(dx_overlay* SELf, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* context);

dx_result dx_overlay_create(dx_overlay** RETURN, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* context);

/// @brief Render this overlay.
/// @param SELF A pointer to this overlay.
/// @default-runtime-calling-convention
dx_result dx_overlay_render(dx_overlay* SELF, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height);

dx_result dx_overlay_add_message(dx_overlay* SELF, dx_string* message);
dx_result dx_overlay_clear_messages(dx_overlay* SELF);

dx_result dx_overlay_set_visible(dx_overlay* SELF, bool visible);

dx_result dx_overlay_get_visible(bool *RETURN, dx_overlay* SELF);

#endif // DX_ENGINE_OVERLAY_H_INCLUDED
