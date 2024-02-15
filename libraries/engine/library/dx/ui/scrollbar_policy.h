#if !defined(DX_UI_SCROLLBAR_POLICY_H_INCLUDED)
#define DX_UI_SCROLLBAR_POLICY_H_INCLUDED

#include "dx/core.h"

/// @brief A policy for scrollbars.
/// Denotes under which condition a scrollbar is show.
Core_declareEnumerationType("dx.ui.scrollbar_policy",
                            dx_ui_scrollbar_policy);

enum dx_scrollbar_policy {

  /// @brief Always show the scrollbar.
  dx_ui_scrollbar_policy_always,

  /// @brief Never show the scrollbar.
  dx_ui_scrollbar_policy_never,

  /// @brief Show the scrollbar if required.
  dx_ui_scrollbar_policy_auto,

};

#endif // DX_UI_SCROLLBAR_POLICY_H_INCLUDED
