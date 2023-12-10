#if !defined(DX_UI_WIDGET_H_INCLUDED)
#define DX_UI_WIDGET_H_INCLUDED

#include "dx/core.h"
typedef struct dx_font_presenter dx_font_presenter;
typedef struct dx_rectangle_presenter dx_rectangle_presenter;
typedef struct dx_ui_manager dx_ui_manager;

DX_DECLARE_OBJECT_TYPE("dx.ui.widget",
                       dx_ui_widget,
                       Core_Object);

static dx_ui_widget* DX_UI_WIDGET(void* p) {
  return (dx_ui_widget*)p;
}

struct dx_ui_widget {
  Core_Object _parent;
  /// @brief A pointer to the name.
  Core_String* name;
  /// @brief A pointer to the UI manager.
  dx_ui_manager* manager;
  /// @brief A pointer to the parent widget or the null pointer.
  dx_ui_widget* parent;
};

static dx_ui_widget_dispatch* DX_UI_WIDGET_DISPATCH(void* p) {
  return (dx_ui_widget_dispatch*)p;
}

struct dx_ui_widget_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*set_name)(dx_ui_widget*, Core_String*);
  Core_Result(*get_name)(Core_String**, dx_ui_widget*);
  
  Core_Result(*set_relative_position)(dx_ui_widget*,DX_VEC2_F32 const*);
  Core_Result(*get_relative_position)(DX_VEC2_F32*,dx_ui_widget*);
  
  Core_Result(*set_relative_size)(dx_ui_widget*,DX_VEC2_F32 const*);
  Core_Result(*get_relative_size)(DX_VEC2_F32*,dx_ui_widget*);
  
  Core_Result(*set_relative_rectangle)(dx_ui_widget*, DX_RECT2_F32 const*);
  Core_Result(*get_relative_rectangle)(DX_RECT2_F32*, dx_ui_widget*);
  
  Core_Result(*get_absolute_position)(DX_VEC2_F32*,dx_ui_widget*);
  Core_Result(*get_absolute_size)(DX_VEC2_F32*,dx_ui_widget*);
  Core_Result(*get_absolute_rectangle)(DX_RECT2_F32*, dx_ui_widget*);

  Core_Result(*render)(dx_ui_widget*,Core_Real32,Core_Real32,Core_Real32,Core_Real32);
  Core_Result(*get_child_by_name)(dx_ui_widget**, dx_ui_widget*, Core_String*);
};

/// @param manager
/// A pointer to the UI manager.
/// @remarks
/// The UI widget retains a reference to the UI manager.
/// That is, the UI manager is not destroyed before the last UI widget or derived type object is destroyed.
/// @constructor{dx_ui_widget}
Core_Result dx_ui_widget_construct(dx_ui_widget* SELF, dx_ui_manager* manager);

/// @brief
/// Set the name of this widget.
/// @param name
/// A pointer to the name.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_set_name(dx_ui_widget* SELF, Core_String* name) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, set_name, SELF, name);
}

/// @brief
/// Get the name of this widget.
/// @param RETURN
/// A pointer to a <code>Core_String*</code> variable.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_name(Core_String** RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_name, RETURN, SELF);
}

/// @brief
/// Set the relative position.
/// @param position
/// A pointer to a <code>DX_VEC2_F32</code> variable.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_set_relative_position(dx_ui_widget* SELF, DX_VEC2_F32 const* relative_position) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, set_relative_position, SELF, relative_position);
}

/// @brief
/// Get the relative position.
/// @param RETURN
/// A pointer to a <code>DX_VEC2_F32</code> variable.
/// @success
/// <code>*RETURN</code> was assigned the relative position.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_relative_position(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_relative_position, RETURN, SELF);
}

/// @brief
/// Set the relative size.
/// @param size
/// A pointer to a <code>DX_VEC2_F32</code> variable.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_set_relative_size(dx_ui_widget* SELF, DX_VEC2_F32 const* relative_size) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, set_relative_size, SELF, relative_size);
}

/// @brief
/// Get the relative size.
/// @param RETURN
/// A pointer to a <code>DX_VEC2_F32</code> variable.
/// @success
/// <code>*RETURN</code> was assigned the relative size.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_relative_size(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_relative_size, RETURN, SELF);
}

/// @brief
/// Set the relative rectangle.
/// @param rectangle
/// A pointer to a <code>DX_RECT2_F32</code> variable.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_set_relative_rectangle(dx_ui_widget* SELF, DX_RECT2_F32 const* rectangle) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, set_relative_rectangle, SELF, rectangle);
}

/// @brief 
/// Get the relative rectangle.
/// @param RETURN
/// A pointer to a <code>_DX_RECT2_F32</code> variable.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_relative_rectangle(DX_RECT2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_relative_rectangle, RETURN, SELF);
}

/// @brief
/// Get the absolute position.
/// @param RETURN
/// A pointer to a <code>DX_VEC2_F32</code> variable.
/// @success
/// <code>*RETURN</code> was assigned the absolute position.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_absolute_position, RETURN, SELF);
}

/// @brief
/// Get the absolute size.
/// @param RETURN
/// A pointer to a DX_VEC2_F32 variable.
/// @success
/// <code>*RETURN</code> was assigned the absolute size.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_absolute_size, RETURN, SELF);
}

/// @brief
/// Get the absolute rectangle.
/// @param RETURN
/// A pointer to a <code>DX_RECT2_F32</code> variable.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_get_absolute_rectangle(DX_RECT2_F32* RETURN, dx_ui_widget* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_absolute_rectangle, RETURN, SELF);
}

/// @brief
/// Render this widget.
/// @param canvas_width
/// The width, in pixels, of the canvas.
/// @param canvas_height
/// The height, in pixels, of the canvas.
/// @param rectangle_presenter
/// A pointer to the rectangle presenter.
/// @method{dx_ui_widget}
static inline Core_Result dx_ui_widget_render(dx_ui_widget* SELF, Core_Real32 canvas_horizontal_size, Core_Real32 canvas_vertical_size, Core_Real32 dpi_horizontal, Core_Real32 dpi_vertical) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, render, SELF, canvas_horizontal_size, canvas_vertical_size, dpi_horizontal, dpi_vertical)
}

/// @brief
/// Get a child by its name.
/// @param RETURN
/// A pointer to a <code>dx_ui_widget*</code> variable.
/// @param name
/// A pointer to the name string.
/// @success
/// <code>*RETURN</code> received a pointer to the child.
/// The caller acquired a reference to that child.
/// @method{dx_ui_widget}
/// @error
/// Core_Error_NotFound if the child was not found.
/// @remarks
/// If a child has no name, then it cannot be get by its name.
static inline Core_Result dx_ui_widget_get_child_by_name(dx_ui_widget** RETURN, dx_ui_widget* SELF, Core_String* name) {
  DX_OBJECT_VIRTUALCALL(dx_ui_widget, get_child_by_name, RETURN, SELF, name);
}

#endif // DX_UI_WIDGET_H_INCLUDED
