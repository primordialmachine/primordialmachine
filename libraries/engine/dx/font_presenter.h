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
Core_declareObjectType("dx.glyph_atlas",
                       dx_glyph_atlas,
                       Core_Object);

static inline dx_glyph_atlas* DX_GLYPH_ATLAS_PRESENTER(void* p) {
  return (dx_glyph_atlas*)p;
}

struct dx_glyph_atlas {
  Core_Object _parent;
  // the rectangle presenter
  dx_rectangle_presenter* rectangle_presenter;
  // the font manager
  dx_font_manager* font_manager;
  // the context to render the font
  dx_val_context* val_context;
};

static inline dx_glyph_atlas_Dispatch* DX_GLYPH_ATLAS_DISPATCH(void* p) {
  return (dx_glyph_atlas_Dispatch*)p;
}

struct dx_glyph_atlas_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Construct this glyph atlas.
/// @param SELf A pointer to this glyph atlas.
/// @param font_manager A pointer to the font manager.
/// @param rectangle_presenter A pointer to the rectangle presenter.
/// @param val_context A pointer to the VAL context.
Core_Result dx_glyph_atlas_construct(dx_glyph_atlas* SELF, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context);

Core_Result dx_glyph_atlas_create(dx_glyph_atlas** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter, dx_val_context* val_context);

Core_Result dx_glyph_atlas_get_texture(dx_glyph_atlas* SELF, dx_font_glyph* glyph, dx_val_texture** val_texture, DX_RECT2_F32 *texture_coordinates);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.font_presenter",
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

static inline dx_font_presenter_Dispatch* DX_FONT_RENDERER_DISPATCH(void* p) {
  return (dx_font_presenter_Dispatch*)p;
}

struct dx_font_presenter_Dispatch {
  dx_presenter_Dispatch _parent;
};

Core_Result dx_font_presenter_construct(dx_font_presenter* SELf, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter);

Core_Result dx_font_presenter_create(dx_font_presenter** RETURN, dx_font_manager* font_manager, dx_rectangle_presenter* rectangle_presenter);

/// @brief Enumeration of policies for not available glyphs.
typedef enum DX_GLYH_NOT_AVAILABLE_POLICY {

  /// @brief If a glyph is not available, raise an error.
  DX_GLYPH_NOT_AVAILABLE_POLICY_ERROR = 0,

  /// @brief If a glyph is not available, replace its code point by a placeholder code point.
  DX_GLYPH_NOT_AVAILABLE_POLICY_PLACEHOLDER = 1,

  /// @brief If a glyph is not available, skip its code point.
  DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP = 2,

} DX_GLYPH_NOT_AVAILABLE_POLICY;

/// @brief Enumeration of policies for not presentable code points.
typedef enum DX_CODE_POINT_NOT_PRESENTABLE_POLICY {

  /// @brief If a code point is not presentable, raise an error.
  DX_CODE_POINT_NOT_PRESENTABLE_POLICY_ERROR = 0,
  
  /// @brief If a code point is not presentable, reaplce that code point by a placeholder code point.
  DX_CODE_POINT_NOT_PRESENTABLE_POLICY_PLACEHOLDER = 1,
  
  /// @brief If a code point is not presentable, skip that code point.
  DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP = 2,

} DX_CODE_POINT_NOT_PRESENTABLE_POLICY;

/// @brief Opions for measuring text.
typedef struct DX_TEXT_MEASUREMENT_OPTIONS {

  /// @brief Policy for not available glyphs.
  DX_GLYPH_NOT_AVAILABLE_POLICY glyph_not_availabe_policy;

  /// @brief Policy for not presentable code points.
  DX_CODE_POINT_NOT_PRESENTABLE_POLICY code_point_not_presentable_policy;

  /// @brief The vertical anchor to be used.
  dx_text_anchor_vertical vertical_anchor;

  /// @brief The text bounds type to be used.
  dx_text_bounds_type text_bounds_type;

} DX_TEXT_MEASUREMENT_OPTIONS;

typedef struct DX_TEXT_PRESENTATION_OPTIONS {

  /// @brief Policy for not available glyphs.
  DX_GLYPH_NOT_AVAILABLE_POLICY glyph_not_availabe_policy;

  /// @brief Policy for not presentable code points.
  DX_CODE_POINT_NOT_PRESENTABLE_POLICY code_point_not_presentable_policy;

  /// @brief If a glyph's descender shall be presented.
  Core_Boolean present_glyph_descender;

  /// @brief If a glyph's ascender shall be presented.
  Core_Boolean present_glyph_ascender;

  /// @brief If the glyph shall be presented.
  Core_Boolean present_glyph;

  /// @brief The vertical anchor to be used.
  dx_text_anchor_vertical vertical_anchor;

} DX_TEXT_PRESENTATION_OPTIONS;

/// @brief 
/// @param position
/// The position at which to render the text.
/// This is the left/bottom position of the baseline of the first line.
/// @param string
/// A pointer to the string to be rendered.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param color The color to be used.
/// @param font The font to be used.
/// @param options A pointer to the options.
/// @method{dx_font_presenter}
Core_Result dx_font_presenter_render_line_string(dx_font_presenter* SELF,
                                               DX_VEC2_F32 const* position,
                                               Core_String* string,
                                               DX_RGBA_F32 const* text_color,
                                               dx_font* font,
                                               DX_TEXT_PRESENTATION_OPTIONS const* options);

/// @brief 
/// @param position
/// The position at which to measure the text.
/// This is the left/bottom position of the baseline of the first line.
/// @param string
/// A pointer to the string to measured.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param color The color to be used.
/// @param font The font to be used.
/// @param options A pointer to the options.
/// @method{dx_font_presenter}
Core_Result dx_font_presenter_measure_line_string(dx_font_presenter* SELF,
                                                DX_VEC2_F32 const* position,
                                                Core_String* string,
                                                dx_font* font,
                                                DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                                DX_RECT2_F32* bounds);

/// @brief 
/// @param position
/// The position at which to render the text.
/// This is the left/bottom position of the baseline of the text line.
/// @param string_iterator The string iterator.
/// A pointer to the string iterator of the string to be rendered.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param color The color to be used.
/// @param font The font to be used.
/// @param options A pointer to the options.
/// @method{dx_font_presenter}
Core_Result dx_font_presenter_render_line_string_iterator(dx_font_presenter* SELF,
                                                        DX_VEC2_F32 const* position,
                                                        dx_string_iterator* string_iterator,
                                                        DX_RGBA_F32 const* text_color,
                                                        dx_font* font,
                                                        DX_TEXT_PRESENTATION_OPTIONS const* options);

/// @brief 
/// @param position
/// The position at which to measure the text.
/// This is the left/bottom position of the baseline of the text line.
/// @param string_iterator The string iterator.
/// A pointer to the string iterator of the string to be rendered.
/// Newline codepoints are replaced by a placeholder symbol. Unprintable codepoints are replaced by a placeholder symbol.
/// @param color The color to be used.
/// @param font The font to be used.
/// @param options A pointer to the options.
/// @method{dx_font_presenter}
Core_Result dx_font_presenter_measure_line_string_iterator(dx_font_presenter* SELF,
                                                         DX_VEC2_F32 const* position,
                                                         dx_string_iterator* string_iterator,
                                                         dx_font* font,
                                                         DX_TEXT_MEASUREMENT_OPTIONS const* options,
                                                         DX_RECT2_F32* bounds);

#endif // DX_FONT_PRESENTER_H_INCLUDED
