#if !defined(DX_DEFAULT_CONSOLE_H_INCLUDED)
#define DX_DEFAULT_CONSOLE_H_INCLUDED

#include "dx/console.h"
#include "dx/engine/font_presenter.h"
#include "dx/engine/rectangle_presenter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.text.line",
                       dx_text_line,
                       dx_object);

static inline dx_text_line* DX_TEXT_LINE(void* p) {
  return (dx_text_line*)p;
}

struct dx_text_line {
  dx_object _parent;
  dx_size start;
  dx_size length;
};

static inline dx_text_line_dispatch* DX_TEXT_LINE_DISPATCH(void* p) {
  return (dx_text_line_dispatch*)p;
}

struct dx_text_line_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_text_line_construct(dx_text_line* SELF, dx_size start, dx_size length);

dx_result dx_text_line_create(dx_text_line** RETURN, dx_size start, dx_size length);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.text.document",
                       dx_text_document,
                       dx_object);

static inline dx_text_document* DX_TEXT_DOCUMENT(void* p) {
  return (dx_text_document*)p;
}

struct dx_text_document {
  dx_object _parent;
  dx_string_buffer* text;
  dx_object_array *lines;
};

static inline dx_text_document_dispatch* DX_TEXT_DOCUMENT_DISPATCH(void* p) {
  return (dx_text_document_dispatch*)p;
}

struct dx_text_document_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_text_document_construct(dx_text_document* SELF);

dx_result dx_text_document_create(dx_text_document** RETURN);

dx_result dx_text_document_append_text(dx_text_document* SELF, dx_string* text);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.default_console",
                       dx_default_console,
                       dx_console);

static inline dx_default_console* DX_DEFAULT_CONSOLE(void* p) {
  return (dx_default_console*)p;
}

struct dx_default_console {
  dx_console _parent;
  /// The rectangle presenter.
  dx_rectangle_presenter* rectangle_presenter;
  /// The font presenter.
  dx_font_presenter* font_presenter;
  /// The default font of the console.
  dx_font* font;
  /// @brief The VAL context.
  dx_val_context* val_context;
  /// @brief The command list.
  dx_val_command_list* command_list;
  /// @brief The output of this console.
  dx_text_document* output;
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

dx_result dx_default_console_construct(dx_default_console* SELf, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* context);

dx_result dx_default_console_create(dx_default_console** RETURN, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* context);

#endif // DX_DEFAULT_CONSOLE_H_INCLUDED
