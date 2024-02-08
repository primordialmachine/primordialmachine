#if !defined(DX_DEFAULT_CONSOLE_H_INCLUDED)
#define DX_DEFAULT_CONSOLE_H_INCLUDED

#include "dx/document.h"
#include "dx/console.h"
#include "dx/val/context.h"
#include "dx/ui/text_field.h"
#include "dx/ui/group.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.default_console",
                       dx_default_console,
                       dx_console);

static inline dx_default_console* DX_DEFAULT_CONSOLE(void* p) {
  return (dx_default_console*)p;
}

struct dx_default_console {
  dx_console _parent;
  /// @brief The UI manager.
  dx_ui_manager* ui_manager;
  /// @brief The UI text field for the input of this console.
  dx_ui_text_field* ui_input_field;
  /// @brief The UI text field for the output of this console.
  dx_ui_text_field* ui_output_field;
  /// @brief The UI group of this console.
  dx_ui_group* ui_group;
  // t indicates the current state.
  // this is a value between 0 and 1.
  // 0 means fully closed, 1 means fully opened.
  Core_Real32 cs;
  // dt indicates the target state.
  // this is a value between 0 and 1.
  Core_Real32 ts;
  /// @brief The console prompt.
  Core_StringBuffer *prompt;
};

static inline dx_default_console_Dispatch* DX_DEFAULT_CONSOLE_DISPATCH(void* p) {
  return (dx_default_console_Dispatch*)p;
}

struct dx_default_console_Dispatch {
  dx_console_Dispatch _parent;
};

Core_Result dx_default_console_construct(dx_default_console* SELF, dx_ui_manager* ui_manager);

Core_Result dx_default_console_create(dx_default_console** RETURN, dx_ui_manager* ui_manager);

#endif // DX_DEFAULT_CONSOLE_H_INCLUDED
