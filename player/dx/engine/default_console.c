#include "dx/engine/default_console.h"

#include "dx/val/command.h"
#include "dx/val/program_text.h"
#include "dx/val/texture.h"
#include "dx/engine.h"
#include "dx/engine/console_commands.h"
#include "dx/engine/utilities_2d.h"
#include "dx/player/run.h"
#include "dx/document_definition_language.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.text_line",
                      dx_text_line,
                      dx_object);

static void dx_text_line_destruct(dx_text_line* SELF)
{/*Intentionally empty.*/}

static void dx_text_line_dispatch_construct(dx_text_line_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_text_line_construct(dx_text_line* SELF, dx_size start, dx_size length) {
  dx_rti_type* TYPE = dx_text_line_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->start = start;
  SELF->length = length;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_text_line_create(dx_text_line** RETURN, dx_size start, dx_size length) {
  dx_text_line* SELF = DX_TEXT_LINE(dx_object_alloc(sizeof(dx_text_line)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_text_line_construct(SELF, start, length)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.text_document",
                      dx_text_document,
                      dx_object);

static void dx_text_document_destruct(dx_text_document* SELF) {
  DX_UNREFERENCE(SELF->lines);
  SELF->lines = NULL;
  DX_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_text_document_dispatch_construct(dx_text_document_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_text_document_construct(dx_text_document* SELF) {
  dx_rti_type* TYPE = dx_text_document_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_object_array_create(&SELF->lines, 0)) {
    return DX_FAILURE;
  }
  if (dx_string_buffer_create(&SELF->text)) {
    DX_UNREFERENCE(SELF->lines);
    SELF->lines = NULL;
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_text_document_create(dx_text_document** RETURN) {
  dx_text_document* SELF = DX_TEXT_DOCUMENT(dx_object_alloc(sizeof(dx_text_document)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_text_document_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result get_last_line_or_null(dx_text_line** RETURN, dx_text_document* SELF) {
  dx_size size = 0;
  if (dx_object_array_get_size(&size, SELF->lines)) {
    return DX_FAILURE;
  }
  if (size == 0) {
    *RETURN = NULL;
    return DX_SUCCESS;
  }
  return dx_object_array_get_at((dx_object**)RETURN, SELF->lines, size - 1);
}

static dx_result dx_string_buffer_ends_with_newline(dx_bool* RETURN, dx_string_buffer* SELF) {
  if (SELF->backend.size < 1) {
    *RETURN = DX_FALSE;
    return DX_SUCCESS;
  }
  *RETURN = (SELF->backend.elements[SELF->backend.size - 1] == '\n')
         || (SELF->backend.elements[SELF->backend.size - 1] == '\r');
  return DX_SUCCESS;
}

static dx_result dx_text_document_append_lines(dx_text_document* SELF, dx_object_array* lines) {
  dx_size existing_number_of_lines = 0, number_of_lines = 0;
  if (dx_object_array_get_size(&existing_number_of_lines, SELF->lines) || dx_object_array_get_size(&number_of_lines, lines)) {
    return DX_FAILURE;
  }
  // if the array is empty, there is nothing to do.
  if (0 == number_of_lines) {
    return DX_SUCCESS;
  }
  // if the last existing line does not end with a newline,
  // append the first (incoming) line to that line.
  dx_text_line* last_line = NULL;
  if (get_last_line_or_null(&last_line, SELF)) {
    return DX_FAILURE;
  }
  dx_size i = 0;
  if (last_line) {
    dx_bool ends_with_newline = false;
    if (dx_string_buffer_ends_with_newline(&ends_with_newline, SELF->text)) {
      return DX_FAILURE;
    }
    dx_text_line* current_line = NULL;
    if (dx_object_array_get_at((dx_object**)&current_line, lines, 0)) {
      return DX_FAILURE;
    }
    if (!ends_with_newline) {
      last_line->length += current_line->length;
      i++;
    }
  }
  // add the remaining lines.
  for (; i < number_of_lines; ++i) {
    dx_text_line* line = NULL;
    if (dx_object_array_get_at((dx_object**)&line, lines, i)) {
      return DX_FAILURE;
    }
    if (dx_object_array_append(SELF->lines, DX_OBJECT(line))) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

dx_result dx_text_document_append_text(dx_text_document* SELF, dx_string* text) {
  // Get the start offset.
  dx_size start_offset;
  if (dx_string_buffer_get_number_of_bytes(&start_offset, SELF->text)) {
    return DX_FAILURE;
  }
  dx_object_array *lines;
  if (dx_object_array_create(&lines, 0)) {
    return DX_FAILURE;
  }
  // Scan the text.
  dx_document_definition_language_scanner* scanner = NULL;
  if (dx_document_definition_language_scanner_create(&scanner)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  dx_size number_of_bytes = 0;
  if (dx_string_get_number_of_bytes(&number_of_bytes, text)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  void const* bytes = NULL;
  if (dx_string_get_bytes(&bytes, text)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  if (dx_scanner_set(DX_SCANNER(scanner), bytes, number_of_bytes)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  dx_document_definition_language_word_kind word_kind;
  // start of input
  if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  if (word_kind != dx_document_definition_language_word_kind_start_of_input) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    return DX_FAILURE;
  }
  if (dx_scanner_step(DX_SCANNER(scanner))) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  // lines*
  if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    return DX_FAILURE;
  }
  while (dx_document_definition_language_word_kind_line == word_kind) {
    dx_size word_start_offset = 0,
            word_end_offset = 0;
    if (dx_scanner_get_word_start_offset(&word_start_offset, DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    if (dx_scanner_get_word_end_offset(&word_end_offset, DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    dx_text_line* current_line = NULL;
    if (dx_text_line_create(&current_line, start_offset + word_start_offset, word_end_offset - word_start_offset)) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    if (dx_object_array_append(lines, DX_OBJECT(current_line))) {
      DX_UNREFERENCE(current_line);
      current_line = NULL;
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(current_line);
    current_line = NULL;
    if (dx_scanner_step(DX_SCANNER(scanner))) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
    if (dx_document_definition_language_scanner_get_word_kind(&word_kind, scanner)) {
      DX_UNREFERENCE(scanner);
      scanner = NULL;
      DX_UNREFERENCE(lines);
      lines = NULL;
      return DX_FAILURE;
    }
  }
  // end of input
  if (word_kind != dx_document_definition_language_word_kind_end_of_input) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(scanner);
  scanner = NULL;

  if (dx_text_document_append_lines(SELF, lines)) {
    DX_UNREFERENCE(lines);
    lines = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(lines);
  lines = NULL;
  if (dx_string_buffer_append_string(SELF->text, text)) {
    /// @todo Can we recover?
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// open/close with 2 units per second.
static dx_f32 const OPEN_CLOSE_SPEED = 2.f;
// the maximum number of console lines.
static dx_size const MAX_LINES = 20;
// the font file
static char* const FONT_FILE = "./assets/fonts/SourceCodePro-Regular.ttf";
/// The font size of the overlay in points.
/// @todo
/// The font size is 12pt. However, a DPI scaling of 225% is currently computed in such that the font size is 27pt = 12pt * 225%.
/// Font sizes in GUIs should be specified without incorporating some machine/monitor-specific DPI scalings.
static dx_size const FONT_SIZE = 27;
// the console background color
static DX_RGBA_F32 const CONSOLE_BACKGROUND_COLOR = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f };
// the text color
static DX_RGBA_F32 const TEXT_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

DX_DEFINE_OBJECT_TYPE("dx.default_console",
                      dx_default_console,
                      dx_console);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Execute the prompt.
/// @param SELF A pointer to this console.
/// @method-call
static dx_result on_execute_prompt(dx_default_console* SELF);

static dx_result on_keyboard_key_message(dx_default_console* SELF, dx_keyboard_key_msg* keyboard_key_message);

static dx_result render(dx_default_console* SELF, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height);

static dx_result open(dx_default_console* SELF);

static dx_result close(dx_default_console* SELF);

static dx_result toggle(dx_default_console* SELF);

static dx_result is_open(dx_bool* RETURN, dx_default_console* SELF);

static dx_result is_closed(dx_bool* RETURN, dx_default_console* SELF);

static dx_result append_text(dx_default_console* SELF, dx_string* text);

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
  dx_console_append_text(DX_CONSOLE(SELF), string);
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
  dx_console_append_text(DX_CONSOLE(SELF), newline);
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

static dx_result render(dx_default_console* SELF, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
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
    dx_font_get_baseline_distance(&baseline_distance, SELF->font);
    // the distance from the baseline to the maximal extend of any symbol above the baseline.
    dx_f32 ascender;
    dx_font_get_ascender(&ascender, SELF->font);
    // the distance from the baseline to the maximal extend of any symbol below the baseline.
    dx_f32 descender;
    dx_font_get_descender(&descender, SELF->font);
    // the distance of the baselines of two lines.
    baseline_distance = baseline_distance * 1.5f;
    dx_f32 insets_x = 16.f;
    dx_f32 insets_y = 16.f;
    // Make a viewport that starts at (0,0) and extends to (canvas_width, canvas_height).
    // The viewport width is fixed to canvas width. 
    dx_f32 viewport_width = canvas_width;
    // The viewport height is fixed to canvas height.
    dx_f32 viewport_height = canvas_height;
    // The viewport left is fixed to 0.
    dx_f32 viewport_left = 0.f;
    // The viewport bottom is fixed to 0.
    dx_f32 viewport_bottom = 0.f;

    dx_f32 max_line_height = ascender - descender;

    // when the console is closed, it starts above canvas_height.
    dx_f32 console_position_y_min = canvas_height;
    // the position of the console along the x-axis is 0.
    dx_f32 console_position_x = 0.f;
    // the width of the console is the width of the canvas.
    dx_f32 console_width = canvas_width;
    // the height of the console is a little bit less then the height of the canvas.
    dx_f32 console_height = canvas_height - 16.f * 4;
    dx_f32 console_position_y_max = canvas_height - console_height;

    dx_f32 console_position_y;
    // we lerp from position canvas_
    dx_lerp(&console_position_y, console_position_y_min, console_position_y_max, SELF->cs);

    dx_val_command* command;

    dx_val_command_list_clear(SELF->command_list);

    // set viewport command
    if (dx_val_command_create_viewport(&command, viewport_left, viewport_bottom, viewport_width, viewport_height)) {
      return DX_FAILURE;
    }
    if (dx_val_command_list_append(SELF->command_list, command)) {
      DX_UNREFERENCE(command);
      command = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(command);
    command = NULL;
    //
    // execute the commands
    if (dx_val_context_execute_commands(SELF->val_context, SELF->command_list)) {
      return DX_FAILURE;
    }
    {
      DX_MAT4 world_matrix;
      dx_mat4_set_identity(&world_matrix);
      dx_val_cbinding_set_mat4(SELF->rectangle_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
      dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
      DX_MAT4 view_matrix;
      dx_mat4_set_identity(&view_matrix);
      dx_val_cbinding_set_mat4(SELF->rectangle_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
      dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
      DX_MAT4 projection_matrix;
      dx_mat4_set_ortho(&projection_matrix, 0, canvas_width, 0, canvas_height, -1, +1);
      dx_val_cbinding_set_mat4(SELF->rectangle_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
      dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
    }
    {
      DX_RECT2_F32 target_rectangle = {
        .left = console_position_x,
        .bottom = console_position_y,
        .right = console_position_x + console_width,
        .top = console_position_y + console_height,
      };
      dx_rectangle_presenter_fill_rectangle(SELF->rectangle_presenter,
                                            &target_rectangle,
                                            0.f,
                                            &CONSOLE_BACKGROUND_COLOR);
//
      {

        dx_string_buffer* string_buffer;
        if (dx_string_buffer_clone(&string_buffer, SELF->prompt)) {
          return DX_FAILURE;
        }

        dx_f32 starty;

        // render the prompt and then the lines in reverse order
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
        dx_font_presenter_render_line_string(SELF->font_presenter, console_position_x + insets_x, starty, string, &TEXT_COLOR, SELF->font);
        DX_UNREFERENCE(string);
        string = NULL;
        starty += baseline_distance;
        dx_f32 startx = console_position_x + insets_x;
        dx_size n;
        if (dx_object_array_get_size(&n, SELF->output->lines)) {
          return DX_FAILURE;
        }
        for (dx_size i = n; i > 0; --i) {
          dx_text_line* line = NULL;
          if (dx_object_array_get_at((dx_object**)&line, SELF->output->lines, i - 1)) {
            return DX_FAILURE;
          }
          void const* bytes = NULL;
          if (dx_string_buffer_get_bytes(&bytes, SELF->output->text)) {
            return DX_FAILURE;
          }
          dx_size number_of_bytes;
          if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->output->text)) {
            return DX_FAILURE;
          }
          dx_string* string;
          if (dx_string_create(&string, ((char const*)bytes) + line->start, line->length)) {
            return DX_FAILURE;
          }
          dx_font_presenter_render_line_string(SELF->font_presenter, console_position_x + insets_x, starty, string, &TEXT_COLOR, SELF->font);
          DX_UNREFERENCE(string);
          string = NULL;
          starty += baseline_distance;
        }
      }
    }
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

static dx_result append_text(dx_default_console* SELF, dx_string* text) {
  if (dx_text_document_append_text(SELF->output, text)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @Ensure that SELF->font is pointing to a suitable font.
/// @param SELF A pointer to this console.
/// @default-return
/// @default-failure
static dx_result create_font(dx_default_console* SELF) {
  dx_string* font_file = NULL;
  if (dx_string_create(&font_file, FONT_FILE, strlen(FONT_FILE))) {
    return DX_FAILURE;
  }
  dx_font* font = NULL;
  dx_result result = dx_font_manager_get_or_create_font(&font, SELF->font_presenter->font_manager, font_file, FONT_SIZE);
  DX_UNREFERENCE(font_file);
  font_file = NULL;
  if (result) {
    return DX_FAILURE;
  }
  SELF->font = font;
  return DX_SUCCESS;
}

static void dx_default_console_destruct(dx_default_console* SELF) {
  DX_UNREFERENCE(SELF->output);
  SELF->output = NULL;

  DX_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;

  DX_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;

  DX_UNREFERENCE(SELF->command_list);
  SELF->command_list = NULL;

  DX_UNREFERENCE(SELF->font);
  SELF->font = NULL;

  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;

  DX_UNREFERENCE(SELF->prompt);
  SELF->prompt = NULL;
}

static void dx_default_console_dispatch_construct(dx_default_console_dispatch* SELF) {
  DX_CONSOLE_DISPATCH(SELF)->close = (dx_result(*)(dx_console*)) & close;
  DX_CONSOLE_DISPATCH(SELF)->on_keyboard_key_message = (dx_result(*)(dx_console*,dx_keyboard_key_msg*)) & on_keyboard_key_message;
  DX_CONSOLE_DISPATCH(SELF)->open = (dx_result(*)(dx_console*)) & open;
  DX_CONSOLE_DISPATCH(SELF)->close = (dx_result(*)(dx_console*)) & close;
  DX_CONSOLE_DISPATCH(SELF)->render = (dx_result(*)(dx_console*, dx_f32, dx_i32, dx_i32)) & render;
  DX_CONSOLE_DISPATCH(SELF)->toggle = (dx_result(*)(dx_console*)) & toggle;
  DX_CONSOLE_DISPATCH(SELF)->is_open = (dx_result(*)(dx_bool*,dx_console*)) & is_open;
  DX_CONSOLE_DISPATCH(SELF)->is_closed = (dx_result(*)(dx_bool*,dx_console*)) & is_closed;
  DX_CONSOLE_DISPATCH(SELF)->append_text = (dx_result(*)(dx_console*, dx_string*)) & append_text;
}

static void line_added_callback(dx_string** element) {
  DX_REFERENCE(*element);
}

static void line_removed_callback(dx_string** element) {
  DX_UNREFERENCE(*element);
}

dx_result dx_default_console_construct(dx_default_console* SELF, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* val_context) {
  dx_rti_type* TYPE = dx_default_console_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_console_construct(DX_CONSOLE(SELF))) {
    return DX_FAILURE;
  }
  if (dx_text_document_create(&SELF->output)) {
    return DX_FAILURE;
  }
  if (dx_string_buffer_create(&SELF->prompt)) {
    DX_UNREFERENCE(SELF->output);
    SELF->output = NULL;
    return DX_FAILURE;
  }
  if (!val_context) {
    DX_UNREFERENCE(SELF->prompt);
    SELF->prompt = NULL;
    DX_UNREFERENCE(SELF->output);
    SELF->output = NULL;
    return DX_FAILURE;
  }
  SELF->val_context = val_context;
  DX_REFERENCE(SELF->val_context);
  if (dx_val_command_list_create(&SELF->command_list)) {
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    DX_UNREFERENCE(SELF->prompt);
    SELF->prompt = NULL;
    DX_UNREFERENCE(SELF->output);
    SELF->output = NULL;
    return DX_FAILURE;
  }
  SELF->cs = 0.f;
  SELF->ts = 0.f;
  if (!font_presenter) {
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    DX_UNREFERENCE(SELF->prompt);
    SELF->prompt = NULL;
    DX_UNREFERENCE(SELF->output);
    SELF->output = NULL;
    return DX_FAILURE;
  }
  SELF->font_presenter = font_presenter;
  DX_REFERENCE(SELF->font_presenter);
  if (create_font(SELF)) {
    DX_UNREFERENCE(SELF->font_presenter);
    SELF->font_presenter = NULL;
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    DX_UNREFERENCE(SELF->prompt);
    SELF->prompt = NULL;
    DX_UNREFERENCE(SELF->output);
    SELF->output = NULL;
    return DX_FAILURE;
  }
  if (dx_rectangle_presenter_create(&SELF->rectangle_presenter, SELF->val_context)) {
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    DX_UNREFERENCE(SELF->font_presenter);
    SELF->font_presenter = NULL;
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    DX_UNREFERENCE(SELF->font);
    SELF->font = NULL;
    DX_UNREFERENCE(SELF->prompt);
    SELF->prompt = NULL;
    DX_UNREFERENCE(SELF->output);
    SELF->output = NULL;
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_default_console_create(dx_default_console** RETURN, dx_font_presenter* font_presenter, dx_font_manager* font_manager, dx_val_context* context) {
  dx_default_console* SELF = DX_DEFAULT_CONSOLE(dx_object_alloc(sizeof(dx_default_console)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_default_console_construct(SELF, font_presenter, font_manager, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
