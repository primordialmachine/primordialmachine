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
static dx_f32 const OPEN_CLOSE_SPEED = 2.f;
// the maximum number of console lines.
static dx_size const MAX_LINES = 20;

// the console background color
static DX_RGBA_F32 const CONSOLE_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f };
// the text color
static DX_RGBA_F32 const TEXT_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

DX_DEFINE_OBJECT_TYPE("dx.default_console",
                      dx_default_console,
                      dx_console);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Execute the prompt.
/// @method{dx_default_console}
static dx_result on_execute_prompt(dx_default_console* SELF);

static dx_result on_keyboard_key_message(dx_default_console* SELF, dx_keyboard_key_msg* keyboard_key_message);

static dx_result render(dx_default_console* SELF, dx_f32 delta_seconds, dx_i32 canvas_size_horizontal, dx_i32 canvas_size_vertical, dx_i32 dpi_horizontal, dx_i32 dpi_vertical);

static dx_result open(dx_default_console* SELF);

static dx_result close(dx_default_console* SELF);

static dx_result toggle(dx_default_console* SELF);

static dx_result is_open(dx_bool* RETURN, dx_default_console* SELF);

static dx_result is_closed(dx_bool* RETURN, dx_default_console* SELF);

static dx_result append_output_text(dx_default_console* SELF, dx_string* text);

static dx_result get_wigdget_by_name(dx_ui_widget** RETURN, dx_default_console* SELF, const char* name);

static dx_result on_layout(dx_default_console* SELF, dx_f32 console_x, dx_f32 console_y, dx_f32 console_w, dx_f32 console_h);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result on_execute_prompt(dx_default_console* SELF) {
  dx_string* string = NULL;
  if (dx_string_buffer_to_string(&string, SELF->prompt)) {
    return DX_FAILURE;
  }
  dx_application_presenter* application_presenter = NULL;
  if (dx_application_presenter_get(&application_presenter)) {
    DX_UNREFERENCE(string);
    string = NULL;
    return DX_FAILURE;
  }
  dx_cl_interpreter* cl_interpreter = NULL;
  if (dx_application_presenter_get_cl_interpreter(&cl_interpreter, application_presenter)) {
    DX_UNREFERENCE(application_presenter);
    application_presenter = NULL;
    DX_UNREFERENCE(string);
    string = NULL;
    return DX_FAILURE;
  }
  dx_console_append_output_text(DX_CONSOLE(SELF), string);
  dx_string* newline = NULL;
  if (dx_string_create(&newline, "\n", sizeof("\n") - 1)) {
    DX_UNREFERENCE(string);
    string = NULL;
    DX_UNREFERENCE(application_presenter);
    application_presenter = NULL;
    DX_UNREFERENCE(string);
    string = NULL;
    return DX_FAILURE;
  }
  dx_console_append_output_text(DX_CONSOLE(SELF), newline);
  DX_UNREFERENCE(newline);
  newline = NULL;

  dx_string_buffer_clear(SELF->prompt);
  if (dx_cl_interpreter_execute(cl_interpreter, application_presenter, string)) {
    dx_set_error(DX_NO_ERROR);
  }
  DX_UNREFERENCE(string);
  string = NULL;
  DX_UNREFERENCE(cl_interpreter);
  cl_interpreter = NULL;
  DX_UNREFERENCE(application_presenter);
  application_presenter = NULL;
  return DX_SUCCESS;
}

static dx_result on_keyboard_key_message(dx_default_console* SELF, dx_keyboard_key_msg* keyboard_key_message) {
  dx_keyboard_key_action action;
  dx_keyboard_key key;
  if (dx_keyboard_key_msg_get_action(&action, keyboard_key_message) || dx_keyboard_key_msg_get_key(&key, keyboard_key_message)) {
    return DX_FAILURE;
  }
  if (DX_KEYBOARD_KEY_ACTION_RELEASED == action) {
    if (dx_keyboard_key_return == key) {
      return on_execute_prompt(SELF);
    } else if (dx_keyboard_key_backspace == key) {
      dx_size number_of_bytes;
      if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->prompt)) {
        return DX_FAILURE;
      }
      if (number_of_bytes > 0) {
        return dx_string_buffer_remove_last(SELF->prompt);
      } else {
        return DX_SUCCESS;
      }
    } else if (dx_keyboard_key_space == key) {
      char symbol = ' ';
      return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
    } else if (dx_keyboard_key_less == key) {
      char symbol;
      uint8_t modifiers = 0;
      if (dx_input_msg_get_modifiers(&modifiers, DX_INPUT_MSG(keyboard_key_message))) {
        return DX_FAILURE;
      }
      if (dx_keyboard_key_less == key) {
        symbol = modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT) ? '>' : '<';
      } else {
        dx_set_error(DX_ERROR_ENVIRONMENT_FAILED); // should not happen
        return DX_FAILURE;
      }
      return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
    } else if (dx_keyboard_key_minus == key || dx_keyboard_key_plus == key || dx_keyboard_key_comma == key ||
               dx_keyboard_key_period == key) {
      char symbol;
      uint8_t modifiers = 0;
      if (dx_input_msg_get_modifiers(&modifiers, DX_INPUT_MSG(keyboard_key_message))) {
        return DX_FAILURE;
      }
      if (dx_keyboard_key_minus == key) {
        symbol = modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT) ? '_' : '-';
      } else if (dx_keyboard_key_plus == key) {
        symbol = modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT) ? '*' : '+';
      } else if (dx_keyboard_key_comma == key) {
        symbol = modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT) ? ';' : ',';
      } else if (dx_keyboard_key_period == key) {
        symbol = modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT) ? ':' : '.';
      } else {
        dx_set_error(DX_ERROR_ENVIRONMENT_FAILED); // should not happen
        return DX_FAILURE;
      }
      return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
    } else if (dx_keyboard_key__0 <= key && key <= dx_keyboard_key__9) {
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
      uint8_t modifiers = 0;
      if (dx_input_msg_get_modifiers(&modifiers, DX_INPUT_MSG(keyboard_key_message))) {
        return DX_FAILURE;
      }
      if (modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT)) {
        symbol = SHIFT_MAP[key - dx_keyboard_key__0];
        return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
      } else {
        symbol = MAP[key - dx_keyboard_key__0];
        return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
      }
    } else if (dx_keyboard_key_a <= key && key <= dx_keyboard_key_z) {
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
      uint8_t modifiers = 0;
      if (dx_input_msg_get_modifiers(&modifiers, DX_INPUT_MSG(keyboard_key_message))) {
        return DX_FAILURE;
      }
      if (modifiers & (DX_MODIFIER_LSHIFT | DX_MODIFIER_RSHIFT)) {
        symbol = SHIFT_MAP[key - dx_keyboard_key_a];
        return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
      } else {
        symbol = MAP[key - dx_keyboard_key_a];
        return dx_string_buffer_append_bytes(SELF->prompt, &symbol, 1);
      }
    } else {
      return DX_SUCCESS;
    }
  }
  return DX_SUCCESS;
}

// this layouter
// - it applies a margin to the group box containing the input field and the output field
// - sets the size of the input field to one line
// - sets the size of the output field to canvas height minus the size of the input field
// - places the output field above the input field
static dx_result render(dx_default_console* SELF, dx_f32 delta_seconds, dx_i32 canvas_size_horizontal, dx_i32 canvas_size_vertical, dx_i32 dpi_horizontal, dx_i32 dpi_vertical) {
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
    dx_f32 baseline_distance;
    dx_font_get_baseline_distance(&baseline_distance, SELF->ui_output_field->font);
    // the distance from the baseline to the maximal extend of any symbol above the baseline.
    dx_f32 ascender;
    dx_font_get_ascender(&ascender, SELF->ui_output_field->font);
    // the distance from the baseline to the maximal extend of any symbol below the baseline.
    dx_f32 descender;
    dx_font_get_descender(&descender, SELF->ui_output_field->font);
    // the distance of the baselines of two lines.
    baseline_distance = baseline_distance * 1.5f;
    dx_f32 insets_x = 16.f;
    dx_f32 insets_y = 16.f;

    dx_f32 max_line_height = ascender - descender;

    // when the console is closed, it starts above canvas size vertical.
    dx_f32 console_position_y_min = canvas_size_vertical;
    // the position of the console along the x-axis is 0.
    dx_f32 console_position_x = 0.f;
    // the width of the console is the horizontal size of the canvas.
    dx_f32 console_width = canvas_size_horizontal;
    // the height of the console is a little bit less then the height of the canvas.
    dx_f32 console_height = canvas_size_vertical - 16.f * 4;
    dx_f32 console_position_y_max = canvas_size_vertical - console_height;

    dx_f32 console_position_y;
    // we lerp from position canvas_
    dx_lerp(&console_position_y, console_position_y_min, console_position_y_max, SELF->cs);

    {
      DX_VEC2_F32 temporary;

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
        dx_string_buffer* string_buffer;
        if (dx_string_buffer_clone(&string_buffer, SELF->prompt)) {
          return DX_FAILURE;
        }

        dx_f32 startx, starty;

        // console input
        startx = console_position_x + insets_x;
        starty = console_position_y + insets_y - descender;
        static const char* SYMBOL = ">";
        if (dx_string_buffer_prepend_bytes(string_buffer, SYMBOL, strlen(SYMBOL))) {
          DX_UNREFERENCE(string_buffer);
          string_buffer = NULL;
          return DX_FAILURE;
        }
        dx_string* string = NULL;
        if (dx_string_buffer_to_string(&string, string_buffer)) {
          DX_UNREFERENCE(string_buffer);
          string_buffer = NULL;
          return DX_FAILURE;
        }
        DX_UNREFERENCE(string_buffer);
        string_buffer = NULL;
        {
          dx_ui_text_field_set_text_color(SELF->ui_input_field, &TEXT_COLOR);
          DX_RGBA_F32 bgc;
          dx_rgba_f32_set(&bgc, 0.f, 0.f, 0.f, 1.f);
          dx_rgba_f32_set(&bgc, 1.f, 0.f, 0.f, 1.f);
          dx_ui_text_field_set_background_color(SELF->ui_input_field, &bgc);
          SELF->ui_input_field->vertical_text_anchor = dx_text_anchor_vertical_center;
        }
        {
        }
        {
        }
        {
          dx_ui_text_field_set_text(SELF->ui_input_field, string);
        }
        DX_UNREFERENCE(string);
        string = NULL;

        // console output
        startx = console_position_x + insets_x;
        starty += baseline_distance;
        {
          DX_VEC2_F32 p;
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
  return DX_SUCCESS;
}

static dx_result open(dx_default_console* SELF) {
  SELF->ts = 1.f;
  return DX_SUCCESS;
}

static dx_result close(dx_default_console* SELF) {
  SELF->ts = 0.f;
  return DX_SUCCESS;
}

static dx_result toggle(dx_default_console* SELF) {
  if (SELF->ts == 1.f) {
    SELF->ts = 0.f;
  } else if (SELF->ts == 0.f) {
    SELF->ts = 1.f;
  }
  return DX_SUCCESS;
}

static dx_result is_open(dx_bool* RETURN, dx_default_console* SELF) {
  *RETURN = 1.f == SELF->ts;
  return DX_SUCCESS;
}

static dx_result is_closed(dx_bool* RETURN, dx_default_console* SELF) {
  *RETURN = 0.f == SELF->ts;
  return DX_SUCCESS;
}

static dx_result append_output_text(dx_default_console* SELF, dx_string* text) {
  if (dx_ui_text_field_append_text(SELF->ui_output_field, text)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result get_wigdget_by_name(dx_ui_widget** RETURN, dx_default_console* SELF, const char* name) {
  dx_string* name1 = NULL;
  if (dx_string_create(&name1, name, strlen(name))) {
    return DX_FAILURE;
  }
  dx_ui_widget* widget1 = NULL;
  if (dx_ui_widget_get_child_by_name(&widget1, DX_UI_WIDGET(SELF->ui_group), name1)) {
    return DX_FAILURE;
  }
  *RETURN = widget1;
  return DX_SUCCESS;
}

static dx_result on_layout(dx_default_console* SELF, dx_f32 console_x, dx_f32 console_y, dx_f32 console_w, dx_f32 console_h) {
  // positive y axis is going up

  // the insets on the left side of the "console" group.
  dx_f32 insets_l = 16.f;
  // the insets on the top side of the "console" group.
  dx_f32 insets_t = 16.f;
  // the insets on the bottom side of the "console" group.
  dx_f32 insets_b = 16.f;
  // the insets on the right side of the "console" group.
  dx_f32 insets_r = 16.f;

  // set the position and the size of the "console" group.
  DX_VEC2_F32 v;
  dx_vec2_f32_set(&v, console_x, console_y);
  dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->ui_group), &v);
  dx_vec2_f32_set(&v, console_w, console_h);
  dx_ui_widget_set_relative_size(DX_UI_WIDGET(SELF->ui_group), &v);
  // set the position and the size of the "output" text.

  // set the position and the size of the "input" text.
  {
    DX_VEC2_F32 p;
    dx_vec2_f32_set(&p, console_x + insets_l, console_y + insets_b);
    dx_ui_widget_set_relative_position(DX_UI_WIDGET(SELF->ui_input_field), &p);
  }
  {
    dx_f32 baseline_distance;
    dx_font_get_baseline_distance(&baseline_distance, SELF->ui_input_field->font);
    baseline_distance *= 1.5f;

    DX_VEC2_F32 s;
    dx_vec2_f32_set(&s, console_w - insets_l - insets_r,
                        baseline_distance);
    dx_ui_widget_set_relative_size(DX_UI_WIDGET(SELF->ui_input_field), &s);
  }

  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_default_console_destruct(dx_default_console* SELF) {
  DX_UNREFERENCE(SELF->ui_output_field);
  SELF->ui_output_field = NULL;

  DX_UNREFERENCE(SELF->ui_input_field);
  SELF->ui_input_field = NULL;

  DX_UNREFERENCE(SELF->ui_group);
  SELF->ui_group = NULL;

  DX_UNREFERENCE(SELF->ui_manager);
  SELF->ui_manager = NULL;

  DX_UNREFERENCE(SELF->prompt);
  SELF->prompt = NULL;
}

static void dx_default_console_dispatch_construct(dx_default_console_dispatch* SELF) {
  DX_CONSOLE_DISPATCH(SELF)->close = (dx_result(*)(dx_console*)) & close;
  DX_CONSOLE_DISPATCH(SELF)->on_keyboard_key_message = (dx_result(*)(dx_console*,dx_keyboard_key_msg*)) & on_keyboard_key_message;
  DX_CONSOLE_DISPATCH(SELF)->open = (dx_result(*)(dx_console*)) & open;
  DX_CONSOLE_DISPATCH(SELF)->close = (dx_result(*)(dx_console*)) & close;
  DX_CONSOLE_DISPATCH(SELF)->render = (dx_result(*)(dx_console*, dx_f32, dx_i32, dx_i32, dx_i32, dx_i32)) & render;
  DX_CONSOLE_DISPATCH(SELF)->toggle = (dx_result(*)(dx_console*)) & toggle;
  DX_CONSOLE_DISPATCH(SELF)->is_open = (dx_result(*)(dx_bool*,dx_console*)) & is_open;
  DX_CONSOLE_DISPATCH(SELF)->is_closed = (dx_result(*)(dx_bool*,dx_console*)) & is_closed;
  DX_CONSOLE_DISPATCH(SELF)->append_output_text = (dx_result(*)(dx_console*, dx_string*)) & append_output_text;
}

dx_result dx_default_console_construct(dx_default_console* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  dx_rti_type* TYPE = dx_default_console_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_console_construct(DX_CONSOLE(SELF))) {
    return DX_FAILURE;
  }
  if (dx_ui_manager_create(&SELF->ui_manager, font_presenter, rectangle_presenter)) {
    return DX_FAILURE;
  }
  {
    dx_string* path = NULL;
    if (dx_string_create(&path, "./assets/console/console.ui", sizeof("./assets/console/console.ui") - 1)) {
      DX_UNREFERENCE(SELF->ui_manager);
      SELF->ui_manager = NULL;
      return DX_FAILURE;
    }
    dx_ui_widget* temporary = NULL;
    if (dx_ui_manager_load(&temporary, SELF->ui_manager, path)) {
      DX_UNREFERENCE(path);
      path = NULL;
      DX_UNREFERENCE(SELF->ui_manager);
      SELF->ui_manager = NULL;
      return DX_FAILURE;
    }
    SELF->ui_group = DX_UI_GROUP(temporary);
    DX_UNREFERENCE(path);
    path = NULL;
  }
  if (get_wigdget_by_name((dx_ui_widget**) & SELF->ui_input_field, SELF, "inputField")) {
    DX_UNREFERENCE(SELF->ui_group);
    SELF->ui_group = NULL;
    DX_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return DX_FAILURE;
  }
  if (get_wigdget_by_name((dx_ui_widget**) & SELF->ui_output_field, SELF, "outputField")) {
    DX_UNREFERENCE(SELF->ui_input_field);
    SELF->ui_input_field = NULL;
    DX_UNREFERENCE(SELF->ui_group);
    SELF->ui_group = NULL;
    DX_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return DX_FAILURE;
  }
  if (dx_string_buffer_create(&SELF->prompt)) {
    DX_UNREFERENCE(SELF->ui_output_field);
    SELF->ui_output_field = NULL;
    DX_UNREFERENCE(SELF->ui_input_field);
    SELF->ui_input_field = NULL;
    DX_UNREFERENCE(SELF->ui_group);
    SELF->ui_group = NULL;
    DX_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return DX_FAILURE;
  }
  SELF->cs = 0.f;
  SELF->ts = 0.f;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_default_console_create(dx_default_console** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_default_console)
  if (dx_default_console_construct(SELF, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
