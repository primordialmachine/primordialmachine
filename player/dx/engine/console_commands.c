#include "dx/engine/console_commands.h"

#include "dx/player/run.h"

#include "dx/assets.h"
#include "dx/core.h"
#include "dx/data_definition_language.h"
#include "dx/document_definition_language.h"
#include "dx/engine.h"
#include <string.h>

static Core_Result print_library_info(dx_application_presenter* application_presenter, char const* name, Core_Natural64 major_version, Core_Natural64 minor_version) {
  Core_String* format_string = NULL,
           * name_string = NULL,
           * message_string = NULL;
//
  if (Core_String_create(&format_string, "${s} ${n64}.${n64}\n", sizeof("${s} ${n64}.${n64}\n") - 1)) {
    return Core_Failure;
  }
  //
  if (Core_String_create(&name_string, name, strlen(name))) {
    DX_UNREFERENCE(format_string);
    format_string = NULL;
    return Core_Failure;
  }
  //
  message_string = NULL;
  if (Core_String_printf(&message_string, format_string, name_string, (Core_Natural64)major_version, (Core_Natural64)minor_version)) {
    DX_UNREFERENCE(name_string);
    name_string = NULL;
    DX_UNREFERENCE(format_string);
    format_string = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(name_string);
  name_string = NULL;
  DX_UNREFERENCE(format_string);
  format_string = NULL;
  //
  dx_console* console = NULL;
  if (dx_application_presenter_get_console(&console, application_presenter)) {
    DX_UNREFERENCE(message_string);
    message_string = NULL;
    return Core_Failure;
  }
  if (dx_console_append_output_text(console, message_string)) {
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(message_string);
    message_string = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(console);
  console = NULL;
  DX_UNREFERENCE(message_string);
  message_string = NULL;
  //
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result command_core_library_print_info(dx_application_presenter* application_presenter);

static Core_Result command_assets_library_print_info(dx_application_presenter* application_presenter);

static Core_Result command_engine_library_print_info(dx_application_presenter* application_presenter);

static Core_Result command_data_definition_language_library_print_info(dx_application_presenter* application_presenter);

static Core_Result command_document_definition_language_library_print_info(dx_application_presenter* application_presenter);

static Core_Result command_application_request_quit(dx_application_presenter* application_presenter);

static Core_Result command_core_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_CORE_LIBRARY_NAME, DX_CORE_LIBRARY_MAJOR_VERSION, DX_CORE_LIBRARY_MINOR_VERSION);
}

static Core_Result command_assets_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_ASSETS_LIBRARY_NAME, DX_ASSETS_LIBRARY_MAJOR_VERSION, DX_ASSETS_LIBRARY_MINOR_VERSION);
}

static Core_Result command_engine_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_ENGINE_LIBRARY_NAME, DX_ENGINE_LIBRARY_MAJOR_VERSION, DX_ENGINE_LIBRARY_MINOR_VERSION);
}

static Core_Result command_data_definition_language_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_NAME, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION);
}

static Core_Result command_document_definition_language_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_NAME, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION);
}

static Core_Result command_application_request_quit(dx_application_presenter* application_presenter) {
  dx_application* application = NULL;
  if (dx_application_get(&application)) { 
    return Core_Failure;
  }
  if (dx_application_emit_quit_msg(application)) {
    DX_UNREFERENCE(application);
    application = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(application);
  application = NULL;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_console_commands_register_all(dx_cl_interpreter* cl_interpreter) {
#define DEFINE(CEL_NAME, CXX_NAME) \
  { \
    Core_String* cel_name = NULL; \
    if (Core_String_create(&cel_name, CEL_NAME, strlen(CEL_NAME))) { \
      return Core_Failure; \
    } \
    if (dx_cl_interpreter_register_function(cl_interpreter, cel_name, &CXX_NAME)) { \
      DX_UNREFERENCE(cel_name); \
      cel_name = NULL; \
      return Core_Failure; \
    } \
    DX_UNREFERENCE(cel_name); \
    cel_name = NULL; \
  }
  DEFINE("assetLibrary.printInfo", command_assets_library_print_info);
  DEFINE("coreLibrary.printInfo", command_core_library_print_info);
  DEFINE("dataDefinitionLanguageLibrary.printInfo", command_data_definition_language_library_print_info);
  DEFINE("documentDefinitionLanguageLibrary.printInfo", command_document_definition_language_library_print_info);
  DEFINE("engineLibrary.printInfo", command_engine_library_print_info);
  DEFINE("application.request_quit", command_application_request_quit);
#undef DEFINE
  return Core_Success;
}
