#include "dx/player/run.h"

#include "dx/font_manager.h"
#include "dx/application.h"
#include "dx/scenes/default_scene_presenter.h"
#include "dx/fps_counter.h"
#include <stdio.h>

#include "dx/data_definition_language.h"

#if defined(_WIN32)
  // GetTickCount64
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("environment not (yet) supported")
#endif

#if DX_VISUALS_OPENGL4 == DX_VISUALS
  #include "dx/val/gl.h"
#elif DX_VISUALS_DIRECT3D12 == DX_VISUALS
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

static dx_result dx_player_create_application(dx_application** RETURN, dx_msg_queue* msg_queue);

#if DX_VISUALS_OPENGL4 == DX_VISUALS
  #include "dx/val/gl.h"
#else
  #error("environment not (yet) supported")
#endif

#if DX_AUDIALS_OPENAL == DX_AUDIALS
  #include "dx/aal/al/system_factory.h"
#else
  #error("environment not (yet) supported")
#endif

static dx_result dx_player_create_application(dx_application** RETURN, dx_msg_queue* msg_queue) {
#if DX_VISUALS_OPENGL4 == DX_VISUALS && DX_OPERATING_SYSTEM_WINDOWS == DX_OPERATING_SYSTEM
  dx_val_system_factory* val_system_factory = NULL;
  if (dx_val_gl_wgl_system_factory_create((dx_val_gl_wgl_system_factory**)&val_system_factory)) {
    return DX_FAILURE;
  }
#else
  #error("environment not (yet) supported")
#endif
#if DX_AUDIALS_OPENAL == DX_AUDIALS
  dx_aal_system_factory* aal_system_factory = NULL;
  if (dx_aal_al_system_factory_create((dx_aal_al_system_factory**)&aal_system_factory)) {
    DX_UNREFERENCE(val_system_factory);
    val_system_factory = NULL;
    return DX_FAILURE;
  }
#else
  #error("environment not (yet) supported")
#endif
  dx_application* temporary = NULL;
  if (dx_application_create(&temporary, val_system_factory, aal_system_factory, msg_queue)) {
    DX_UNREFERENCE(aal_system_factory);
    aal_system_factory = NULL;
    DX_UNREFERENCE(val_system_factory);
    val_system_factory = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(aal_system_factory);
  aal_system_factory = NULL;
  DX_UNREFERENCE(val_system_factory);
  val_system_factory = NULL;
  *RETURN = temporary;
  return DX_SUCCESS;
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

static dx_result run();

static dx_result startup();

static dx_result shutdown();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_val_context* dx_val_context_get() {
  dx_val_context* temporary = NULL;
  if (dx_application_get_val_context(&temporary, g_application)) {
    return NULL;
  }
  return temporary;
}

dx_result dx_application_presenter_get(dx_application_presenter** RETURN) {
  if (!RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_application_presenter);
  *RETURN = g_application_presenter;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result startup_managers() {
  dx_val_context* context = dx_val_context_get();
  if (!context) {
    return DX_FAILURE;
  }
  if (dx_font_manager_create(&g_font_manager, context)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return DX_FAILURE;
  }
  if (dx_rectangle_presenter_create(&g_rectangle_presenter, context)) {
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(context);
    context = NULL;
    return DX_FAILURE;
  }
  if (dx_font_presenter_create(&g_font_presenter, g_font_manager, g_rectangle_presenter, context)) {
    DX_UNREFERENCE(g_rectangle_presenter);
    g_rectangle_presenter = NULL;
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(context);
    context = NULL;
    return DX_FAILURE;
  }
  if (dx_cl_interpreter_create(&g_cl_interpreter)) {
    DX_UNREFERENCE(g_font_presenter);
    g_font_presenter = NULL;
    DX_UNREFERENCE(g_rectangle_presenter);
    g_rectangle_presenter = NULL;
    DX_UNREFERENCE(g_font_manager);
    g_font_manager = NULL;
    DX_UNREFERENCE(context);
    context = NULL;
    return DX_FAILURE;
  }
#define DEFINE(CEL_NAME, CXX_NAME) \
  { \
    dx_string* k = NULL; \
    if (dx_string_create(&k, CEL_NAME, strlen(CEL_NAME))) { \
      DX_UNREFERENCE(g_cl_interpreter); \
      g_cl_interpreter = NULL; \
      DX_UNREFERENCE(g_font_presenter); \
      g_font_presenter = NULL; \
      DX_UNREFERENCE(g_rectangle_presenter); \
      g_rectangle_presenter = NULL; \
      DX_UNREFERENCE(g_font_manager); \
      g_font_manager = NULL; \
      DX_UNREFERENCE(context); \
      context = NULL; \
      return DX_FAILURE; \
    } \
    if (dx_cl_interpreter_register_function(g_cl_interpreter, k, &CXX_NAME)) { \
      DX_UNREFERENCE(k); \
      k = NULL; \
      DX_UNREFERENCE(g_cl_interpreter); \
      g_cl_interpreter = NULL; \
      DX_UNREFERENCE(g_font_presenter); \
      g_font_presenter = NULL; \
      DX_UNREFERENCE(g_rectangle_presenter); \
      g_rectangle_presenter = NULL; \
      DX_UNREFERENCE(g_font_manager); \
      g_font_manager = NULL; \
      DX_UNREFERENCE(context); \
      context = NULL; \
      return DX_FAILURE; \
    } \
    DX_UNREFERENCE(k); \
    k = NULL; \
  }
  DEFINE("assetLibrary.printInfo", dx_console_commands_print_assets_info);
  DEFINE("coreLibrary.printInfo", dx_console_commands_print_core_info);
  DEFINE("dataDefinitionLanguageLibrary.printInfo", dx_console_commands_print_data_definition_language_info);
  DEFINE("documentDefinitionLanguageLibrary.printInfo", dx_console_commands_print_document_definition_language_info);
  DEFINE("engineLibrary.printInfo", dx_console_commands_print_engine_info);
#undef DEFINE

  DX_UNREFERENCE(context);
  context = NULL;
  return DX_SUCCESS;
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

static dx_result run() {
  ENTER(DX_C_FUNCTION_NAME);
  dx_val_context* ctx = dx_val_context_get();
  {
    dx_msg* msg;
    //
    msg = DX_MSG(dx_emit_msg_create("Engine version: 0.1\n", sizeof("Engine: version 0.1\n") - 1));
    if (!msg) {
      DX_UNREFERENCE(ctx);
      ctx = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return DX_FAILURE;
    }
    //
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      DX_UNREFERENCE(ctx);
      ctx = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return DX_FAILURE;
    }
    //
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  {
    dx_msg* msg;
    //
    dx_string* information = dx_val_context_get_information(ctx);
    if (!information) {
      DX_UNREFERENCE(ctx);
      ctx = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return DX_FAILURE;
    }
    //
    msg = DX_MSG(dx_emit_msg_create(information->bytes, information->number_of_bytes));
    DX_UNREFERENCE(information);
    information = NULL;
    //
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      DX_UNREFERENCE(ctx);
      ctx = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return DX_FAILURE;
    }
    //
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  if (dx_application_presenter_startup(g_application_presenter)) {
    DX_UNREFERENCE(ctx);
    ctx = NULL;
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  if (dx_application_presenter_run(g_application_presenter)) {
    DX_UNREFERENCE(ctx);
    ctx = NULL;
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  dx_application_presenter_shutdown(g_application_presenter);
  DX_UNREFERENCE(ctx);
  ctx = NULL;
  LEAVE(DX_C_FUNCTION_NAME);
  return DX_SUCCESS;
}

static dx_result startup() {
  ENTER(DX_C_FUNCTION_NAME);
  if (dx_rti_initialize()) {
    return DX_FAILURE;
  }
  if (dx_msg_queue_create(&g_msg_queue)) {
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  g_application = create_application(g_msg_queue);
  if (!g_application) {
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  if (dx_application_startup_systems(g_application)) {
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  if (startup_managers()) {
    dx_application_shutdown_systems(g_application);
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  dx_val_context* val_context = dx_val_context_get();
  if (!val_context) {
    shutdown_managers();
    dx_application_shutdown_systems(g_application);
    DX_UNREFERENCE(g_application);
    g_application = NULL;
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  if (dx_default_application_presenter_create((dx_default_application_presenter**)&g_application_presenter,
                                              g_font_presenter,
                                              g_font_manager,
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
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(val_context);
  val_context = NULL;
  LEAVE(DX_C_FUNCTION_NAME);
  return DX_SUCCESS;
}

static dx_result shutdown() {
  ENTER(DX_C_FUNCTION_NAME);
  DX_UNREFERENCE(g_application_presenter);
  g_application_presenter = NULL;
  shutdown_managers();
  dx_application_shutdown_systems(g_application);
  DX_UNREFERENCE(g_application);
  g_application = NULL;
  dx_msg_queue_destroy(g_msg_queue);
  g_msg_queue = NULL;
  dx_rti_unintialize();
  LEAVE(DX_C_FUNCTION_NAME);
  return DX_SUCCESS;
}

static dx_result dx_run_tests() {
#if defined(DX_NEXT_POWER_OF_TWO_WITH_TESTS) && 1 == DX_NEXT_POWER_OF_TWO_WITH_TESTS
  if (dx_next_power_of_two_tests()) {
    return DX_FAILURE;
  }
#endif
#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
  if (dx_math_tests()) {
    return DX_FAILURE;
  }
#endif
#if defined(DX_SAFE_ADD_NX_WITH_TESTS) && 1 == DX_SAFE_ADD_NX_WITH_TESTS
  if (dx_safe_add_nx_tests()) {
    return DX_FAILURE;
  }
#endif
#if defined(DX_SAFE_MUL_IX_WITH_TESTS) && 1 == DX_SAFE_MUL_IX_WITH_TESTS
  if (dx_safe_mul_ix_tests()) {
    return DX_FAILURE;
  }
#endif
#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS
  if (dx_safe_mul_nx_tests()) {
    return DX_FAILURE;
  }
#endif
#if defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS) && 1 == DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS
  if (dx_ddl_parser_tests()) {
    return DX_FAILURE;
  }
#endif
#if defined(DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS) && 1 == DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS
  if (dx_ddl_linemap_tests()) {
    return DX_FAILURE;
  }
#endif
  return DX_SUCCESS;
}

dx_result dx_run(int argc, char** argv) {
  if (startup()) {
    return DX_FAILURE;
  }
  if (dx_run_tests()) {
    shutdown();
    return DX_FAILURE;
  }
  if (run()) {
    shutdown();
    return DX_FAILURE;
  }
  if (shutdown()) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}
