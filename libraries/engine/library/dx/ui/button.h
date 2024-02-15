#if !defined(DX_UI_BUTTON_H_INCLUDED)
#define DX_UI_BUTTON_H_INCLUDED

#include "dx/ui/widget.h"

Core_declareObjectType("dx.ui.button",
                       dx_ui_button,
                       dx_ui_widget);

static dx_ui_button* DX_UI_BUTTON(void* p) {
  return (dx_ui_button*)p;
}

struct dx_ui_button {
  dx_ui_widget _parent;
  /// @brief The relative position of this widget.
  Core_InlineVector2R32 relative_position;
  /// @brief The relative size of this widget.
  Core_InlineVector2R32 relative_size;
  /// @brief The background color of this widget.
  Core_InlineRgbaR32 background_color;
};

static dx_ui_button_Dispatch* DX_UI_BUTTON_DISPATCH(void* p) {
  return (dx_ui_button_Dispatch*)p;
}

struct dx_ui_button_Dispatch {
  dx_ui_widget_Dispatch _parent;
};

Core_Result dx_ui_button_construct(dx_ui_button* SELF, dx_ui_manager* manager);

Core_Result dx_ui_button_create(dx_ui_button** RETURN, dx_ui_manager* manager);

#endif // DX_UI_BUTTON_H_INCLUDED
