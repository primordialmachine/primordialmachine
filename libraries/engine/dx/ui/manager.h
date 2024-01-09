#if !defined(DX_UI_MANAGER_H_INCLUDED)
#define DX_UI_MANAGER_H_INCLUDED

#include "dx/font_presenter.h"
#include "dx/rectangle_presenter.h"
typedef struct dx_ui_group dx_ui_group;
typedef struct dx_ui_widget dx_ui_widget;

Core_declareObjectType("dx.ui.manager",
                       dx_ui_manager,
                       Core_Object);

static inline dx_ui_manager* DX_UI_MANAGER(void* p) {
  return (dx_ui_manager*)p;
}

struct dx_ui_manager {
  Core_Object _parent;

  /// @brief Map from type names (e.g., "Text" or "Group"/Core_String) to type handlers (dx_ui_type_handler). 
  Core_InlinePointerHashmap type_handlers;

  dx_ui_widget* root;
  
  DX_VEC2_F32 dpi;
  DX_VEC2_F32 resolution;

  /// @brief The command list for setting the viewport.
  dx_val_command_list* command_list;

  dx_font_presenter* font_presenter;
  dx_rectangle_presenter* rectangle_presenter;
};

static inline dx_ui_manager_Dispatch* DX_UI_MANAGER_DISPATCH(void* p) {
  return (dx_ui_manager_Dispatch*)p;
}

struct dx_ui_manager_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_ui_manager_construct(dx_ui_manager* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

Core_Result dx_ui_manager_create(dx_ui_manager** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

/// @brief Set the canvas DPI.
/// @param dpi A pointer to a <code>DX_VEC2</code> variable.
/// @method{dx_ui_manager}
Core_Result dx_ui_manager_set_canvas_dpi(dx_ui_manager* SELF, DX_VEC2_F32 const* dpi);

/// @brief Set the canvas resolution.
/// @param resolution A pointer to a <code>DX_VEC2</code> variable.
/// @method{dx_ui_manager}
Core_Result dx_ui_manager_set_canvas_resolution(dx_ui_manager* SELF, DX_VEC2_F32 const* resolution);

/// @brief Set the root.
/// @param root A pointer to the UI widget or the null pointer.
/// @method{dx_ui_manager}
Core_Result dx_ui_manager_set_root(dx_ui_manager* SELF, dx_ui_widget* root);

/// @brief Enter rendering phase.
/// @method{dx_ui_manager}
Core_Result dx_ui_manager_enter_render(dx_ui_manager* SELF);

/// @brief Leave rendering phase.
/// @method{dx_ui_manager}
Core_Result dx_ui_manager_leave_render(dx_ui_manager* SELF);

Core_Result dx_ui_manager_load(dx_ui_widget** RETURN, dx_ui_manager* SELF, Core_String* path);

#endif // DX_UI_MANAGER_H_INCLUDED
