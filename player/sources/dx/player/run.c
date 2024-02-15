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

static Core_Result dx_player_create_application(Core_Application** RETURN, Core_MessageQueue* msg_queue);

#if Core_VisualsBackend_OpenGl4 == Core_VisualsBackend
  #include "dx/val/gl.h"
#else
  #error("environment not (yet) supported")
#endif

#if Core_AudialsBackend_OpenAl == Core_AudialsBackend
  #include "dx/aal/al.h"
#else
  #error("environment not (yet) supported")
#endif

static Core_Result dx_player_create_application(Core_Application** RETURN, Core_MessageQueue* msg_queue) {
#if Core_VisualsBackend_OpenGl4 == Core_VisualsBackend && Core_OperatingSystem_Windows == Core_OperatingSystem
  Core_Visuals_SystemFactory* visualsSystemFactory = NULL;
  if (Core_Val_Gl_Wgl_SystemFactory_create((Core_Val_Gl_Wgl_SystemFactory**)&visualsSystemFactory)) {
    return Core_Failure;
  }
#else
  #error("environment not (yet) supported")
#endif
#if Core_AudialsBackend_OpenAl == Core_AudialsBackend
  Core_Audials_SystemFactory* audialsSystemFactory = NULL;
  if (Core_Audials_Al_SystemFactory_create((Core_Audials_Al_SystemFactory**)&audialsSystemFactory)) {
    CORE_UNREFERENCE(visualsSystemFactory);
    visualsSystemFactory = NULL;
    return Core_Failure;
  }
#else
  #error("environment not (yet) supported")
#endif
  Core_Assets_SystemFactory* assets_system_factory = NULL;
  if (Core_Assets_SystemFactory_create(&assets_system_factory)) {
    CORE_UNREFERENCE(audialsSystemFactory);
    audialsSystemFactory = NULL;
    CORE_UNREFERENCE(visualsSystemFactory);
    visualsSystemFactory = NULL;
    return Core_Failure;
  }
  Core_Application* temporary = NULL;
  if (Core_Application_create(&temporary, visualsSystemFactory, audialsSystemFactory, assets_system_factory, msg_queue)) {
    CORE_UNREFERENCE(assets_system_factory);
    assets_system_factory = NULL;
    CORE_UNREFERENCE(audialsSystemFactory);
    audialsSystemFactory = NULL;
    CORE_UNREFERENCE(visualsSystemFactory);
    visualsSystemFactory = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(assets_system_factory);
  assets_system_factory = NULL;
  CORE_UNREFERENCE(audialsSystemFactory);
  audialsSystemFactory = NULL;
  CORE_UNREFERENCE(visualsSystemFactory);
  visualsSystemFactory = NULL;
  *RETURN = temporary;
  return Core_Success;
}


/// @brief Create an application with the specified message queue.
/// @param msg_queue A pointer to the message queue.
/// @return A pointer to the application on success. The null pointer on failure. 
static Core_Application* create_application(Core_MessageQueue* msg_queue);

static Core_Application* create_application(Core_MessageQueue* msg_queue) {
  Core_Application* application = NULL;
  if (dx_player_create_application(&application, msg_queue)) {
    return NULL;
  }
  return CORE_APPLICATION(application);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_application_presenter* g_application_presenter = NULL;

static Core_Application* g_application = NULL;

static Core_MessageQueue*g_msg_queue  = NULL;

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
  CORE_REFERENCE(g_application_presenter);
  *RETURN = g_application_presenter;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result startup_managers() {
  Core_Visuals_Context* val_context = NULL;
  if (Core_Application_get_val_context(&val_context, g_application)) {
    return Core_Failure;
  }
  Core_Audials_Context* aal_context = NULL;
  if (Core_Application_get_aal_context(&aal_context, g_application)) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_cl_interpreter_create(&g_cl_interpreter)) {
    CORE_UNREFERENCE(aal_context);
    aal_context = NULL;
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_console_commands_register_all(g_cl_interpreter)) {
    CORE_UNREFERENCE(aal_context);
    aal_context = NULL;
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(aal_context);
  aal_context = NULL;
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  return Core_Success;
}

static void shutdown_managers() {
  CORE_UNREFERENCE(g_cl_interpreter);
  g_cl_interpreter = NULL;
}

static Core_Result run() {
  ENTER(DX_C_FUNCTION_NAME);
  Core_Visuals_Context* val_context = NULL;
  if (Core_Application_get_val_context(&val_context, g_application)) {
    return Core_Failure;
  }
  {
    Core_Message* msg = NULL;
    Core_String* msgText = NULL;
    if (Core_String_create(&msgText, "Engine version: 0.1\n", sizeof("Engine: version 0.1\n") - 1)) {
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    if (Core_EmitMessage_create((Core_EmitMessage**)&msg, msgText)) {
      CORE_UNREFERENCE(msgText);
      msgText = NULL;
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    CORE_UNREFERENCE(msgText);
    msgText = NULL;
    //
    if (Core_MessageQueue_push(g_msg_queue, msg)) {
      CORE_UNREFERENCE(msg);
      msg = NULL;
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    CORE_UNREFERENCE(msg);
    msg = NULL;
  }
  {
    Core_Message* msg = NULL;
    //
    Core_String* msgText = NULL;
    if (Core_Visuals_Context_getInformation(&msgText, val_context)) {
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    if (Core_EmitMessage_create((Core_EmitMessage**)&msg, msgText)) {
      CORE_UNREFERENCE(msgText);
      msgText = NULL;
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    CORE_UNREFERENCE(msgText);
    msgText = NULL;
    //
    if (Core_MessageQueue_push(g_msg_queue, msg)) {
      CORE_UNREFERENCE(msg);
      msg = NULL;
      CORE_UNREFERENCE(val_context);
      val_context = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return Core_Failure;
    }
    //
    CORE_UNREFERENCE(msg);
    msg = NULL;
  }
  if (dx_application_presenter_startup(g_application_presenter)) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (dx_application_presenter_run(g_application_presenter)) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  dx_application_presenter_shutdown(g_application_presenter);
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  LEAVE(DX_C_FUNCTION_NAME);
  return Core_Success;
}

static Core_Result startup() {
  ENTER(DX_C_FUNCTION_NAME);
  if (Core_initialize()) {
    return Core_Failure;
  }
  if (Core_MessageQueue_create(&g_msg_queue)) {
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  g_application = create_application(g_msg_queue);
  if (!g_application) {
    CORE_UNREFERENCE(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (Core_Application_startup_systems(g_application)) {
    CORE_UNREFERENCE(g_application);
    g_application = NULL;
    CORE_UNREFERENCE(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (startup_managers()) {
    Core_Application_shutdown_systems(g_application);
    CORE_UNREFERENCE(g_application);
    g_application = NULL;
    CORE_UNREFERENCE(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  Core_Visuals_Context* val_context = NULL;
  if (Core_Application_get_val_context(&val_context, g_application)) {
    shutdown_managers();
    Core_Application_shutdown_systems(g_application);
    CORE_UNREFERENCE(g_application);
    g_application = NULL;
    CORE_UNREFERENCE(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  if (dx_default_application_presenter_create((dx_default_application_presenter**)&g_application_presenter,
                                              g_application,
                                              g_cl_interpreter,
                                              g_msg_queue)) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;

    shutdown_managers();
    Core_Application_shutdown_systems(g_application);
    CORE_UNREFERENCE(g_application);
    g_application = NULL;
    CORE_UNREFERENCE(g_msg_queue);
    g_msg_queue = NULL;
    Core_uninitialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return Core_Failure;
  }
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  LEAVE(DX_C_FUNCTION_NAME);
  return Core_Success;
}

static Core_Result shutdown() {
  ENTER(DX_C_FUNCTION_NAME);
  CORE_UNREFERENCE(g_application_presenter);
  g_application_presenter = NULL;
  shutdown_managers();
  Core_Application_shutdown_systems(g_application);
  CORE_UNREFERENCE(g_application);
  g_application = NULL;
  CORE_UNREFERENCE(g_msg_queue);
  g_msg_queue = NULL;
  Core_uninitialize();
  LEAVE(DX_C_FUNCTION_NAME);
  return Core_Success;
}

static Core_Result dx_run_tests() {
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
