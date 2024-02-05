#if !defined(DX_UI_SCROLLBAR_H_INCLUDED)
#define DX_UI_SCROLLBAR_H_INCLUDED

#include "dx/ui/widget.h"

Core_declareObjectType("dx.ui.scrollbar",
                       dx_ui_scrollbar,
                       dx_ui_widget);

static dx_ui_scrollbar* DX_UI_SCROLLBAR(void* p) {
  return (dx_ui_scrollbar*)p;
}

struct dx_ui_scrollbar {
  dx_ui_widget _parent;
  /// @brief The relative position of this UI text.
  DX_VEC2_F32 relative_position;
  /// @brief The relative size of this UI text.
  DX_VEC2_F32 relative_size;
  /// @brief The background color of this UI text.
  Core_InlineRgbaR32 background_color;
};

static dx_ui_scrollbar_Dispatch* DX_UI_SCROLLBAR_DISPATCH(void* p) {
  return (dx_ui_scrollbar_Dispatch*)p;
}

struct dx_ui_scrollbar_Dispatch {
  dx_ui_widget_Dispatch _parent;
};

Core_Result dx_ui_scrollbar_construct(dx_ui_scrollbar* SELF, dx_ui_manager* manager);

Core_Result dx_ui_scrollbar_create(dx_ui_scrollbar** RETURN, dx_ui_manager* manager);

#endif // DX_UI_SCROLLBAR_H_INCLUDED
