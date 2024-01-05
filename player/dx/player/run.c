#include "dx/player/run.h"

#include "dx/font_manager.h"
#include "dx/application.h"
#include "dx/scenes/default_scene_presenter.h"
#include "dx/fps_counter.h"
#include <stdio.h>

#include "dx/data_definition_language.h"

#if Core_VisualsBackend_OpenGl4 == Core_VisualsBackend
  #include "dx/val/gl.h"
#elif Core_VisualsBackend_Direct3d12 == Core_VisualsBackend
  #include "dx/val/dx12.h"
#else
  #error ("environment not (yet) supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

#define DX_WRITE_FPS_TO_LOG (0)

#define DX_TRACE (0)

#if !defined(DX_TRACE) || (1 != DX_TRACE && 0 != DX_TRACE)
  #error("DX_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

static Core_Result dx_player_create_application(dx_application** RETURN, dx_msg_queue* msg_queue);

#if Core_VisualsBackend_OpenGl4 == Core_VisualsBackend
  #include "dx/val/gl.h"
#else
  #error("environment not (yet) supported")
#endif

#if Core_AudialsBackend_OpenAl == Core_AudialsBackend
  #include "dx/aal/al/system_factory.h"
#else
  #error("environment not (yet) supported")
#endif

static Core_Result dx_player_create_application(dx_application** RETURN, dx_msg_queue* msg_queue) {
#if Core_VisualsBackend_OpenGl4 == Core_VisualsBackend && Core_OperatingSystem_Windows == Core_OperatingSystem
  dx_val_system_factory* val_system_factory = NULL;
  if (dx_val_gl_wgl_system_factory_create((dx_val_gl_wgl_system_factory**)&val_system_factory)) {
    return Core_Failure;
  }
#else
  #error("environment not (yet) supported")
#endif
#if Core_AudialsBackend_OpenAl == Core_AudialsBackend
  dx_aal_system_factory* aal_system_factory = NULL;
  if (dx_aal_al_system_factory_create((dx_aal_al_system_factory**)&aal_system_factory)) {
    DX_UNREFERENCE(val_system_factory);
    val_system_factory = NULL;
    return Core_Failure;
  }
#else
  #error("environment not (yet) supported")
#endif
  dx_assets_system_factory* assets_system_factory = NULL;
  if (dx_assets_system_factory_create(&assets_system_factory)) {
    DX_UNREFERENCE(aal_system_factory);
    aal_system_factory = NULL;
    DX_UNREFERENCE(val_system_factory);
    val_system_factory = NULL;
    return Core_Failure;
  }
  dx_application* temporary = NULL;
  if (dx_application_create(&temporary, val_system_factory, aal_system_factory, assets_system_factory, msg_queue)) {
    DX_UNREFERENCE(assets_system_factory);
    assets_system_factory = NULL;
    DX_UNREFERENCE(aal_system_factory);
    aal_system_factory = NULL;
    DX_UNREFERENCE(val_system_factory);
    val_system_factory = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(assets_system_factory);
  assets_system_factory = NULL;
  DX_UNREFERENCE(aal_system_factory);
  aal_system_factory = NULL;
  DX_UNREFERENCE(val_system_factory);
  val_system_factory = NULL;
  *RETURN = temporary;
  return Core_Success;
}


/// @brief Create an application with the specified message queue.
/// @param msg_queue A pointer to the message queue.
/// @return A pointer to the application on success. The null pointer on failure. 
static dx_application* create_application(dx_msg_queue* msg_queue);

static dx_application* create_application(dx_msg_queue* msg_queue) {
  dx_application* application = NULL;
  if (dx_player_create_application(&application, msg_queue)) {
    return NULL;
  }
  return DX_APPLICATION(application);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_application_presenter* g_application_presenter = NULL;

static dx_application* g_application = NULL;

static dx_msg_queue *g_msg_queue  = NULL;

static dx_font_manager* g_font_manager = NULL;

static dx_rectangle_presenter* g_rectangle_presenter = NULL;

static dx_font_presenter* g_font_presenter = NULL;

static dx_cl_interpreter* g_cl_interpreter = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result run();

static Core_Result startup();

static Core_Result shutdown();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_application_presenter_get(dx_application_presenter** RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(g_application_presenter);
  *RETURN = g_application_presenter;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result startup_managers() {
  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, g_application)) {
    return Core_Failure;
  }
  dx_aal_context* aal_context = NULL;
  if (dx_application_get_aal_context(&aal_context, g_application)) {
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_font_manager_create(&g_font_manager, val_context)) {
    DX_UNREFERENCE(aal_context);
    aal_context = NULL;
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_rectangle_presenter_create(&g_rectangle_presenter, val_context, aal_context)) {
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(aal_context);
    aal_context = NULL;
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_font_presenter_create(&g_font_presenter, g_font_manager, g_rectangle_presenter)) {
    DX_UNREFERENCE(g_rectangle_presenter);
    g_rectangle_presenter = NULL;
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(aal_context);
    aal_context = NULL;
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_cl_interpreter_create(&g_cl_interpreter)) {
    DX_UNREFERENCE(g_font_presenter);
    g_font_presenter = NULL;
    DX_UNREFERENCE(g_rectangle_presenter);
    g_rectangle_presenter = NULL;
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(aal_context);
    aal_context = NULL;
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_console_commands_register_all(g_cl_interpreter)) {
    DX_UNREFERENCE(g_font_presenter);
    g_font_presenter = NULL;
    DX_UNREFERENCE(g_rectangle_presenter);
    g_rectangle_presenter = NULL;
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(aal_context);
    aal_context = NULL;
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(aal_context);
  aal_context = NULL;
  DX_UNREFERENCE(val_context);
  val_context = NULL;
  return Core_Success;
}

static void shutdown_managers() {
  DX_UNREFERENCE(g_font_presenter);
  g_font_presenter = NULL;
  DX_UNREFERENCE(g_rectangle_presenter);
  g_rectangle_presenter = NULL;
  DX_UNREFERENCE(g_font_manager);
  g_font_manager = NULL;
  DX_UNREFERENCE(g_cl_interpreter);
  g_cl_interpreter = NULL;
}

static Core_Result run() {
  ENTER(DX_C_FUNCTION_NAME);
  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, g_application)) {
    return Core_Failure;
  }
  {
    Core_Message* msg = NULL;
    //
    if (Core_EmitMessage_create((Core_EmitMessage**)&msg, "Engine version: 0.1\n", sizeof("Engine: version 0.1\n") - 1)) {
      DX_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      DX_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  {
    Core_Message* msg = NULL;
    //
    Core_String* information = dx_val_context_get_information(val_context);
    if (!information) {
      DX_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    if (Core_EmitMessage_create((Core_EmitMessage**)&msg, information->bytes, information->number_of_bytes)) {
      DX_UNREFERENCE(information);
      information = NULL;
      DX_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    DX_UNREFERENCE(information);
    information = NULL;
    //
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      DX_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  if (dx_application_presenter_startup(g_application_presenter)) {
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (dx_application_presenter_run(g_application_presenter)) {
    DX_UNREFERENCE(val_context);
    val_context = NULL;
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  dx_application_presenter_shutdown(g_application_presenter);
  DX_UNREFERENCE(val_context);
  val_context = NULL;
  LEAVE(DX_C_FUNCTION_NAME);
  return Core_Success;
}

static Core_Result startup() {
  ENTER(DX_C_FUNCTION_NAME);
  if (Core_initialize()) {
    return Core_Failure;
  }
  if (dx_msg_queue_create(&g_msg_queue)) {
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  g_application = create_application(g_msg_queue);
  if (!g_application) {
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (dx_application_startup_systems(g_application)) {
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (startup_managers()) {
    dx_application_shutdown_systems(g_application);
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, g_application)) {
    shutdown_managers();
    dx_application_shutdown_systems(g_application);
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (dx_default_application_presenter_create((dx_default_application_presenter**)&g_application_presenter,
                                              g_font_presenter,
                                              g_rectangle_presenter,
                                              g_application,
                                              g_cl_interpreter,
                                              g_msg_queue,
                                              val_context)) {
    DX_UNREFERENCE(val_context);
    val_context = NULL;

    shutdown_managers();
    dx_application_shutdown_systems(g_application);
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  DX_UNREFERENCE(val_context);
  val_context = NULL;
  LEAVE(DX_C_FUNCTION_NAME);
  return Core_Success;
}

static Core_Result shutdown() {
  ENTER(DX_C_FUNCTION_NAME);
  DX_UNREFERENCE(g_application_presenter);
  g_application_presenter = NULL;
  shutdown_managers();
  dx_application_shutdown_systems(g_application);
  DX_UNREFERENCE(g_application);
  g_application = NULL;
  dx_msg_queue_destroy(g_msg_queue);
  g_msg_queue = NULL;
  Core_uninitialize();
  LEAVE(DX_C_FUNCTION_NAME);
  return Core_Success;
}

static Core_Result dx_run_tests() {
#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
  if (dx_math_tests()) {
    return Core_Failure;
  }
#endif
#if defined(DX_SAFE_MUL_IX_WITH_TESTS) && 1 == DX_SAFE_MUL_IX_WITH_TESTS
  if (dx_safe_mul_ix_tests()) {
    return Core_Failure;
  }
#endif
#if defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS) && 1 == DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS
  if (dx_ddl_parser_tests()) {
    return Core_Failure;
  }
#endif
#if defined(DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS) && 1 == DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS
  if (dx_ddl_linemap_tests()) {
    return Core_Failure;
  }
#endif
  return Core_Success;
}

Core_Result dx_run(int argc, char** argv) {
  if (startup()) {
    return Core_Failure;
  }
  if (dx_run_tests()) {
    shutdown();
    return Core_Failure;
  }
  if (run()) {
    shutdown();
    return Core_Failure;
  }
  if (shutdown()) {
    return Core_Failure;
  }
  return Core_Success;
}
