#if !defined(DX_ENGINE_OVERLAY_H_INCLUDED)
#define DX_ENGINE_OVERLAY_H_INCLUDED

#include "dx/font_presenter.h"
#include "dx/rectangle_presenter.h"
#include "dx/ui/text_field.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.overlay",
                       dx_overlay,
                       Core_Object);

static inline dx_overlay* DX_OVERLAY(void* p) {
  return (dx_overlay*)p;
}

struct dx_overlay {
  Core_Object _parent;
  /// @brief The UI manager.
  dx_ui_manager* ui_manager;
  /// @brief The UI text field for overlay text.
  dx_ui_text_field* text_field;
  /// If the overlay is visible or not.
  Core_Boolean visible;
};

static inline dx_overlay_Dispatch* DX_OVERLAY_DISPATCH(void* p) {
  return (dx_overlay_Dispatch*)p;
}

struct dx_overlay_Dispatch {
  Core_Object_Dispatch _parent;
};
 
/// @param ui_manager A pointer to the UI manager.
/// @constructor{dx_overlay}
Core_Result dx_overlay_construct(dx_overlay* SELf, dx_ui_manager* ui_manager);

/// @param ui_manager A pointer to the UI manager.
/// @create-operator{dx_overlay} 
Core_Result dx_overlay_create(dx_overlay** RETURN, dx_ui_manager* ui_manager);

/// @brief Render this overlay.
/// @method{dx_overlay}
Core_Result dx_overlay_render(dx_overlay* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_size_horizontal, Core_Integer32 canvas_size_vertical, Core_Integer32 dpi_horizontal, Core_Integer32 dpi_vertical);

Core_Result dx_overlay_add_message(dx_overlay* SELF, Core_String* message);
Core_Result dx_overlay_clear_messages(dx_overlay* SELF);

Core_Result dx_overlay_set_visible(dx_overlay* SELF, bool visible);

Core_Result dx_overlay_get_visible(bool *RETURN, dx_overlay* SELF);

#endif // DX_ENGINE_OVERLAY_H_INCLUDED
