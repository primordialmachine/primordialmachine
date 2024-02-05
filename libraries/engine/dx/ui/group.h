#if !defined(DX_UI_GROUP_H_INCLUDED)
#define DX_UI_GROUP_H_INCLUDED

#include "dx/ui/widget.h"

Core_declareObjectType("dx.ui.group",
                       dx_ui_group,
                       dx_ui_widget);

static dx_ui_group* DX_UI_GROUP(void* p) {
  return (dx_ui_group*)p;
}

struct dx_ui_group {
  dx_ui_widget _parent;
  /// @brief The relative position of this UI text.
  DX_VEC2_F32 relative_position;
  /// @brief The relative size of this UI text.
  DX_VEC2_F32 relative_size;
  /// @brief The background color of this UI text.
  Core_InlineRgbaR32 background_color;

  dx_object_array* children;
};

static dx_ui_group_Dispatch* DX_UI_GROUP_DISPATCH(void* p) {
  return (dx_ui_group_Dispatch*)p;
}

struct dx_ui_group_Dispatch {
  dx_ui_widget_Dispatch _parent;
};

/// @constructor{dx_ui_group}
Core_Result dx_ui_group_construct(dx_ui_group* SELF, dx_ui_manager* manager);

Core_Result dx_ui_group_create(dx_ui_group** RETURN, dx_ui_manager* manager);

/// @brief Set the background color.
/// @param background_color A pointer to a <code>Core_InlineRgbaR32</code> variable.
/// @method{dx_ui_group}
Core_Result dx_ui_group_set_background_color(dx_ui_group* SELF, Core_InlineRgbaR32 const* background_color);

/// @brief Get the background color.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @success <code>*RETURN</code> was assigned the background color.
/// @method{dx_ui_group}
Core_Result dx_ui_group_get_background_color(Core_InlineRgbaR32* RETURN, dx_ui_group* SELF);

/// @brief Append a child to the list of children of this group.
/// @param child A pointer to the child.
/// @method{dx_ui_group}
Core_Result dx_ui_group_append_child(dx_ui_group* SELF, dx_ui_widget* child);

/// @brief Prepend a child to the list of children of this group.
/// @param child A pointer to the child.
/// @method{dx_ui_group}
Core_Result dx_ui_group_prepend_child(dx_ui_group* SELF, dx_ui_widget* child);

/// @brief Insert a child into the list of children of this group.
/// @param child A pointer to the child.
/// @param index The index at which to insert the child.
/// @error #DX_ERROR_INVALID_INDEX The index is greater than the size of the child list.
/// @error #Core_Error_ArgumentInvalid @a child is null.
/// @method{dx_ui_group}
Core_Result dx_ui_group_insert_child(dx_ui_group* SELF, dx_ui_widget* child, Core_Size index);

#endif // DX_UI_GROUP_H_INCLUDED
