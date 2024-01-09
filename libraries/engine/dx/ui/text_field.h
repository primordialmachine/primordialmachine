#if !defined(DX_UI_TEXT_FIELD_H_INCLUDED)
#define DX_UI_TEXT_FIELD_H_INCLUDED

#include "dx/ui/widget.h"
#include "dx/ui/scrollbar_policy.h"
#include "dx/document.h"
typedef struct dx_font dx_font;

Core_declareObjectType("dx.ui.text_field",
                       dx_ui_text_field,
                       dx_ui_widget);

static dx_ui_text_field* DX_UI_TEXT_FIELD(void* p) {
  return (dx_ui_text_field*)p;
}

struct dx_ui_text_field {
  dx_ui_widget _parent;
  /// @brief The relative position of this UI text.
  DX_VEC2_F32 relative_position;
  /// @brief The relative size of this UI text.
  DX_VEC2_F32 relative_size;
  /// @brief The background color of this UI text.
  DX_RGBA_F32 background_color;

  /// @brief The bounds of the text.
  DX_RECT2_F32 text_bounds;
  /// @brief If the text bounds need to be recomputed.
  bool text_bounds_dirty;

  /// @brief The policy for showing the horizontal scrollbar.
  /// @default dx_ui_scrollbar_policy_auto
  dx_ui_scrollbar_policy horizontal_scrollbar_policy;
  /// @brief The policy for showing the vertical scrollbar.
  /// @default dx_ui_scrollbar_policy_auto
  dx_ui_scrollbar_policy vertical_scrollbar_policy;

  /// The text is in general anchored to the (horiontal) left border (for single line text controls).
  dx_text_anchor_horizontal horizontal_text_anchor;
  /// The text is in general anchored to the (vertical) center (for single line text controls).
  dx_text_anchor_vertical vertical_text_anchor;

  /// @brief The text color of this UI text.
  DX_RGBA_F32 text_color;
  /// @brief The font of this UI text.
  dx_font* font;
  /// @brief The text document of this UI text.
  dx_text_document* text;
};

static dx_ui_text_field_Dispatch* DX_UI_TEXT_FIELD_DISPATCH(void* p) {
  return (dx_ui_text_field_Dispatch*)p;
}

struct dx_ui_text_field_Dispatch {
  dx_ui_widget_Dispatch _parent;
};

/// @constructor{dx_ui_text_field}
Core_Result dx_ui_text_field_construct(dx_ui_text_field* SELF, dx_ui_manager* manager);

/// @create-operator{dx_ui_text_field}
Core_Result dx_ui_text_field_create(dx_ui_text_field** RETURN, dx_ui_manager* manager);

/// @{

/// @brief Get the background color.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @success <code>*RETURN</code> was assigned the background color.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_get_background_color(DX_RGBA_F32* RETURN, dx_ui_text_field* SELF);

/// @brief Set the background color.
/// @param background_color A pointer to a <code>DX_RGBA_F32</code> variable.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_set_background_color(dx_ui_text_field* SELF, DX_RGBA_F32 const* background_color);

/// @}

/// @{

/// @brief Get the text color.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @success <code>*RETURN</code> was assigned the text color.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_get_text_color(DX_RGBA_F32* RETURN, dx_ui_text_field* SELF);

/// @brief Set the text color.
/// @param text_color A pointer to a <code>DX_RGBA_F32</code> variable.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_set_text_color(dx_ui_text_field* SELF, DX_RGBA_F32 const* text_color);

/// @}

/// @{

/// @brief Get the font.
/// @param RETURN A pointer to a <code>dx_font*</code> variable.
/// @success <code>*RETURN</code> was assigned a pointer to the font object.
/// The caller acquired a reference to the font object.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_get_font(dx_font** RETURN, dx_ui_text_field* SELF);

/// @brief Set the font.
/// @param font A pointer to the font..
/// @success The font was set.
/// This text field acquired a reference to the font object.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_set_font(dx_ui_text_field* SELF, dx_font* font);

/// @}

/// @{ 

/// @brief Get the horizontal scrollbar policy.
/// @param RETURN A pointer to a <code>dx_ui_scrollbar_policy</code> variable.
/// @success <code>*RETURN</code> was assigned the horizontal scrollbar policy.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_get_horizontal_scrollbar_policy(dx_ui_scrollbar_policy* RETURN, dx_ui_text_field* SELF);

/// @brief Set the horizontal scrollbar policy.
/// @param param scrollbar_policy The scrollbar policy.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_set_horizontal_scrollbar_policy(dx_ui_text_field* SELF, dx_ui_scrollbar_policy scrollbar_policy);

/// @}

/// @{

/// @brief Get the vertical scrollbar policy.
/// @param RETURN A pointer to a <code>dx_ui_scrollbar_policy</code> variable.
/// @success <code>*RETURN</code> was assigned the vertical scrollbar policy.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_get_vertical_scrollbar_policy(dx_ui_scrollbar_policy* RETURN, dx_ui_text_field* SELF);

/// @brief Set the vertical scrollbar policy.
/// @param param scrollbar_policy The scrollbar policy.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_set_vertical_scrollbar_policy(dx_ui_text_field* SELF, dx_ui_scrollbar_policy scrollbar_policy);

/// @}

/// @{

/// @brief Set the text.
/// @param text A pointer to the text to set.
/// @success
/// A copy of the text was made.
/// The text object is no longer required by this text field object.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_set_text(dx_ui_text_field* SELF, Core_String* text);

/// @brief Get the text.
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @success <code>*RETURN</code> was assigned a pointer to the font object.
/// The caller acquired a reference to the font object.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_get_text(Core_String** RETURN, dx_ui_text_field* SELF);

/// @}

/// @brief Append to the text.
/// @param text A pointer to the text to append.
/// @method{dx_ui_text_field}
Core_Result dx_ui_text_field_append_text(dx_ui_text_field* SELF, Core_String* text);

#endif // DX_UI_TEXT_FIELD_H_INCLUDED
