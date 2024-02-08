#if !defined(DX_UI_SCROLLBAR_H_INCLUDED)
#define DX_UI_SCROLLBAR_H_INCLUDED

#include "dx/ui/widget.h"

Core_declareEnumerationType("dx.ui.scrollbar_direction",
                            dx_ui_scrollbar_direction);

enum dx_ui_scrollbar_direction {
  dx_ui_scrollbar_direction_vertical,
  dx_ui_scrollbar_direction_horizontal,
};

Core_declareObjectType("dx.ui.scrollbar",
                       dx_ui_scrollbar,
                       dx_ui_widget);

static dx_ui_scrollbar* DX_UI_SCROLLBAR(void* p) {
  return (dx_ui_scrollbar*)p;
}

struct dx_ui_scrollbar {
  dx_ui_widget _parent;
  /// @brief The relative position of this widget.
  Core_InlineVector2R32 relative_position;
  /// @brief The relative size of this widget.
  Core_InlineVector2R32 relative_size;
  /// @brief The background color of this widget.
  Core_InlineRgbaR32 background_color;
  /// @brief The scrollbar direction.
  /// @default dx_ui_scrollbar_direction_horizontal
  dx_ui_scrollbar_direction scrollbar_direction;
};

static dx_ui_scrollbar_Dispatch* DX_UI_SCROLLBAR_DISPATCH(void* p) {
  return (dx_ui_scrollbar_Dispatch*)p;
}

struct dx_ui_scrollbar_Dispatch {
  dx_ui_widget_Dispatch _parent;
};

Core_Result dx_ui_scrollbar_construct(dx_ui_scrollbar* SELF, dx_ui_manager* manager);

Core_Result dx_ui_scrollbar_create(dx_ui_scrollbar** RETURN, dx_ui_manager* manager);

Core_Result dx_ui_scrollbar_get_scrollbar_direction(dx_ui_scrollbar_direction* RETURN, dx_ui_scrollbar* SELF);

Core_Result dx_ui_scrollbar_set_scrollbar_direction(dx_ui_scrollbar* SELF, dx_ui_scrollbar_direction scrollbar_direction);

/// @{

/// @brief Get the background color.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @success <code>*RETURN</code> was assigned the background color.
/// @method{dx_ui_scrollbar}
Core_Result dx_ui_scrollbar_get_background_color(Core_InlineRgbaR32* RETURN, dx_ui_scrollbar* SELF);

/// @brief Set the background color.
/// @param background_color A pointer to a <code>Core_InlineRgbaR32</code> variable.
/// @method{dx_ui_scrollbar}
Core_Result dx_ui_scrollbar_set_background_color(dx_ui_scrollbar* SELF, Core_InlineRgbaR32 const* background_color);

/// @}

#endif // DX_UI_SCROLLBAR_H_INCLUDED
