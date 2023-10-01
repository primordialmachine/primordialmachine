#include "dx/player/run.h"

#include "dx/engine/default_console.h"
#include "dx/application.h"
#include "dx/data_definition_language.h"
#include "dx/fps_counter.h"
#include "dx/engine/console_commands.h"
#include "dx/scenes/default_scene_presenter.h"
#include <stdio.h>

#if DX_OPERATING_SYSTEM_WINDOWS == DX_OPERATING_SYSTEM
  // GetTickCount64
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("environment not (yet) supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static char const* PATHNAMES[] = {
  "./assets/scenes/day1.empty.adl",
  //
  "./assets/scenes/day2.quadriliteral.adl",
  "./assets/scenes/day2.triangle.adl",
  //
  "./assets/scenes/day3.quadriliteral.adl",
  "./assets/scenes/day3.triangle.adl",
  //
  "./assets/scenes/day4.quadriliteral.adl",
  "./assets/scenes/day4.triangle.adl",
  //
  "./assets/scenes/day5.quadriliteral.adl",
  "./assets/scenes/day5.triangle.adl",
  //
  "./assets/scenes/day6.quadriliteral.adl",
  "./assets/scenes/day6.triangle.adl",
  //
  "./assets/scenes/day7.cube.adl",
  "./assets/scenes/day7.octahedron.adl",
  //
  "./assets/scenes/day8.cube.adl",
  "./assets/scenes/day8.octahedron.adl",
  //
  "./assets/scenes/day9.cube.loaded-texture.adl",
  "./assets/scenes/day9.octahedron.loaded-texture.adl",
};

static dx_size const NUMBER_OF_PATHNAMES = sizeof(PATHNAMES) / sizeof(char const*);

static dx_result on_msg(dx_default_application_presenter* SELF, dx_msg* msg);

static dx_result startup(dx_default_application_presenter* SELF);

static dx_result run(dx_default_application_presenter* SELF);

static dx_result shutdown(dx_default_application_presenter* SELF);

static dx_result request_quit(dx_default_application_presenter* SELF);

static dx_result quit_requested(dx_bool* RETURN, dx_default_application_presenter* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result on_msg(dx_default_application_presenter* SELF, dx_msg* msg) {
  switch (dx_msg_get_flags(msg)) {
    case DX_MSG_TYPE_EMIT: {
      dx_emit_msg* emit_msg = DX_EMIT_MSG(msg);
      char const* p; dx_size n;
      if (dx_emit_msg_get(emit_msg, &p, &n)) {
        return DX_FAILURE;
      }
      dx_log(p, n);
    } break;
    case DX_MSG_TYPE_INPUT: {
      dx_input_msg* input_msg = DX_INPUT_MSG(msg);
      if (DX_INPUT_MSG_KIND_KEYBOARD_KEY == dx_input_msg_get_kind(input_msg)) {
        dx_keyboard_key_msg* keyboard_key_msg = DX_KEYBOARD_KEY_MSG(input_msg);
        dx_bool temporary;
        if (dx_console_is_open(&temporary, SELF->console)) {
          return DX_FAILURE;
        }
        if (temporary) {
          // if the circumflex key is released, we close the console.
          dx_keyboard_key_action action;
          dx_keyboard_key key;
          if (dx_keyboard_key_msg_get_action(&action, keyboard_key_msg) || dx_keyboard_key_msg_get_key(&key, keyboard_key_msg)) {
            return DX_FAILURE;
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_dead_circumflex == key) {
            dx_console_toggle(SELF->console);
          } else {
            dx_console_on_keyboard_key_message(SELF->console, keyboard_key_msg);
          }
        } else {
          dx_keyboard_key_action action;
          dx_keyboard_key key;
          if (dx_keyboard_key_msg_get_action(&action, keyboard_key_msg) || dx_keyboard_key_msg_get_key(&key, keyboard_key_msg)) {
            return DX_FAILURE;
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_return == key) {
            dx_size n;
            if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
              return DX_FAILURE;
            }
            SELF->scene_index = (SELF->scene_index + 1) % n;
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_dead_circumflex == key) {
            dx_console_open(SELF->console);
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_escape == key) {
            dx_msg* msg = NULL;
            if (dx_quit_msg_create((dx_quit_msg**)&msg)) {
              return DX_FAILURE;
            }
            if (dx_msg_queue_push(SELF->message_queue, msg)) {
              DX_UNREFERENCE(msg);
              msg = NULL;
              return DX_FAILURE;
            }
            DX_UNREFERENCE(msg);
            msg = NULL;
          }
        }
      }
    } break;
    case DX_MSG_TYPE_QUIT: {
      SELF->quit = true;
    } break;
    case DX_MSG_TYPE_UNDETERMINED:
    default: {
      return DX_SUCCESS;
    } break;
  }
  return DX_SUCCESS;
}

static dx_result get_val_context(dx_val_context** RETURN, dx_default_application_presenter* SELF) {
  if (!RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_REFERENCE(SELF->val_context);
  *RETURN = SELF->val_context;
  return DX_SUCCESS;
}

static dx_result get_console(dx_console** RETURN, dx_default_application_presenter* SELF) {
  if (!RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_REFERENCE(SELF->console);
  *RETURN = SELF->console;
  return DX_SUCCESS;
}

static dx_result get_cl_interpreter(dx_cl_interpreter** RETURN, dx_default_application_presenter* SELF) {
  if (!RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_REFERENCE(SELF->cl_interpreter);
  *RETURN = SELF->cl_interpreter;
  return DX_SUCCESS;
}

static dx_result print_info(dx_default_application_presenter* SELF) {
  static char const* cmds[] = {
    "assetLibrary.printInfo",
    "coreLibrary.printInfo",
    "dataDefinitionLanguageLibrary.printInfo",
    "documentDefinitionLanguageLibrary.printInfo",
    "engineLibrary.printInfo"
  };
  for (dx_size i = 0, n = sizeof(cmds) / sizeof(char const*); i < n; ++i) {
    dx_string* f = NULL;
    if (dx_string_create(&f, cmds[i], strlen(cmds[i]))) {
      return DX_FAILURE;
    }
    if (dx_cl_interpreter_execute(SELF->cl_interpreter, DX_APPLICATION_PRESENTER(SELF), f)) {
      DX_UNREFERENCE(f);
      f = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(f);
    f = NULL;
  }
  dx_string* f = NULL;
  if (dx_string_create(&f, "enter `help` to find out more about the console\n", sizeof("enter `help` to find out more about the console\n") - 1)) {
    return DX_FAILURE;
  }
  if (dx_console_append_text(SELF->console, f)) {
    DX_UNREFERENCE(f);
    f = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(f);
  f = NULL;
  return DX_SUCCESS;
}

static dx_result startup(dx_default_application_presenter* SELF) {
  if (print_info(SELF)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0, n = NUMBER_OF_PATHNAMES; i < n; ++i) {
    dx_string* path = NULL;
    if (dx_string_create(&path, PATHNAMES[i], strlen(PATHNAMES[i]))) {
      dx_inline_object_array_clear(SELF->scene_presenters);
      return DX_FAILURE;
    }
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_default_scene_presenter_create((dx_default_scene_presenter**)&scene_presenter, path)) {
      DX_UNREFERENCE(path);
      path = NULL;
      dx_inline_object_array_clear(SELF->scene_presenters);
      return DX_FAILURE;
    }
    DX_UNREFERENCE(path);
    path = NULL;
    if (dx_inline_object_array_append(SELF->scene_presenters, DX_OBJECT(scene_presenter))) {
      DX_UNREFERENCE(scene_presenter);
      scene_presenter = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(scene_presenter);
    scene_presenter = NULL;
  }
  //
  dx_size n;
  if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_inline_object_array_get_at((dx_object**) & scene_presenter, SELF->scene_presenters, i)) {
      /// @undefined
      return DX_FAILURE;
    }
    if (dx_scene_presenter_startup(scene_presenter, SELF->val_context)) {
      while (i > 0) {
        if (dx_inline_object_array_get_at((dx_object**)&scene_presenter, SELF->scene_presenters, --i)) {
          /// @undefined
          return DX_FAILURE;
        }
        dx_scene_presenter_shutdown(scene_presenter, SELF->val_context);
      }
      dx_inline_object_array_clear(SELF->scene_presenters);
      return DX_FAILURE;
    }
  }
  //
  return DX_SUCCESS;
}

static dx_result run(dx_default_application_presenter* SELF) {
  uint64_t last = GetTickCount64();
  uint64_t now = last;
  uint64_t delta = now - last;
  while (!SELF->quit) {
    now = GetTickCount64();
    delta = now - last;
    last = now;
    if (dx_application_update(SELF->application)) {
      return DX_FAILURE;
    }
    do {
      dx_msg* msg;
      if (dx_msg_queue_pop(&msg, SELF->message_queue)) {
        return DX_FAILURE;
      }
      if (msg) {
        if (on_msg(SELF, msg)) {
          DX_UNREFERENCE(msg);
          msg = NULL;
          return DX_FAILURE;
        }
        DX_UNREFERENCE(msg);
        msg = NULL;
      } else {
        break;
      }
    } while (true);
    dx_fps_counter_on_enter_frame(SELF->fps_counter);
    dx_val_context_enter_frame(SELF->val_context);

    // Get the canvas size.
    dx_i32 canvas_size_horizontal, canvas_size_vertical;
    if (dx_val_context_get_canvas_size(SELF->val_context, &canvas_size_horizontal, &canvas_size_vertical)) {
      dx_val_context_leave_frame(SELF->val_context);
      dx_fps_counter_on_leave_frame(SELF->fps_counter);
      return DX_FAILURE;
    }
    // Get the DPI.
    dx_i32 dpi_horizontal, dpi_vertical;
    if (dx_val_context_get_dpi(SELF->val_context, &dpi_horizontal, &dpi_vertical)) {
      dx_val_context_leave_frame(SELF->val_context);
      dx_fps_counter_on_leave_frame(SELF->fps_counter);
      return DX_FAILURE;
    }

    dx_f32 delta_seconds = ((dx_f32)delta) / 1000.f;
    dx_size n;
    if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
      return DX_FAILURE;
    }
    if (SELF->scene_index < n) {
      dx_scene_presenter* scene_presenter = NULL;
      if (dx_inline_object_array_get_at((dx_object**)&scene_presenter, SELF->scene_presenters, SELF->scene_index)) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return DX_FAILURE;
      }
      if (dx_scene_presenter_render(scene_presenter, SELF->val_context, delta_seconds, canvas_size_horizontal, canvas_size_vertical)) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return DX_FAILURE;
      }
    }
    {
      dx_string* format = NULL;
      if (dx_string_create(&format, "FPS: ${r64.2}", sizeof("FPS: ${r64.2}"))) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return DX_FAILURE;
      }
      dx_f64 value;
      if (dx_fps_counter_get_fps(&value, SELF->fps_counter)) {
        DX_UNREFERENCE(format);
        format = NULL;
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return DX_FAILURE;
      }
      dx_string* msg = dx_string_printf(format, value);
      DX_UNREFERENCE(format);
      format = NULL;
      if (!msg) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return DX_FAILURE;
      }
    #if defined(DX_WRITE_FPS_TO_LOG) && 1 == DX_WRITE_FPS_TO_LOG
      dx_log(msg->bytes, msg->number_of_bytes);
      dx_log("\n", strlen("\n"));
    #endif
      dx_overlay_clear_messages(SELF->overlay);
      dx_overlay_add_message(SELF->overlay, msg);
      DX_UNREFERENCE(msg);
      msg = NULL;
    }

    dx_console_render(SELF->console, delta_seconds, canvas_size_horizontal, canvas_size_vertical, dpi_horizontal, dpi_vertical);
    dx_overlay_render(SELF->overlay, delta_seconds, canvas_size_horizontal, canvas_size_vertical, dpi_horizontal, dpi_vertical);
    dx_val_context_leave_frame(SELF->val_context);
    dx_fps_counter_on_leave_frame(SELF->fps_counter);
  }
  return DX_SUCCESS;
}

static dx_result shutdown(dx_default_application_presenter* SELF) {
  dx_size n;
  if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
    return DX_FAILURE;
  }
  for (dx_size i = n; i > 0; --i) {
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_inline_object_array_get_at((dx_object**)&scene_presenter, SELF->scene_presenters, i - 1)) {
      /// @undefined
      return DX_FAILURE;
    }
    dx_scene_presenter_shutdown(scene_presenter, SELF->val_context);
  }
  dx_inline_object_array_clear(SELF->scene_presenters);
  return DX_SUCCESS;
}

static dx_result request_quit(dx_default_application_presenter* SELF) {
  SELF->quit = true;
  return DX_SUCCESS;
}

static dx_result quit_requested(dx_bool* RETURN, dx_default_application_presenter* SELF) {
  *RETURN = SELF->quit;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.default_application_presenter",
                      dx_default_application_presenter,
                      dx_application_presenter);

static void dx_default_application_presenter_destruct(dx_default_application_presenter* SELF) {
  SELF->message_queue = NULL;
  DX_UNREFERENCE(SELF->cl_interpreter);
  SELF->cl_interpreter = NULL;
  DX_UNREFERENCE(SELF->application);
  SELF->application = NULL;
  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
  DX_UNREFERENCE(SELF->overlay);
  SELF->overlay = NULL;
  DX_UNREFERENCE(SELF->console);
  SELF->console = NULL;
  DX_UNREFERENCE(SELF->fps_counter);
  SELF->fps_counter = NULL;
  dx_inline_object_array_uninitialize(SELF->scene_presenters);
  dx_memory_deallocate(SELF->scene_presenters);
}

static void dx_default_application_presenter_dispatch_construct(dx_default_application_presenter_dispatch* SELF) {
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_val_context = (dx_result(*)(dx_val_context**, dx_application_presenter*)) & get_val_context;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->run = (dx_result(*)(dx_application_presenter*)) & run;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->shutdown = (dx_result(*)(dx_application_presenter*)) &shutdown;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->startup = (dx_result(*)(dx_application_presenter*)) &startup;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_console = (dx_result(*)(dx_console**, dx_application_presenter*)) & get_console;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_cl_interpreter = (dx_result(*)(dx_cl_interpreter**, dx_application_presenter*)) & get_cl_interpreter;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->request_quit = (dx_result(*)(dx_application_presenter*)) & request_quit;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->quit_requested = (dx_result(*)(dx_bool*, dx_application_presenter*)) & quit_requested;
}

dx_result dx_default_application_presenter_construct(dx_default_application_presenter* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter, dx_application* application, dx_cl_interpreter* cl_interpreter, dx_msg_queue* message_queue, dx_val_context* val_context) {
  dx_rti_type* TYPE = dx_default_application_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_application_presenter_construct(DX_APPLICATION_PRESENTER(SELF))) {
    return DX_FAILURE;
  }
  SELF->scene_presenters = NULL;
  if (dx_memory_allocate(&SELF->scene_presenters, sizeof(dx_inline_object_array))) {
    return DX_FAILURE;
  }
  if (dx_inline_object_array_initialize(SELF->scene_presenters, 0)) {
    dx_memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return DX_FAILURE;
  }
  SELF->scene_index = 0;
  SELF->quit = false;
  if (dx_default_fps_counter_create((dx_default_fps_counter**)&SELF->fps_counter)) {
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    dx_memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return DX_FAILURE;
  }
  if (dx_default_console_create((dx_default_console**)&SELF->console, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF->fps_counter);
    SELF->fps_counter = NULL;
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    dx_memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return DX_FAILURE;
  }
  if (dx_overlay_create(&SELF->overlay, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF->console);
    SELF->console = NULL;
    DX_UNREFERENCE(SELF->fps_counter);
    SELF->fps_counter = NULL;
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    dx_memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return DX_FAILURE;
  }
  //
  SELF->application = application;
  DX_REFERENCE(SELF->application);
  SELF->val_context = val_context;
  DX_REFERENCE(SELF->val_context);
  SELF->cl_interpreter = cl_interpreter;
  DX_REFERENCE(SELF->cl_interpreter);
  //
  SELF->message_queue = message_queue;
  //
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_default_application_presenter_create(dx_default_application_presenter** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter, dx_application* application, dx_cl_interpreter* cl_interpreter, dx_msg_queue* message_queue, dx_val_context* val_context) {
  DX_CREATE_PREFIX(dx_default_application_presenter)
  if (dx_default_application_presenter_construct(SELF, font_presenter, rectangle_presenter, application, cl_interpreter, message_queue, val_context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
