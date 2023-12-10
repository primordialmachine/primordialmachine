#include "dx/player/run.h"

#include "dx/engine/default_console.h"
#include "dx/application.h"
#include "dx/data_definition_language.h"
#include "dx/fps_counter.h"
#include "dx/engine/console_commands.h"
#include "dx/scenes/default_scene_presenter.h"
#include <stdio.h>

#if Core_OperatingSystem_Windows == Core_OperatingSystem
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

static Core_Size const NUMBER_OF_PATHNAMES = sizeof(PATHNAMES) / sizeof(char const*);

static Core_Result on_msg(dx_default_application_presenter* SELF, Core_Message* msg);

static Core_Result startup(dx_default_application_presenter* SELF);

static Core_Result run(dx_default_application_presenter* SELF);

static Core_Result shutdown(dx_default_application_presenter* SELF);

static Core_Result request_quit(dx_default_application_presenter* SELF);

static Core_Result quit_requested(Core_Boolean* RETURN, dx_default_application_presenter* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result on_msg(dx_default_application_presenter* SELF, Core_Message* msg) {
  Core_Natural32 msg_flags;
  if (Core_Message_getFlags(&msg_flags, msg)) {
    return Core_Failure;
  }
  switch (msg_flags) {
    case DX_MSG_TYPE_EMIT: {
      dx_emit_msg* emit_msg = DX_EMIT_MSG(msg);
      char const* p; Core_Size n;
      if (dx_emit_msg_get(emit_msg, &p, &n)) {
        return Core_Failure;
      }
      dx_log(p, n);
    } break;
    case DX_MSG_TYPE_INPUT: {
      dx_input_msg* input_msg = DX_INPUT_MSG(msg);
      if (DX_INPUT_MSG_KIND_KEYBOARD_KEY == dx_input_msg_get_kind(input_msg)) {
        dx_keyboard_key_msg* keyboard_key_msg = DX_KEYBOARD_KEY_MSG(input_msg);
        Core_Boolean temporary;
        if (dx_console_is_open(&temporary, SELF->console)) {
          return Core_Failure;
        }
        if (temporary) {
          // if the circumflex key is released, we close the console.
          dx_keyboard_key_action action;
          dx_keyboard_key key;
          if (dx_keyboard_key_msg_get_action(&action, keyboard_key_msg) || dx_keyboard_key_msg_get_key(&key, keyboard_key_msg)) {
            return Core_Failure;
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
            return Core_Failure;
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_return == key) {
            Core_Size n;
            if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
              return Core_Failure;
            }
            SELF->scene_index = (SELF->scene_index + 1) % n;
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_dead_circumflex == key) {
            dx_console_open(SELF->console);
          }
          if (DX_KEYBOARD_KEY_ACTION_RELEASED == action && dx_keyboard_key_escape == key) {
            Core_Message* msg = NULL;
            if (dx_quit_msg_create((dx_quit_msg**)&msg)) {
              return Core_Failure;
            }
            if (dx_msg_queue_push(SELF->message_queue, msg)) {
              DX_UNREFERENCE(msg);
              msg = NULL;
              return Core_Failure;
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
      return Core_Success;
    } break;
  }
  return Core_Success;
}

static Core_Result get_console(dx_console** RETURN, dx_default_application_presenter* SELF) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(SELF->console);
  *RETURN = SELF->console;
  return Core_Success;
}

static Core_Result get_cl_interpreter(dx_cl_interpreter** RETURN, dx_default_application_presenter* SELF) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(SELF->cl_interpreter);
  *RETURN = SELF->cl_interpreter;
  return Core_Success;
}

static Core_Result print_info(dx_default_application_presenter* SELF) {
  static char const* cmds[] = {
    "assetLibrary.printInfo",
    "coreLibrary.printInfo",
    "dataDefinitionLanguageLibrary.printInfo",
    "documentDefinitionLanguageLibrary.printInfo",
    "engineLibrary.printInfo"
  };
  for (Core_Size i = 0, n = sizeof(cmds) / sizeof(char const*); i < n; ++i) {
    Core_String* f = NULL;
    if (Core_String_create(&f, cmds[i], strlen(cmds[i]))) {
      return Core_Failure;
    }
    if (dx_cl_interpreter_execute(SELF->cl_interpreter, DX_APPLICATION_PRESENTER(SELF), f)) {
      DX_UNREFERENCE(f);
      f = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(f);
    f = NULL;
  }
  Core_String* f = NULL;
  if (Core_String_create(&f, "enter `help` to find out more about the console\n", sizeof("enter `help` to find out more about the console\n") - 1)) {
    return Core_Failure;
  }
  if (dx_console_append_output_text(SELF->console, f)) {
    DX_UNREFERENCE(f);
    f = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(f);
  f = NULL;
  return Core_Success;
}

static Core_Result startup(dx_default_application_presenter* SELF) {
  if (print_info(SELF)) {
    return Core_Failure;
  }
  for (Core_Size i = 0, n = NUMBER_OF_PATHNAMES; i < n; ++i) {
    Core_String* path = NULL;
    if (Core_String_create(&path, PATHNAMES[i], strlen(PATHNAMES[i]))) {
      dx_inline_object_array_clear(SELF->scene_presenters);
      return Core_Failure;
    }
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_default_scene_presenter_create((dx_default_scene_presenter**)&scene_presenter, path)) {
      DX_UNREFERENCE(path);
      path = NULL;
      dx_inline_object_array_clear(SELF->scene_presenters);
      return Core_Failure;
    }
    DX_UNREFERENCE(path);
    path = NULL;
    if (dx_inline_object_array_append(SELF->scene_presenters, CORE_OBJECT(scene_presenter))) {
      DX_UNREFERENCE(scene_presenter);
      scene_presenter = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(scene_presenter);
    scene_presenter = NULL;
  }
  //
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_inline_object_array_get_at((Core_Object**) & scene_presenter, SELF->scene_presenters, i)) {
      /// @undefined
      return Core_Failure;
    }
    if (dx_scene_presenter_startup(scene_presenter, SELF->val_context)) {
      while (i > 0) {
        if (dx_inline_object_array_get_at((Core_Object**)&scene_presenter, SELF->scene_presenters, --i)) {
          /// @undefined
          return Core_Failure;
        }
        dx_scene_presenter_shutdown(scene_presenter, SELF->val_context);
      }
      dx_inline_object_array_clear(SELF->scene_presenters);
      return Core_Failure;
    }
  }
  //
  return Core_Success;
}

static Core_Result run(dx_default_application_presenter* SELF) {
  uint64_t last = GetTickCount64();
  uint64_t now = last;
  uint64_t delta = now - last;
  while (!SELF->quit) {
    now = GetTickCount64();
    delta = now - last;
    last = now;
    if (dx_application_update(SELF->application)) {
      return Core_Failure;
    }
    do {
      Core_Message* msg;
      if (dx_msg_queue_pop(&msg, SELF->message_queue)) {
        return Core_Failure;
      }
      if (msg) {
        if (on_msg(SELF, msg)) {
          DX_UNREFERENCE(msg);
          msg = NULL;
          return Core_Failure;
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
    Core_Integer32 canvas_size_horizontal, canvas_size_vertical;
    if (dx_val_context_get_canvas_size(SELF->val_context, &canvas_size_horizontal, &canvas_size_vertical)) {
      dx_val_context_leave_frame(SELF->val_context);
      dx_fps_counter_on_leave_frame(SELF->fps_counter);
      return Core_Failure;
    }
    // Get the DPI.
    Core_Integer32 dpi_horizontal, dpi_vertical;
    if (dx_val_context_get_canvas_dpi(SELF->val_context, &dpi_horizontal, &dpi_vertical)) {
      dx_val_context_leave_frame(SELF->val_context);
      dx_fps_counter_on_leave_frame(SELF->fps_counter);
      return Core_Failure;
    }

    Core_Real32 delta_seconds = ((Core_Real32)delta) / 1000.f;
    Core_Size n;
    if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
      return Core_Failure;
    }
    if (SELF->scene_index < n) {
      dx_scene_presenter* scene_presenter = NULL;
      if (dx_inline_object_array_get_at((Core_Object**)&scene_presenter, SELF->scene_presenters, SELF->scene_index)) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return Core_Failure;
      }
      if (dx_scene_presenter_render(scene_presenter, SELF->val_context, delta_seconds, canvas_size_horizontal, canvas_size_vertical)) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return Core_Failure;
      }
    }
    {
      Core_String* format = NULL;
      if (Core_String_create(&format, "FPS: ${r64.2}", sizeof("FPS: ${r64.2}"))) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return Core_Failure;
      }
      Core_Real64 value;
      if (dx_fps_counter_get_fps(&value, SELF->fps_counter)) {
        DX_UNREFERENCE(format);
        format = NULL;
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return Core_Failure;
      }
      Core_String* msg = NULL;
      if (Core_String_printf(&msg, format, value)) {
        DX_UNREFERENCE(format);
        format = NULL;
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return Core_Failure;
      }
      DX_UNREFERENCE(format);
      format = NULL;
    #if 0
      if (!msg) {
        dx_val_context_leave_frame(SELF->val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        return Core_Failure;
      }
    #endif
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
  return Core_Success;
}

static Core_Result shutdown(dx_default_application_presenter* SELF) {
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
    return Core_Failure;
  }
  for (Core_Size i = n; i > 0; --i) {
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_inline_object_array_get_at((Core_Object**)&scene_presenter, SELF->scene_presenters, i - 1)) {
      /// @undefined
      return Core_Failure;
    }
    dx_scene_presenter_shutdown(scene_presenter, SELF->val_context);
  }
  dx_inline_object_array_clear(SELF->scene_presenters);
  return Core_Success;
}

static Core_Result request_quit(dx_default_application_presenter* SELF) {
  SELF->quit = true;
  return Core_Success;
}

static Core_Result quit_requested(Core_Boolean* RETURN, dx_default_application_presenter* SELF) {
  *RETURN = SELF->quit;
  return Core_Success;
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
  Core_Memory_deallocate(SELF->scene_presenters);
}

static void dx_default_application_presenter_constructDispatch(dx_default_application_presenter_dispatch* SELF) {
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->run = (Core_Result(*)(dx_application_presenter*)) & run;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->shutdown = (Core_Result(*)(dx_application_presenter*)) &shutdown;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->startup = (Core_Result(*)(dx_application_presenter*)) &startup;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_console = (Core_Result(*)(dx_console**, dx_application_presenter*)) & get_console;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_cl_interpreter = (Core_Result(*)(dx_cl_interpreter**, dx_application_presenter*)) & get_cl_interpreter;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->request_quit = (Core_Result(*)(dx_application_presenter*)) & request_quit;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->quit_requested = (Core_Result(*)(Core_Boolean*, dx_application_presenter*)) & quit_requested;
}

Core_Result dx_default_application_presenter_construct(dx_default_application_presenter* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter, dx_application* application, dx_cl_interpreter* cl_interpreter, dx_msg_queue* message_queue, dx_val_context* val_context) {
  DX_CONSTRUCT_PREFIX(dx_default_application_presenter);
  if (dx_application_presenter_construct(DX_APPLICATION_PRESENTER(SELF))) {
    return Core_Failure;
  }
  SELF->scene_presenters = NULL;
  if (Core_Memory_allocate(&SELF->scene_presenters, sizeof(dx_inline_object_array))) {
    return Core_Failure;
  }
  if (dx_inline_object_array_initialize(SELF->scene_presenters, 0)) {
    Core_Memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return Core_Failure;
  }
  SELF->scene_index = 0;
  SELF->quit = false;
  if (dx_default_fps_counter_create((dx_default_fps_counter**)&SELF->fps_counter)) {
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    Core_Memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return Core_Failure;
  }
  if (dx_default_console_create((dx_default_console**)&SELF->console, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF->fps_counter);
    SELF->fps_counter = NULL;
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    Core_Memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return Core_Failure;
  }
  if (dx_overlay_create(&SELF->overlay, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF->console);
    SELF->console = NULL;
    DX_UNREFERENCE(SELF->fps_counter);
    SELF->fps_counter = NULL;
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    Core_Memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return Core_Failure;
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
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_default_application_presenter_create(dx_default_application_presenter** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter, dx_application* application, dx_cl_interpreter* cl_interpreter, dx_msg_queue* message_queue, dx_val_context* val_context) {
  DX_CREATE_PREFIX(dx_default_application_presenter);
  if (dx_default_application_presenter_construct(SELF, font_presenter, rectangle_presenter, application, cl_interpreter, message_queue, val_context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
