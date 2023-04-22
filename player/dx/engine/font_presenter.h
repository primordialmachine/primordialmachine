#if !defined(DX_FONT_PRESENTER_H_INCLUDED)
#define DX_FONT_PRESENTER_H_INCLUDED

#include "dx/font_manager.h"
#include "dx/val/context.h"
#include "dx/val/buffer.h"
#include "dx/val/material.h"
#include "dx/val/viewer.h"
#include "dx/engine/rectangle_presenter.h"

DX_DECLARE_OBJECT_TYPE("dx.font_presenter",
                       dx_font_presenter,
                       dx_object);

static inline dx_font_presenter* DX_FONT_PRESENTER(void* p) {
  return (dx_font_presenter*)p;
}

struct dx_font_presenter {
  dx_object _parent;
  // the rectangle presenter
  dx_rectangle_presenter* rectangle_presenter;
  // the font manager
  dx_font_manager* font_manager;
  // the context to render the font
  dx_val_context* val_context;
  /// the material to render the font
  dx_val_material* val_material;
  // the constant binding to render the font
  dx_val_cbinding* val_cbinding;
  // the buffer to render the font
  dx_val_buffer* val_buffer;
  // the command list to render the font
  dx_val_command_list* val_command_list;
  struct {
    // the program to render the text
    dx_val_program* val_program;
    // the variable binding to render the text
    dx_val_vbinding* val_vbinding;
  } text;
  struct {
    // the program to render the text bounds
    dx_val_program* val_program;
    // the variable binding to render the text bounds7
    dx_val_vbinding* val_vbinding;
  } text_bounds;
};

static inline dx_font_presenter_dispatch* DX_FONT_RENDERER_DISPATCH(void* p) {
  return (dx_font_presenter_dispatch*)p;
}

struct dx_font_presenter_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_font_presenter_construct(dx_font_presenter* SELf, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context);

dx_result dx_font_presenter_create(dx_font_presenter** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context);

/// replace an unknown symbol by a placeholder symbol
#define dx_font_presenter_unknown_symbol_policy_placeholder (0)
/// raise an error when encountering an unknown symbol
#define dx_font_presenter_unknown_symbol_policy_error (1)

/// visualize the bounds of a glyph above and below the base.
#define dx_font_presenter_presentation_policy_ascender_descener (1)
/// print characters
#define dx_font_presenter_presentation_policy_glyph (4)

/// @brief 
/// @param SELF A pointer to this font presenter.
/// @param left The position along the x-axis. 
/// @param bottom The position along the y-axis.
/// @param string
/// A pointer to the string to be rendered.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param font The font to be used.
/// @default-runtime-calling-convention
dx_result dx_font_presenter_render_line_string(dx_font_presenter* SELF, dx_f32 left, dx_f32 bottom, dx_string* string, DX_RGBA_F32 const* text_color, dx_font* font);

/// @brief 
/// @param SELF A pointer to this font presenter.
/// @param position The left/bottom position of the baseline.
/// @param string_iterator The string iterator.
/// A pointer to the string iterator of the string to be rendered.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param font The font to render the text with.
/// @default-runtime-calling-convention
dx_result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF, DX_VEC2 const* position, dx_string_iterator* string_iterator, DX_RGBA_F32 const* text_color, dx_font* font);

#endif // DX_FONT_PRESENTER_H_INCLUDED
