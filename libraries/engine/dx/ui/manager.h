#if !defined(DX_UI_MANAGER_H_INCLUDED)
#define DX_UI_MANAGER_H_INCLUDED

#include "dx/font_presenter.h"
#include "dx/rectangle_presenter.h"
typedef struct dx_ui_group dx_ui_group;
typedef struct dx_ui_widget dx_ui_widget;

DX_DECLARE_OBJECT_TYPE("dx.ui.manager",
                       dx_ui_manager,
                       dx_object);

static inline dx_ui_manager* DX_UI_MANAGER(void* p) {
  return (dx_ui_manager*)p;
}

struct dx_ui_manager {
  dx_object _parent;
  dx_ui_widget* root;
  DX_VEC2 dpi;
  DX_VEC2 resolution;

  /// @brief The command list for setting the viewport.
  dx_val_command_list* command_list;

  dx_font_presenter* font_presenter;
  dx_rectangle_presenter* rectangle_presenter;
};

static inline dx_ui_manager_dispatch* DX_UI_MANAGER_DISPATCH(void* p) {
  return (dx_ui_manager_dispatch*)p;
}

struct dx_ui_manager_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_ui_manager_construct(dx_ui_manager* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

dx_result dx_ui_manager_create(dx_ui_manager** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

/// @brief Set the canvas DPI.
/// @param SELF A pointer to this UI manager.
/// @param dpi A pointer to a <code>DX_VEC2</code> variable.
/// @method-call
dx_result dx_ui_manager_set_canvas_dpi(dx_ui_manager* SELF, DX_VEC2 const* dpi);

/// @brief Set the canvas resolution.
/// @param SELF A pointer to this UI manager.
/// @param resolution A pointer to a <code>DX_VEC2</code> variable.
/// @method-call
dx_result dx_ui_manager_set_canvas_resolution(dx_ui_manager* SELF, DX_VEC2 const* resolution);

/// @brief Set the root.
/// @param SELF A pointer to this UI manager.
/// @param root A pointer to the UI widget or the null pointer.
/// @method-call
dx_result dx_ui_manager_set_root(dx_ui_manager* SELF, dx_ui_widget* root);

/// @brief Enter rendering phase.
/// @param SELF A pointer to this UI manager.
/// @method-call
dx_result dx_ui_manager_enter_render(dx_ui_manager* SELF);

/// @brief Leave rendering phase.
/// @param SELF A pointer to this UI manager.
/// @method-call
dx_result dx_ui_manager_leave_render(dx_ui_manager* SELF);

#endif // DX_UI_MANAGER_H_INCLUDED
