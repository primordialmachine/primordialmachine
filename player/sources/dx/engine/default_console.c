#include "dx/engine/default_console.h"

#include "dx/engine/console_commands.h"
#include "dx/player/run.h"
#include "dx/document_definition_language.h"
#include "dx/ui/group.h"
#include "dx/ui/text_field.h"
#include "dx/ui/manager.h"
// strlen
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// open/close with 2 units per second.
static Core_Real32 const OPEN_CLOSE_SPEED = 2.f;
// the maximum number of console lines.
static Core_Size const MAX_LINES = 20;

// the console background color
static Core_InlineRgbaR32 const CONSOLE_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f };
// the text color
static Core_InlineRgbaR32 const TEXT_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

Core_defineObjectType("dx.default_console",
                      dx_default_console,
                      dx_console);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Execute the prompt.
/// @method{dx_default_console}
static Core_Result on_execute_prompt(dx_default_console* SELF);

static Core_Result on_keyboard_key_message(dx_default_console* SELF, Core_KeyboardKeyMessage* keyboard_key_message);

static Core_Result render(dx_default_console* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_size_horizontal, Core_Integer32 canvas_size_vertical, Core_Integer32 dpi_horizontal, Core_Integer32 dpi_vertical);

static Core_Result open(dx_default_console* SELF);

static Core_Result close(dx_default_console* SELF);

static Core_Result toggle(dx_default_console* SELF);

static Core_Result is_open(Core_Boolean* RETURN, dx_default_console* SELF);

static Core_Result is_closed(Core_Boolean* RETURN, dx_default_console* SELF);

static Core_Result append_output_text(dx_default_console* SELF, Core_String* text);

static Core_Result get_wigdget_by_name(dx_ui_widget** RETURN, dx_default_console* SELF, const char* name);

static Core_Result on_layout(dx_default_console* SELF, Core_Real32 console_x, Core_Real32 console_y, Core_Real32 console_w, Core_Real32 console_h);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result on_execute_prompt(dx_default_console* SELF) {
  Core_String* string = NULL;
  if (Core_StringBuffer_getString(&string, SELF->prompt)) {
    return Core_Failure;
  }
  dx_application_presenter* application_presenter = NULL;
  if (dx_application_presenter_get(&application_presenter)) {
    CORE_UNREFERENCE(string);
    string = NULL;
    return Core_Failure;
  }
  dx_cl_interpreter* cl_interpreter = NULL;
  if (dx_application_presenter_get_cl_interpreter(&cl_interpreter, application_presenter)) {
    CORE_UNREFERENCE(application_presenter);
    application_presenter = NULL;
    CORE_UNREFERENCE(string);
    string = NULL;
    return Core_Failure;
  }
  dx_console_append_output_text(DX_CONSOLE(SELF), string);
  Core_String* newline = NULL;
  if (Core_String_create(&newline, "\n", sizeof("\n") - 1)) {
    CORE_UNREFERENCE(string);
    string = NULL;
    CORE_UNREFERENCE(application_presenter);
    application_presenter = NULL;
    CORE_UNREFERENCE(string);
    string = NULL;
    return Core_Failure;
  }
  dx_console_append_output_text(DX_CONSOLE(SELF), newline);
  CORE_UNREFERENCE(newline);
  newline = NULL;

  Core_StringBuffer_clear(SELF->prompt);
  if (dx_cl_interpreter_execute(cl_interpreter, application_presenter, string)) {
    Core_setError(Core_Error_NoError);
  }
  CORE_UNREFERENCE(string);
  string = NULL;
  CORE_UNREFERENCE(cl_interpreter);
  cl_interpreter = NULL;
  CORE_UNREFERENCE(application_presenter);
  application_presenter = NULL;
  return Core_Success;
}

static Core_Result on_keyboard_key_message(dx_default_console* SELF, Core_KeyboardKeyMessage* keyboard_key_message) {
  Core_KeyboardKeyAction action;
  Core_KeyboardKey key;
  if (Core_KeyboardKeyMessage_getAction(&action, keyboard_key_message) || Core_KeyboardKeyMessage_getKey(&key, keyboard_key_message)) {
    return Core_Failure;
  }
  if (Core_KeyboardKeyAction_Released == action) {
    if (Core_KeyboardKey_Return == key) {
      return on_execute_prompt(SELF);
    } else if (Core_KeyboardKey_Backspace == key) {
      Core_Size number_of_bytes;
      if (Core_StringBuffer_getNumberOfBytes(&number_of_bytes, SELF->prompt)) {
        return Core_Failure;
      }
      if (number_of_bytes > 0) {
        return Core_StringBuffer_remove_last(SELF->prompt);
      } else {
        return Core_Success;
      }
    } else if (Core_KeyboardKey_Space == key) {
      char symbol = ' ';
      return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
    } else if (Core_KeyboardKey_Less == key) {
      char symbol;
      Core_ModifierKeys modifierKeys = 0;
      if (Core_InputMessage_getModifierKeys(&modifierKeys, CORE_INPUTMESSAGE(keyboard_key_message))) {
        return Core_Failure;
      }
      if (Core_KeyboardKey_Less == key) {
        symbol = modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift) ? '>' : '<';
      } else {
        Core_setError(Core_Error_EnvironmentFailed); // should not happen
        return Core_Failure;
      }
      return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
    } else if (Core_KeyboardKey_Minus == key || Core_KeyboardKey_Plus == key || Core_KeyboardKey_Comma == key ||
               Core_KeyboardKey_Period == key) {
      char symbol;
      Core_ModifierKeys modifierKeys = 0;
      if (Core_InputMessage_getModifierKeys(&modifierKeys, CORE_INPUTMESSAGE(keyboard_key_message))) {
        return Core_Failure;
      }
      if (Core_KeyboardKey_Minus == key) {
        symbol = modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift) ? '_' : '-';
      } else if (Core_KeyboardKey_Plus == key) {
        symbol = modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift) ? '*' : '+';
      } else if (Core_KeyboardKey_Comma == key) {
        symbol = modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift) ? ';' : ',';
      } else if (Core_KeyboardKey_Period == key) {
        symbol = modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift) ? ':' : '.';
      } else {
        Core_setError(Core_Error_EnvironmentFailed); // should not happen
        return Core_Failure;
      }
      return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
    } else if (Core_KeyboardKey_D0 <= key && key <= Core_KeyboardKey_D9) {
      static char MAP[] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
      };
      static char SHIFT_MAP[] = {
        '=',
        '!',
        '2',
        '§',
        '$',
        '%',
        '&',
        '/',
        '(',
        ')',
      };
      char symbol;
      Core_ModifierKeys modifierKeys = 0;
      if (Core_InputMessage_getModifierKeys(&modifierKeys, CORE_INPUTMESSAGE(keyboard_key_message))) {
        return Core_Failure;
      }
      if (modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift)) {
        symbol = SHIFT_MAP[key - Core_KeyboardKey_D0];
        return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
      } else {
        symbol = MAP[key - Core_KeyboardKey_D0];
        return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
      }
    } else if (Core_KeyboardKey_A <= key && key <= Core_KeyboardKey_Z) {
      static char MAP[] = {
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x',
        'y',
        'z',
      };
      static char SHIFT_MAP[] = {
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P',
        'Q',
        'R',
        'S',
        'T',
        'U',
        'V',
        'W',
        'X',
        'Y',
        'Z',
      };
      char symbol;
      Core_ModifierKeys modifierKeys = 0;
      if (Core_InputMessage_getModifierKeys(&modifierKeys, CORE_INPUTMESSAGE(keyboard_key_message))) {
        return Core_Failure;
      }
      if (modifierKeys & (Core_ModifierKeys_LeftShift | Core_ModifierKeys_RightShift)) {
        symbol = SHIFT_MAP[key - Core_KeyboardKey_A];
        return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
      } else {
        symbol = MAP[key - Core_KeyboardKey_A];
        return Core_StringBuffer_appendBytes(SELF->prompt, &symbol, 1);
      }
    } else {
      return Core_Success;
    }
  }
  return Core_Success;
}

// this layouter
// - it applies a margin to the group box containing the input field and the output field
// - sets the size of the input field to one line
// - sets the size of the output field to canvas height minus the size of the input field
// - places the output field above the input field
static Core_Result render(dx_default_console* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_size_horizontal, Core_Integer32 canvas_size_vertical, Core_Integer32 dpi_horizontal, Core_Integer32 dpi_vertical) {
  if (SELF->cs < SELF->ts) {
    SELF->cs += OPEN_CLOSE_SPEED * delta_seconds;
    if (SELF->cs > SELF->ts) {
      SELF->cs = SELF->ts;
    }
  } else if (SELF->cs > SELF->ts) {
    SELF->cs -= OPEN_CLOSE_SPEED * delta_seconds;
    if (SELF->cs < SELF->ts) {
      SELF->cs = SELF->ts;
    }
  }

  if (SELF->cs > 0.f) {
    // this is basically the standard distance from one baseline to another.
    // usually, you want this value to be multiplied by 1.5.
    Core_Real32 baseline_distance;
    dx_font_get_baseline_distance(&baseline_distance, SELF->ui_output_field->font);
    // the distance from the baseline to the maximal extend of any symbol above the baseline.
    Core_Real32 ascender;
    dx_font_get_ascender(&ascender, SELF->ui_output_field->font);
    // the distance from the baseline to the maximal extend of any symbol below the baseline.
    Core_Real32 descender;
    dx_font_get_descender(&descender, SELF->ui_output_field->font);
    // the distance of the baselines of two lines.
    baseline_distance = baseline_distance * 1.5f;
    Core_Real32 insets_x = 16.f;
    Core_Real32 insets_y = 16.f;

    Core_Real32 max_line_height = ascender - descender;

    // when the console is closed, it starts above canvas size vertical.
    Core_Real32 console_position_y_min = canvas_size_vertical;
    // the position of the console along the x-axis is 0.
    Core_Real32 console_position_x = 0.f;
    // the width of the console is the horizontal size of the canvas.
    Core_Real32 console_width = canvas_size_horizontal;
    // the height of the console is a little bit less then the height of the canvas.
    Core_Real32 console_height = canvas_size_vertical - 16.f * 4;
    Core_Real32 console_position_y_max = canvas_size_vertical - console_height;

    Core_Real32 console_position_y;
    // we lerp from position canvas_
    dx_lerp(&console_position_y, console_position_y_min, console_position_y_max, SELF->cs);

    {
      Core_InlineVector2R32 temporary;

      dx_vec2_f32_set(&temporary, dpi_horizontal, dpi_vertical);
      dx_ui_manager_set_canvas_dpi(SELF->ui_manager, &temporary);
      
      dx_vec2_f32_set(&temporary, canvas_size_horizontal, canvas_size_vertical);
      dx_ui_manager_set_canvas_resolution(SELF->ui_manager, &temporary);

      dx_ui_manager_enter_render(SELF->ui_manager);
    }
    {
      on_layout(SELF, console_position_x, console_position_y, console_width, console_height);
      dx_ui_group_set_background_color(SELF->ui_group, &CONSOLE_BACKGROUND_COLOR);
      //
      {
        Core_StringBuffer* string_buffer;
        if (Core_StringBuffer_clone(&string_buffer, SELF->prompt)) {
          return Core_Failure;
        }

        Core_Real32 startx, starty;

        // console input
        startx = console_position_x + insets_x;
        starty = console_position_y + insets_y - descender;
        static const char* SYMBOL = ">";
        if (Core_StringBuffer_prependBytes(string_buffer, SYMBOL, strlen(SYMBOL))) {
          CORE_UNREFERENCE(string_buffer);
          string_buffer = NULL;
          return Core_Failure;
        }
        Core_String* string = NULL;
        if (Core_StringBuffer_getString(&string, string_buffer)) {
          CORE_UNREFERENCE(string_buffer);
          string_buffer = NULL;
          return Core_Failure;
        }
        CORE_UNREFERENCE(string_buffer);
        string_buffer = NULL;
        {
          dx_ui_text_field_set_text(SELF->ui_input_field, string);
        }
        CORE_UNREFERENCE(string);
        string = NULL;

        // console output
        startx = console_position_x + insets_x;
        starty += baseline_distance;
        {
          Core_InlineVector2R32 p;
          dx_vec2_f32_set(&p, startx, starty);
          dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->ui_output_field), &p);
        }
      }
    }
    // render
    dx_ui_manager_set_root(SELF->ui_manager, DX_UI_WIDGET(SELF->ui_group));
    dx_ui_manager_leave_render(SELF->ui_manager);
    dx_ui_manager_set_root(SELF->ui_manager, NULL);
  }
  return Core_Success;
}

static Core_Result open(dx_default_console* SELF) {
  SELF->ts = 1.f;
  return Core_Success;
}

static Core_Result close(dx_default_console* SELF) {
  SELF->ts = 0.f;
  return Core_Success;
}

static Core_Result toggle(dx_default_console* SELF) {
  if (SELF->ts == 1.f) {
    SELF->ts = 0.f;
  } else if (SELF->ts == 0.f) {
    SELF->ts = 1.f;
  }
  return Core_Success;
}

static Core_Result is_open(Core_Boolean* RETURN, dx_default_console* SELF) {
  *RETURN = 1.f == SELF->ts;
  return Core_Success;
}

static Core_Result is_closed(Core_Boolean* RETURN, dx_default_console* SELF) {
  *RETURN = 0.f == SELF->ts;
  return Core_Success;
}

static Core_Result append_output_text(dx_default_console* SELF, Core_String* text) {
  if (dx_ui_text_field_append_text(SELF->ui_output_field, text)) {
    return Core_Failure;
  }
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result get_wigdget_by_name(dx_ui_widget** RETURN, dx_default_console* SELF, const char* name) {
  Core_String* name1 = NULL;
  if (Core_String_create(&name1, name, strlen(name))) {
    return Core_Failure;
  }
  dx_ui_widget* widget1 = NULL;
  if (dx_ui_widget_get_child_by_name(&widget1, DX_UI_WIDGET(SELF->ui_group), name1)) {
    return Core_Failure;
  }
  *RETURN = widget1;
  return Core_Success;
}

static Core_Result on_layout(dx_default_console* SELF, Core_Real32 console_x, Core_Real32 console_y, Core_Real32 console_w, Core_Real32 console_h) {
  // positive y axis is going up

  // the insets on the left side of the "console" group.
  Core_Real32 insets_l = 16.f;
  // the insets on the top side of the "console" group.
  Core_Real32 insets_t = 16.f;
  // the insets on the bottom side of the "console" group.
  Core_Real32 insets_b = 16.f;
  // the insets on the right side of the "console" group.
  Core_Real32 insets_r = 16.f;

  // margin between to controls
  Core_Real32 margin = 16.f;

  // set the position and the size of the "console" group.
  Core_InlineVector2R32 v;
  dx_vec2_f32_set(&v, console_x, console_y);
  dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->ui_group), &v);
  dx_vec2_f32_set(&v, console_w, console_h);
  dx_ui_widget_set_relative_size(DX_UI_WIDGET(SELF->ui_group), &v);

  // set the size of the "input" text field.
  {
    Core_Real32 baseline_distance;
    dx_font_get_baseline_distance(&baseline_distance, SELF->ui_input_field->font);
    baseline_distance *= 1.5f;

    Core_InlineVector2R32 s;
    dx_vec2_f32_set(&s, console_w - insets_l - insets_r, baseline_distance);
    dx_ui_widget_set_relative_size(DX_UI_WIDGET(SELF->ui_input_field), &s);
  }
  // set the size of the "output" text field.
  {
    Core_InlineVector2R32 input_size;
    dx_ui_widget_get_relative_size(&input_size, DX_UI_WIDGET(SELF->ui_input_field));

    Core_InlineVector2R32 s;
    dx_vec2_f32_set(&s, console_w - insets_l - insets_r, console_h - input_size.e[1] - insets_t - margin);
    dx_ui_widget_set_relative_size(DX_UI_WIDGET(SELF->ui_output_field), &s);
  }

  // vertical stack layout
  float l = console_x + insets_l;
  float b = console_y + insets_b;
  // set the position of the "input" text field.
  {
    Core_InlineVector2R32 p;
    dx_vec2_f32_set(&p, l, b);
    dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->ui_input_field), &p);
  }

  // set the position the "output" text field.
  {
    Core_InlineVector2R32 input_size;
    dx_ui_widget_get_relative_size(&input_size, DX_UI_WIDGET(SELF->ui_input_field));

    b += input_size.e[1];

    Core_InlineVector2R32 p;
    dx_vec2_f32_set(&p, console_x + insets_l, b);
    dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->ui_output_field), &p);
  }

  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_default_console_destruct(dx_default_console* SELF) {
  CORE_UNREFERENCE(SELF->ui_output_field);
  SELF->ui_output_field = NULL;

  CORE_UNREFERENCE(SELF->ui_input_field);
  SELF->ui_input_field = NULL;

  CORE_UNREFERENCE(SELF->ui_group);
  SELF->ui_group = NULL;

  CORE_UNREFERENCE(SELF->ui_manager);
  SELF->ui_manager = NULL;

  CORE_UNREFERENCE(SELF->prompt);
  SELF->prompt = NULL;
}

static void dx_default_console_constructDispatch(dx_default_console_Dispatch* SELF) {
  DX_CONSOLE_DISPATCH(SELF)->close = (Core_Result(*)(dx_console*)) & close;
  DX_CONSOLE_DISPATCH(SELF)->on_keyboard_key_message = (Core_Result(*)(dx_console*, Core_KeyboardKeyMessage*)) & on_keyboard_key_message;
  DX_CONSOLE_DISPATCH(SELF)->open = (Core_Result(*)(dx_console*)) & open;
  DX_CONSOLE_DISPATCH(SELF)->close = (Core_Result(*)(dx_console*)) & close;
  DX_CONSOLE_DISPATCH(SELF)->render = (Core_Result(*)(dx_console*, Core_Real32, Core_Integer32, Core_Integer32, Core_Integer32, Core_Integer32)) & render;
  DX_CONSOLE_DISPATCH(SELF)->toggle = (Core_Result(*)(dx_console*)) & toggle;
  DX_CONSOLE_DISPATCH(SELF)->is_open = (Core_Result(*)(Core_Boolean*,dx_console*)) & is_open;
  DX_CONSOLE_DISPATCH(SELF)->is_closed = (Core_Result(*)(Core_Boolean*,dx_console*)) & is_closed;
  DX_CONSOLE_DISPATCH(SELF)->append_output_text = (Core_Result(*)(dx_console*, Core_String*)) & append_output_text;
}

Core_Result dx_default_console_construct(dx_default_console* SELF, dx_ui_manager* ui_manager) {
  DX_CONSTRUCT_PREFIX(dx_default_console);
  if (dx_console_construct(DX_CONSOLE(SELF))) {
    return Core_Failure;
  }
  SELF->ui_manager = ui_manager;
  CORE_REFERENCE(ui_manager);
  {
    Core_String* path = NULL;
    if (Core_String_create(&path, "./assets/console/console.ui", sizeof("./assets/console/console.ui") - 1)) {
      CORE_UNREFERENCE(SELF->ui_manager);
      SELF->ui_manager = NULL;
      return Core_Failure;
    }
    dx_ui_widget* temporary = NULL;
    if (dx_ui_manager_load(&temporary, SELF->ui_manager, path)) {
      CORE_UNREFERENCE(path);
      path = NULL;
      CORE_UNREFERENCE(SELF->ui_manager);
      SELF->ui_manager = NULL;
      return Core_Failure;
    }
    SELF->ui_group = DX_UI_GROUP(temporary);
    CORE_UNREFERENCE(path);
    path = NULL;
  }
  if (get_wigdget_by_name((dx_ui_widget**) & SELF->ui_input_field, SELF, "inputField")) {
    CORE_UNREFERENCE(SELF->ui_group);
    SELF->ui_group = NULL;
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  if (get_wigdget_by_name((dx_ui_widget**) & SELF->ui_output_field, SELF, "outputField")) {
    CORE_UNREFERENCE(SELF->ui_input_field);
    SELF->ui_input_field = NULL;
    CORE_UNREFERENCE(SELF->ui_group);
    SELF->ui_group = NULL;
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  if (Core_StringBuffer_create(&SELF->prompt)) {
    CORE_UNREFERENCE(SELF->ui_output_field);
    SELF->ui_output_field = NULL;
    CORE_UNREFERENCE(SELF->ui_input_field);
    SELF->ui_input_field = NULL;
    CORE_UNREFERENCE(SELF->ui_group);
    SELF->ui_group = NULL;
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }

  {
    dx_ui_text_field_set_text_color(SELF->ui_input_field, &TEXT_COLOR);
    Core_InlineRgbaR32 bgc;
    Core_InlineRgbaR32_set(&bgc, .24f, .24f, .24f, 1.f);
    dx_ui_text_field_set_background_color(SELF->ui_input_field, &bgc);
    SELF->ui_input_field->vertical_text_anchor = dx_text_anchor_vertical_center;
    SELF->ui_output_field->text_bounds_dirty = true;
  }
  {
    dx_ui_text_field_set_text_color(SELF->ui_output_field, &TEXT_COLOR);
    Core_InlineRgbaR32 bgc;
    Core_InlineRgbaR32_set(&bgc, 0.24f, 0.24f, 0.24f, 1.f);
    dx_ui_text_field_set_background_color(SELF->ui_output_field, &bgc);
    SELF->ui_output_field->vertical_text_anchor = dx_text_anchor_vertical_top;
    SELF->ui_output_field->text_bounds_dirty = true;
  }

  SELF->cs = 0.f;
  SELF->ts = 0.f;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_default_console_create(dx_default_console** RETURN, dx_ui_manager* ui_manager) {
  DX_CREATE_PREFIX(dx_default_console);
  if (dx_default_console_construct(SELF, ui_manager)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
