#if !defined(DX_UI_TEXT_H_INCLUDED)
#define DX_UI_TEXT_H_INCLUDED

#include "dx/ui/widget.h"
#include "dx/document.h"
typedef struct dx_font dx_font;

DX_DECLARE_OBJECT_TYPE("dx.ui.text",
                       dx_ui_text,
                       dx_ui_widget);

static dx_ui_text* DX_UI_TEXT(void* p) {
  return (dx_ui_text*)p;
}

struct dx_ui_text {
  dx_ui_widget _parent;
  /// @brief The relative position of this UI text.
  DX_VEC2 relative_position;
  /// @brief The relative size of this UI text.
  DX_VEC2 relative_size;
  /// @brief The background color of this UI text.
  DX_RGBA_F32 background_color;
  /// @brief The text color of this UI text.
  DX_RGBA_F32 text_color;
  /// @brief The font of this UI text.
  dx_font* font;
  /// @brief The text document of this UI text.
  dx_text_document* text;
};

static dx_ui_text_dispatch* DX_UI_TEXT_DISPATCH(void* p) {
  return (dx_ui_text_dispatch*)p;
}

struct dx_ui_text_dispatch {
  dx_ui_widget_dispatch _parent;
};

dx_result dx_ui_text_construct(dx_ui_text* SELF, dx_ui_manager* manager);

dx_result dx_ui_text_create(dx_ui_text** RETURN, dx_ui_manager* manager);

/// @brief Set the background color.
/// @param SELF A pointer to this text.
/// @param background_color A pointer to a <code>DX_RGBA_F32</code> variable.
/// @method-call 
dx_result dx_ui_text_set_background_color(dx_ui_text* SELF, DX_RGBA_F32 const* background_color);

/// @brief Get the background color.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @param SELF A pointer to this text.
/// @succes <code>*RETURN</code> was assigned the background color.
/// @method-call 
dx_result dx_ui_text_get_background_color(DX_RGBA_F32* RETURN, dx_ui_text* SELF);

/// @brief Set the text color.
/// @param SELF A pointer to this text.
/// @param text_color A pointer to a <code>DX_RGBA_F32</code> variable.
/// @method-call 
dx_result dx_ui_text_set_text_color(dx_ui_text* SELF, DX_RGBA_F32 const* text_color);

/// @brief Get the text color.
/// @param RETURN A pointer to a DX_VEC2 variable.
/// @param SELF A pointer to this text.
/// @succes <code>*RETURN</code> was assigned the text color.
/// @method-call 
dx_result dx_ui_text_get_text_color(DX_RGBA_F32* RETURN, dx_ui_text* SELF);

/// @brief Set the text of this text.
/// @param SELF A pointer to this text.
/// @param text A pointer to the text to set.
/// @method-call
dx_result dx_ui_text_set_text(dx_ui_text* SELF, dx_string* text);

/// @brief Append to the text of this text.
/// @param SELF A pointer to this text.
/// @param text A pointer to the text to append.
/// @method-call
dx_result dx_ui_text_append_text(dx_ui_text* SELF, dx_string* text);

/// @brief Get the font of this text.
/// @param RETURN A pointer to a <code>dx_font*</code> variable.
/// @param SELF A pointer to this text.
/// @success <code>*RETURN</code> was assigned a pointer to the font object.
/// The caller acquired a reference to the font object.
/// @method-call
dx_result dx_ui_text_get_font(dx_font** RETURN, dx_ui_text* SELF);

#endif // DX_UI_TEXT_H_INCLUDED
