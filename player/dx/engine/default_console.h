#if !defined(DX_DEFAULT_CONSOLE_H_INCLUDED)
#define DX_DEFAULT_CONSOLE_H_INCLUDED

#include "dx/document.h"
#include "dx/console.h"
#include "dx/val/context.h"
#include "dx/ui/text.h"
#include "dx/ui/group.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.default_console",
                       dx_default_console,
                       dx_console);

static inline dx_default_console* DX_DEFAULT_CONSOLE(void* p) {
  return (dx_default_console*)p;
}

struct dx_default_console {
  dx_console _parent;
  /// @brief The UI manager.
  dx_ui_manager* ui_manager;
  /// @brief The UI text for the input of this console.
  dx_ui_text* ui_input_field;
  /// @brief The UI text for the output of this console.
  dx_ui_text* ui_output_field;
  /// @brief The UI group of this console.
  dx_ui_group* ui_group;
  // t indicates the current state.
  // this is a value between 0 and 1.
  // 0 means fully closed, 1 means fully opened.
  dx_f32 cs;
  // dt indicates the target state.
  // this is a value between 0 and 1.
  dx_f32 ts;
  /// @brief The console prompt.
  dx_string_buffer *prompt;
};

static inline dx_default_console_dispatch* DX_DEFAULT_CONSOLE_DISPATCH(void* p) {
  return (dx_default_console_dispatch*)p;
}

struct dx_default_console_dispatch {
  dx_console_dispatch _parent;
};

dx_result dx_default_console_construct(dx_default_console* SELf, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

dx_result dx_default_console_create(dx_default_console** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter);

#endif // DX_DEFAULT_CONSOLE_H_INCLUDED
