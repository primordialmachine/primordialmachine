#if !defined(DX_CORE_TEXT_ANCHOR_H_INCLUDED)
#define DX_CORE_TEXT_ANCHOR_H_INCLUDED

#include "dx/core/Object.h"

/// @brief Enumeration of vertical text anchors.
/// A text anchor determines the relation of a reference point and the bounding rectangle of the text.
Core_declareEnumerationType("dx.text.anchor_vertical",
                            dx_text_anchor_vertical);

enum dx_text_anchor_vertical {

  /// @brief The reference point is on the top edge of the bounding box of the text.
  dx_text_anchor_vertical_top,

  /// @brief The reference point is on the bottom edge of the bounding box of the text.
  dx_text_anchor_vertical_bottom,

  /// @brief The reference point is one the (vertical) center of the bounding rectangle.
  dx_text_anchor_vertical_center,

  /// @brief The reference point is one the baseline of the first line of the text.
  dx_text_anchor_vertical_baseline,

}; // dx_text_anchor_vertical

/// @brief Enumeration of horiziontal text anchors.
/// A text anchor determines the relation of a reference point and the bounding rectangle of the text.
Core_declareEnumerationType("dx.text.anchor_horizontal",
                            dx_text_anchor_horizontal);

enum dx_text_anchor_horizontal {

  /// @brief The reference point is on the left edge of the bounding rectangle.
  dx_text_anchor_horizontal_left,

  /// @brief The reference point is on the right edge of the bounding rectangle.
  dx_text_anchor_horizontal_right,

  /// @brief The reference point is on the (horizontal) center of the bounding rectangle.
  dx_text_anchor_horizontal_center,

}; // dx_text_anchor_horizontal

#endif // DX_CORE_TEXT_ANCHOR_H_INCLUDED
