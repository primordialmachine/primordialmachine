#include "dx/player/run.h"

#include "dx/engine/default_console.h"
#include "dx/application.h"
#include "dx/data_definition_language.h"
#include "dx/fps_counter.h"
#include "dx/engine/console_commands.h"
#include "dx/scenes/default_scene_presenter.h"
#include "dx/ui/manager.h"
#include <stdio.h>
#include <string.h>

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

static Core_Result onApplicationMessage(dx_default_application_presenter * SELF, Core_ApplicationMessage* applicationMessage) {
  if (applicationMessage->kind == Core_ApplicationMessageKind_QuitRequested) {
    SELF->quit = true;
  }
  return Core_Success;
}

static Core_Result onEmitMessage(dx_default_application_presenter * SELF, Core_EmitMessage * emitMessage) {
  Core_String* text = NULL;
  if (Core_EmitMessage_get(&text, emitMessage)) {
    return Core_Failure;
  }
  void* bytes = NULL;
  if (Core_String_getBytes(&bytes, text)) {
    return Core_Failure;
  }
  Core_Size numberOfBytes = 0;
  if (Core_String_getNumberOfBytes(&numberOfBytes, text)) {
    return Core_Failure;
  }
  dx_log(bytes, numberOfBytes);
  return Core_Success;
}

static Core_Result onInputMessage(dx_default_application_presenter* SELF, Core_InputMessage* message) {
  Core_InputMessageKind kind;
  if (Core_InputMessage_getKind(&kind, message)) {
    return Core_Failure;
  }
  if (Core_InputMessageKind_KeyboardKey == kind) {
    Core_KeyboardKeyMessage* keyboard_key_msg = CORE_KEYBOARDKEYMESSAGE(message);
    Core_Boolean temporary;
    if (dx_console_is_open(&temporary, SELF->console)) {
      return Core_Failure;
    }
    if (temporary) {
      // if the circumflex key is released, we close the console.
      Core_KeyboardKeyAction action;
      Core_KeyboardKey key;
      if (Core_KeyboardKeyMessage_getAction(&action, keyboard_key_msg) || Core_KeyboardKeyMessage_getKey(&key, keyboard_key_msg)) {
        return Core_Failure;
      }
      if (Core_KeyboardKeyAction_Released == action && Core_KeyboardKey_DeadCircumflex == key) {
        dx_console_toggle(SELF->console);
      } else {
        dx_console_on_keyboard_key_message(SELF->console, keyboard_key_msg);
      }
    } else {
      Core_KeyboardKeyAction action;
      Core_KeyboardKey key;
      if (Core_KeyboardKeyMessage_getAction(&action, keyboard_key_msg) || Core_KeyboardKeyMessage_getKey(&key, keyboard_key_msg)) {
        return Core_Failure;
      }
      if (Core_KeyboardKeyAction_Released == action && Core_KeyboardKey_Return == key) {
        Core_Size n;
        if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
          return Core_Failure;
        }
        SELF->scene_index = (SELF->scene_index + 1) % n;
      }
      if (Core_KeyboardKeyAction_Released == action && Core_KeyboardKey_DeadCircumflex == key) {
        dx_console_open(SELF->console);
      }
      if (Core_KeyboardKeyAction_Released == action && Core_KeyboardKey_Escape == key) {
        Core_Message* applicationMessage = NULL;
        if (Core_ApplicationMessage_create((Core_ApplicationMessage**)&applicationMessage, Core_ApplicationMessageKind_QuitRequested)) {
          return Core_Failure;
        }
        if (Core_MessageQueue_push(SELF->message_queue, applicationMessage)) {
          CORE_UNREFERENCE(applicationMessage);
          applicationMessage = NULL;
          return Core_Failure;
        }
        CORE_UNREFERENCE(applicationMessage);
        applicationMessage = NULL;
      }
    }
  }
  return Core_Success;
}

static Core_Result on_msg(dx_default_application_presenter* SELF, Core_Message* message) {
  Core_Boolean result;
  //
  Core_Type* inputMsgType = NULL;
  if (Core_InputMessage_getType(&inputMsgType)) {
    return Core_Failure;
  }
  //
  Core_Type* emitMessageType = NULL;
  if (Core_EmitMessage_getType(&emitMessageType)) {
    return Core_Failure;
  }
  //
  Core_Type* applicationMessageType = NULL;
  if (Core_ApplicationMessage_getType(&applicationMessageType)) {
    return Core_Failure;
  }
  //
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(message)->type, inputMsgType)) {
    return Core_Failure;
  }
  if (result) {
    Core_InputMessage* inputMessage = CORE_INPUTMESSAGE(message);
    if (onInputMessage(SELF, inputMessage)) {
      return Core_Failure;
    }
    return Core_Success;
  }
  //
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(message)->type, emitMessageType)) {
    return Core_Failure;
  }
  if (result) {
    Core_EmitMessage* emitMessage = CORE_EMITMESSAGE(message);
    if (onEmitMessage(SELF, emitMessage)) {
      return Core_Failure;
    }
  }
  //
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(message)->type, applicationMessageType)) {
    return Core_Failure;
  }
  if (result) {
    Core_ApplicationMessage* applicationMessage = CORE_APPLICATIONMESSAGE(message);
    if (onApplicationMessage(SELF, applicationMessage)) {
      return Core_Failure;
    }
  }
  //
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
  CORE_REFERENCE(SELF->console);
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
  CORE_REFERENCE(SELF->cl_interpreter);
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
      CORE_UNREFERENCE(f);
      f = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(f);
    f = NULL;
  }
  Core_String* f = NULL;
  if (Core_String_create(&f, "enter `help` to find out more about the console\n", sizeof("enter `help` to find out more about the console\n") - 1)) {
    return Core_Failure;
  }
  if (dx_console_append_output_text(SELF->console, f)) {
    CORE_UNREFERENCE(f);
    f = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(f);
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
      CORE_UNREFERENCE(path);
      path = NULL;
      dx_inline_object_array_clear(SELF->scene_presenters);
      return Core_Failure;
    }
    CORE_UNREFERENCE(path);
    path = NULL;
    if (dx_inline_object_array_append(SELF->scene_presenters, CORE_OBJECT(scene_presenter))) {
      CORE_UNREFERENCE(scene_presenter);
      scene_presenter = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(scene_presenter);
    scene_presenter = NULL;
  }
  //
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
    return Core_Failure;
  }

  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, SELF->application)) {
    return Core_Failure;
  }

  for (Core_Size i = 0; i < n; ++i) {
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_inline_object_array_get_at((Core_Object**)&scene_presenter, SELF->scene_presenters, i)) {
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      /// @undefined
      return Core_Failure;
    }
    if (dx_scene_presenter_startup(scene_presenter, val_context)) {
      while (i > 0) {
        if (dx_inline_object_array_get_at((Core_Object**)&scene_presenter, SELF->scene_presenters, --i)) {
          CORE_UNREFERENCE(val_context);
          val_context = NULL;
          /// @undefined
          return Core_Failure;
        }
        dx_scene_presenter_shutdown(scene_presenter, val_context);
      }
      dx_inline_object_array_clear(SELF->scene_presenters);
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  //
  return Core_Success;
}

// Add to the overloay a message informing about the frames per second (FPS).
static Core_Result addFpsOverlayMessage(dx_default_application_presenter* SELF) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "\nFPS: ${r64.2}", sizeof("\nFPS: ${r64.2}") - 1)) {
    return Core_Failure;
  }
  Core_Real64 value;
  if (dx_fps_counter_get_fps(&value, SELF->fps_counter)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  Core_String* message = NULL;
  if (Core_String_printf(&message, format, value)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  dx_overlay_add_message(SELF->overlay, message);
  return Core_Success;
}

// Add to the overlay a message informing about the canvas size.
static Core_Result addCanvasSizeOverlayMessage(dx_default_application_presenter* SELF) {
  dx_val_context* valContext = NULL;
  if (dx_application_get_val_context(&valContext, SELF->application)) {
    return Core_Failure;
  }
  Core_Integer32 horizontal, vertical;
  if (dx_val_context_get_canvas_size(valContext, &horizontal, &vertical)) {
    CORE_UNREFERENCE(valContext);
    valContext = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(valContext);
  valContext = NULL;

  Core_String* format = NULL;
  if (Core_String_create(&format, "\nCanvas Size:\n  Horizontal: ${i32}\n  Vertical: ${i32}", sizeof("\nCanvas Size:\n  Horizontal: ${i32}\n  Vertical: ${i32}") - 1)) {
    return Core_Failure;
  }
  Core_String* message = NULL;
  if (Core_String_printf(&message, format, horizontal, vertical)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  dx_overlay_add_message(SELF->overlay, message);

  return Core_Success;
}

// Add to the overlay a message informing about the canvas dpi.
static Core_Result addCanvasDpiOverlayMessage(dx_default_application_presenter* SELF) {
  dx_val_context* valContext = NULL;
  if (dx_application_get_val_context(&valContext, SELF->application)) {
    return Core_Failure;
  }
  Core_Integer32 horizontal, vertical;
  if (dx_val_context_get_canvas_dpi(valContext, &horizontal, &vertical)) {
    CORE_UNREFERENCE(valContext);
    valContext = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(valContext);
  valContext = NULL;

  Core_String* format = NULL;
  if (Core_String_create(&format, "\nCanvas DPI:\n  Horizontal: ${i32}\n  Vertical: ${i32}", sizeof("\nCanvas DPI:\n  Horizontal: ${i32}\n  Vertical: ${i32}") - 1)) {
    return Core_Failure;
  }
  Core_String* message = NULL;
  if (Core_String_printf(&message, format, horizontal, vertical)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  dx_overlay_add_message(SELF->overlay, message);

  return Core_Success;
}

static Core_Result addDpiOverlayMessage(dx_default_application_presenter* SELF) {
  dx_val_context* valContext = NULL;
  if (dx_application_get_val_context(&valContext, SELF->application)) {
    return Core_Failure;
  } 
  Core_Integer32 dpiHorizontal, dpiVertical;
  if (dx_val_context_get_canvas_dpi(valContext, &dpiHorizontal, &dpiVertical)) {
    CORE_UNREFERENCE(valContext);
    valContext = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(valContext);
  valContext = NULL;
  return Core_Success;
}

static Core_Result updateOverlay(dx_default_application_presenter* SELF) {
  dx_overlay_clear_messages(SELF->overlay);
  if (addFpsOverlayMessage(SELF)) {
    return Core_Failure;
  }
  if (addCanvasSizeOverlayMessage(SELF)) {
    return Core_Failure;
  }
  if (addCanvasDpiOverlayMessage(SELF)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result run(dx_default_application_presenter* SELF) {
  Core_Natural64 last;
  if (Core_getNow(&last)) {
    return Core_Failure;
  }
  Core_Natural64 now = last;
  Core_Natural64 delta = now - last;

  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, SELF->application)) {
    return Core_Failure;
  }

  while (!SELF->quit) {
    if (Core_getNow(&now)) {
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      return Core_Failure;
    }
    delta = now - last;
    last = now;
    if (dx_application_update(SELF->application)) {
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      return Core_Failure;
    }
    do {
      Core_Message* msg;
      if (Core_MessageQueue_pop(&msg, SELF->message_queue)) {
        CORE_UNREFERENCE(val_context);
        val_context = NULL;
        return Core_Failure;
      }
      if (msg) {
        if (on_msg(SELF, msg)) {
          CORE_UNREFERENCE(msg);
          msg = NULL;
          CORE_UNREFERENCE(val_context);
          val_context = NULL;
          return Core_Failure;
        }
        CORE_UNREFERENCE(msg);
        msg = NULL;
      } else {
        break;
      }
    } while (true);
    dx_fps_counter_on_enter_frame(SELF->fps_counter);
    dx_val_context_enter_frame(val_context);

    // Get the canvas size.
    Core_Integer32 canvas_size_horizontal, canvas_size_vertical;
    if (dx_val_context_get_canvas_size(val_context, &canvas_size_horizontal, &canvas_size_vertical)) {
      dx_val_context_leave_frame(val_context);
      dx_fps_counter_on_leave_frame(SELF->fps_counter);
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      return Core_Failure;
    }
    // Get the DPI.
    Core_Integer32 dpi_horizontal, dpi_vertical;
    if (dx_val_context_get_canvas_dpi(val_context, &dpi_horizontal, &dpi_vertical)) {
      dx_val_context_leave_frame(val_context);
      dx_fps_counter_on_leave_frame(SELF->fps_counter);
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
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
        dx_val_context_leave_frame(val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        CORE_UNREFERENCE(val_context);
        val_context = NULL;
        return Core_Failure;
      }
      if (dx_scene_presenter_render(scene_presenter, val_context, delta_seconds, canvas_size_horizontal, canvas_size_vertical)) {
        dx_val_context_leave_frame(val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        CORE_UNREFERENCE(val_context);
        val_context = NULL;
        return Core_Failure;
      }
    }
    {
      if (updateOverlay(SELF)) {
        dx_val_context_leave_frame(val_context);
        dx_fps_counter_on_leave_frame(SELF->fps_counter);
        CORE_UNREFERENCE(val_context);
        val_context = NULL;
        return Core_Failure;
      }
    }
    dx_console_render(SELF->console, delta_seconds, canvas_size_horizontal, canvas_size_vertical, dpi_horizontal, dpi_vertical);
    dx_overlay_render(SELF->overlay, delta_seconds, canvas_size_horizontal, canvas_size_vertical, dpi_horizontal, dpi_vertical);
    dx_val_context_leave_frame(val_context);
    dx_fps_counter_on_leave_frame(SELF->fps_counter);
  }
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  return Core_Success;
}

static Core_Result shutdown(dx_default_application_presenter* SELF) {
  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, SELF->application)) {
    return Core_Failure;
  }  
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, SELF->scene_presenters)) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  for (Core_Size i = n; i > 0; --i) {
    dx_scene_presenter* scene_presenter = NULL;
    if (dx_inline_object_array_get_at((Core_Object**)&scene_presenter, SELF->scene_presenters, i - 1)) {
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      /// @undefined
      return Core_Failure;
    }
    dx_scene_presenter_shutdown(scene_presenter, val_context);
  }
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
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

Core_defineObjectType("dx.default_application_presenter",
                      dx_default_application_presenter,
                      dx_application_presenter);

static Core_Result create_standard_ui(dx_default_application_presenter* SELF, dx_application* application) {
  //
  if (dx_ui_manager_create(&SELF->ui_manager, application->font_presenter, application->rectangle_presenter)) {
    return Core_Failure;
  }
  //
  if (dx_default_console_create((dx_default_console**)&SELF->console, SELF->ui_manager)) {
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  //
  if (dx_overlay_create(&SELF->overlay, SELF->ui_manager)) {
    CORE_UNREFERENCE(SELF->console);
    SELF->console = NULL;
    CORE_UNREFERENCE(SELF->ui_manager);
    SELF->ui_manager = NULL;
    return Core_Failure;
  }
  //
  return Core_Success;
}

static void destroy_standard_ui(dx_default_application_presenter* SELF) {
  //
  CORE_UNREFERENCE(SELF->overlay);
  SELF->overlay = NULL;
  //
  CORE_UNREFERENCE(SELF->console);
  SELF->console = NULL;
  //
  //
  CORE_UNREFERENCE(SELF->ui_manager);
  SELF->ui_manager = NULL;
}

static void dx_default_application_presenter_destruct(dx_default_application_presenter* SELF) {
  //
  SELF->message_queue = NULL;
  //
  CORE_UNREFERENCE(SELF->cl_interpreter);
  SELF->cl_interpreter = NULL;
  //
  destroy_standard_ui(SELF);
  //
  CORE_UNREFERENCE(SELF->fps_counter);
  SELF->fps_counter = NULL;
  //
  dx_inline_object_array_uninitialize(SELF->scene_presenters);
  Core_Memory_deallocate(SELF->scene_presenters);
  //
  CORE_UNREFERENCE(SELF->application);
  SELF->application = NULL;
}

static void dx_default_application_presenter_constructDispatch(dx_default_application_presenter_Dispatch* SELF) {
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->run = (Core_Result(*)(dx_application_presenter*)) & run;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->shutdown = (Core_Result(*)(dx_application_presenter*)) &shutdown;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->startup = (Core_Result(*)(dx_application_presenter*)) &startup;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_console = (Core_Result(*)(dx_console**, dx_application_presenter*)) & get_console;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->get_cl_interpreter = (Core_Result(*)(dx_cl_interpreter**, dx_application_presenter*)) & get_cl_interpreter;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->request_quit = (Core_Result(*)(dx_application_presenter*)) & request_quit;
  DX_APPLICATION_PRESENTER_DISPATCH(SELF)->quit_requested = (Core_Result(*)(Core_Boolean*, dx_application_presenter*)) & quit_requested;
}

Core_Result dx_default_application_presenter_construct(dx_default_application_presenter* SELF, dx_application* application, dx_cl_interpreter* cl_interpreter, Core_MessageQueue* message_queue) {
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
  if (create_standard_ui(SELF, application)) {
    CORE_UNREFERENCE(SELF->fps_counter);
    SELF->fps_counter = NULL;
    dx_inline_object_array_uninitialize(SELF->scene_presenters);
    Core_Memory_deallocate(SELF->scene_presenters);
    SELF->scene_presenters = NULL;
    return Core_Failure;
  }
  //
  SELF->application = application;
  CORE_REFERENCE(SELF->application);
  //
  SELF->cl_interpreter = cl_interpreter;
  CORE_REFERENCE(SELF->cl_interpreter);
  //
  SELF->message_queue = message_queue;
  //
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_default_application_presenter_create(dx_default_application_presenter** RETURN, dx_application* application, dx_cl_interpreter* cl_interpreter, Core_MessageQueue* message_queue) {
  DX_CREATE_PREFIX(dx_default_application_presenter);
  if (dx_default_application_presenter_construct(SELF, application, cl_interpreter, message_queue)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
