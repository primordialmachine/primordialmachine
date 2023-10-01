#if !defined(DX_FONT_PRESENTER_H_INCLUDED)
#define DX_FONT_PRESENTER_H_INCLUDED

#include "dx/font_manager.h"
#include "dx/val/context.h"
#include "dx/val/buffer.h"
#include "dx/val/material.h"
#include "dx/val/viewer.h"
#include "dx/rectangle_presenter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Base of a glyph atlas for texture fonts.
/// A glyph atlas returns for a font and Unicode codepoint
/// - a texture containing the glyph as a subtexture and
/// - the texture coordinates for the subtexture.
DX_DECLARE_OBJECT_TYPE("dx.glyph_atlas",
                       dx_glyph_atlas,
                       dx_object);

static inline dx_glyph_atlas* DX_GLYPH_ATLAS_PRESENTER(void* p) {
  return (dx_glyph_atlas*)p;
}

struct dx_glyph_atlas {
  dx_object _parent;
  // the rectangle presenter
  dx_rectangle_presenter* rectangle_presenter;
  // the font manager
  dx_font_manager* font_manager;
  // the context to render the font
  dx_val_context* val_context;
};

static inline dx_glyph_atlas_dispatch* DX_GLYPH_ATLAS_DISPATCH(void* p) {
  return (dx_glyph_atlas_dispatch*)p;
}

struct dx_glyph_atlas_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this glyph atlas.
/// @param SELf A pointer to this glyph atlas.
/// @param font_manager A pointer to the font manager.
/// @param rectangle_presenter A pointer to the rectangle presenter.
/// @param val_context A pointer to the VAL context.
dx_result dx_glyph_atlas_construct(dx_glyph_atlas* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context);

dx_result dx_glyph_atlas_create(dx_glyph_atlas** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context);

dx_result dx_glyph_atlas_get_texture(dx_glyph_atlas* SELF, dx_font_glyph* glyph, dx_val_texture** val_texture, DX_RECT2_F32 *texture_coordinates);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.font_presenter",
                       dx_font_presenter,
                       dx_presenter);

static inline dx_font_presenter* DX_FONT_PRESENTER(void* p) {
  return (dx_font_presenter*)p;
}

struct dx_font_presenter {
  dx_presenter _parent;
  // the rectangle presenter
  dx_rectangle_presenter* rectangle_presenter;
  // the font manager
  dx_font_manager* font_manager;
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
};

static inline dx_font_presenter_dispatch* DX_FONT_RENDERER_DISPATCH(void* p) {
  return (dx_font_presenter_dispatch*)p;
}

struct dx_font_presenter_dispatch {
  dx_presenter_dispatch _parent;
};

dx_result dx_font_presenter_construct(dx_font_presenter* SELf, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter);

dx_result dx_font_presenter_create(dx_font_presenter** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter);

/// replace an unknown symbol by a placeholder symbol
#define dx_font_presenter_unknown_symbol_policy_placeholder (0)
/// raise an error when encountering an unknown symbol
#define dx_font_presenter_unknown_symbol_policy_error (1)

/// visualize the bounds of a glyph above and below the base.
#define dx_font_presenter_presentation_policy_ascender_descender (1)
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
/// @method-call
dx_result dx_font_presenter_render_line_string(dx_font_presenter* SELF, dx_f32 left, dx_f32 bottom, dx_string* string, DX_RGBA_F32 const* text_color, dx_font* font,
                                               uint8_t presentation_policy, uint8_t unknown_symbol_policy);

/// @brief 
/// @param SELF A pointer to this font presenter.
/// @param position The left/bottom position of the baseline.
/// @param string_iterator The string iterator.
/// A pointer to the string iterator of the string to be rendered.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param font The font to render the text with.
/// @method-call
dx_result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF, DX_VEC2_F32 const* position, dx_string_iterator* string_iterator, DX_RGBA_F32 const* text_color, dx_font* font,
                                                        uint8_t presentation_policy, uint8_t unknown_symbol_policy);

#endif // DX_FONT_PRESENTER_H_INCLUDED
