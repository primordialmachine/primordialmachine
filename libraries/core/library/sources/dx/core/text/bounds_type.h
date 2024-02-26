#if !defined(DX_TEXT_BOUNDS_TYPE_H_INCLUDED)
#define DX_TEXT_BOUNDS_TYPE_H_INCLUDED

#include "dx/core/Object.h"

/// @brief Enumeration of text bounds types.
/// The geometry of text can be measured either in terms of the bounds of the particular text to be rendered - visual bounds, or as properties of the font and the characters to be rendered - logical bounds.
///
/// Visual bounds are more useful for positioning text as graphics, and for obtaining tight enclosing bounds around the text.
/// Logical bounds are important for laying out text relative to other text and other components, particularly those which also contain text.
/// The logical bounds are not specific to the text being rendered, and so will report heights which account for the potential ascent and descent of text using the font at its specified size.
/// Also leading and trailing spaces are part of the logical advance width of the text. 
Core_declareEnumerationType("dx.text.bounds_type",
                            dx_text_bounds_type);

enum dx_text_bounds_type {

  /// @brief The visual bounds type.
  dx_text_bounds_type_visual,

  /// @brief The logical bounds type.
  dx_text_bounds_type_logical,

}; // dx_text_bounds_type

#endif // DX_TEXT_BOUNDS_TYPE_H_INCLUDED
