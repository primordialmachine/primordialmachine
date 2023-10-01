#if !defined(DX_UI_WIDGET_H_INCLUDED)
#define DX_UI_WIDGET_H_INCLUDED

#include "dx/core.h"
typedef struct dx_font_presenter dx_font_presenter;
typedef struct dx_rectangle_presenter dx_rectangle_presenter;
typedef struct dx_ui_manager dx_ui_manager;

DX_DECLARE_OBJECT_TYPE("dx.ui.widget",
                       dx_ui_widget,
                       dx_object);

static dx_ui_widget* DX_UI_WIDGET(void* p) {
  return (dx_ui_widget*)p;
}

struct dx_ui_widget {
  dx_object _parent;
  /// @brief A pointer to the UI manager.
  dx_ui_manager* manager;
  /// @brief A pointer to the parent widget or the null pointer.
  dx_ui_widget* parent;
};

static dx_ui_widget_dispatch* DX_UI_WIDGET_DISPATCH(void* p) {
  return (dx_ui_widget_dispatch*)p;
}

struct dx_ui_widget_dispatch {
  dx_object_dispatch _parent;
  dx_result (*set_relative_position)(dx_ui_widget*,DX_VEC2_F32 const*);
  dx_result (*get_relative_position)(DX_VEC2_F32*,dx_ui_widget*);
  dx_result (*set_relative_size)(dx_ui_widget*,DX_VEC2_F32 const*);
  dx_result (*get_relative_size)(DX_VEC2_F32*,dx_ui_widget*);
  dx_result (*get_absolute_position)(DX_VEC2_F32*,dx_ui_widget*);
  dx_result (*get_absolute_size)(DX_VEC2_F32*,dx_ui_widget*);
  dx_result(*render)(dx_ui_widget*,dx_f32,dx_f32,dx_f32,dx_f32);
};

/// @brief Construct this UI widget.
/// @param SELF A pointer to this UI widget.
/// @param manager A pointer to the UI manager.
/// @method-call
/// @remarks The UI widget retains a reference to the UI manager.
/// That is, the UI manager is not destroyed before the last UI widget or derived type object is destroyed.
dx_result dx_ui_widget_construct(dx_ui_widget* SELF, dx_ui_manager* manager);

/// @brief Set the relative position.
/// @param SELF A pointer to this widget.
/// @param position A pointer to a <code>DX_VEC2</code> variable.
/// @method-call 
static inline dx_result dx_ui_widget_set_relative_position(dx_ui_widget* SELF, DX_VEC2_F32 const* relative_position) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, set_relative_position, SELF, relative_position);
}

/// @brief Get the relative position.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @param SELF A pointer to this widget.
/// @succes <code>*RETURN</code> was assigned the relative position.
/// @method-call
static inline dx_result dx_ui_widget_get_relative_position(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_relative_position, RETURN, SELF);
}

/// @brief Set the relative size.
/// @param SELF A pointer to this widget.
/// @param size A pointer to a <code>DX_VEC2</code> variable.
/// @method-call
static inline dx_result dx_ui_widget_set_relative_size(dx_ui_widget* SELF, DX_VEC2_F32 const* relative_size) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, set_relative_size, SELF, relative_size);
}

/// @brief Get the relative size.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @param SELF A pointer to this widget.
/// @succes <code>*RETURN</code> was assigned the relative size.
/// @method-call
static inline dx_result dx_ui_widget_get_relative_size(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_relative_size, RETURN, SELF);
}

/// @brief Get the absolute position.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @param SELF A pointer to this widget.
/// @succes <code>*RETURN</code> was assigned the absolute position.
/// @method-call
static inline dx_result dx_ui_widget_get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_absolute_position, RETURN, SELF);
}

/// @brief Get the absolute size.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @param SELF A pointer to this widget.
/// @succes <code>*RETURN</code> was assigned the absolute size.
/// @method-call
static inline dx_result dx_ui_widget_get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_absolute_size, RETURN, SELF);
}


/// @brief Render this group.
/// @param SELF A pointer to this group.
/// @param canvas_width The width, in pixels, of the canvas.
/// @param canvas_height The height, in pixels, of the canvas.
/// @param rectangle_presenter A pointer to the rectangle presenter.
/// @method-call
static inline dx_result dx_ui_widget_render(dx_ui_widget* SELF, dx_f32 canvas_horizontal_size, dx_f32 canvas_vertical_size, dx_f32 dpi_horizontal, dx_f32 dpi_vertical) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, render, SELF, canvas_horizontal_size, canvas_vertical_size, dpi_horizontal, dpi_vertical)
}

#endif // DX_UI_WIDGET_H_INCLUDED
