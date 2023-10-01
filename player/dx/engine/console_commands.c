#include "dx/engine/console_commands.h"

#include "dx/player/run.h"

#include "dx/assets.h"
#include "dx/core.h"
#include "dx/data_definition_language.h"
#include "dx/document_definition_language.h"
#include "dx/engine.h"
#include <string.h>

static dx_result print_library_info(dx_application_presenter* application_presenter, char const* name, dx_n64 major_version, dx_n64 minor_version) {
  dx_string* format_string = NULL,
           * name_string = NULL,
           * message_string = NULL;
//
  if (dx_string_create(&format_string, "${s} ${n64}.${n64}\n", sizeof("${s} ${n64}.${n64}\n") - 1)) {
    return DX_FAILURE;
  }
  //
  if (dx_string_create(&name_string, name, strlen(name))) {
    DX_UNREFERENCE(format_string);
    format_string = NULL;
    return DX_FAILURE;
  }
  //
  message_string = dx_string_printf(format_string, name_string, (dx_n64)major_version, (dx_n64)minor_version);
  DX_UNREFERENCE(name_string);
  name_string = NULL;
  DX_UNREFERENCE(format_string);
  format_string = NULL;
  if (!message_string) {
    return DX_FAILURE;
  }
  //
  dx_console* console = NULL;
  if (dx_application_presenter_get_console(&console, application_presenter)) {
    DX_UNREFERENCE(message_string);
    message_string = NULL;
    return DX_FAILURE;
  }
  if (dx_console_append_text(console, message_string)) {
    DX_UNREFERENCE(console);
    console = NULL;
    DX_UNREFERENCE(message_string);
    message_string = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(console);
  console = NULL;
  DX_UNREFERENCE(message_string);
  message_string = NULL;
  //
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result command_core_library_print_info(dx_application_presenter* application_presenter);

static dx_result command_assets_library_print_info(dx_application_presenter* application_presenter);

static dx_result command_engine_library_print_info(dx_application_presenter* application_presenter);

static dx_result command_data_definition_language_library_print_info(dx_application_presenter* application_presenter);

static dx_result command_document_definition_language_library_print_info(dx_application_presenter* application_presenter);

static dx_result command_application_request_quit(dx_application_presenter* application_presenter);

static dx_result command_core_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_CORE_LIBRARY_NAME, DX_CORE_LIBRARY_MAJOR_VERSION, DX_CORE_LIBRARY_MINOR_VERSION);
}

static dx_result command_assets_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_ASSETS_LIBRARY_NAME, DX_ASSETS_LIBRARY_MAJOR_VERSION, DX_ASSETS_LIBRARY_MINOR_VERSION);
}

static dx_result command_engine_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_ENGINE_LIBRARY_NAME, DX_ENGINE_LIBRARY_MAJOR_VERSION, DX_ENGINE_LIBRARY_MINOR_VERSION);
}

static dx_result command_data_definition_language_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_NAME, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION);
}

static dx_result command_document_definition_language_library_print_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_NAME, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION);
}

static dx_result command_application_request_quit(dx_application_presenter* application_presenter) {
  dx_application* application = NULL;
  if (dx_application_get(&application)) { 
    return DX_FAILURE;
  }
  if (dx_application_emit_quit_msg(application)) {
    DX_UNREFERENCE(application);
    application = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(application);
  application = NULL;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result dx_console_commands_register_all(dx_cl_interpreter* cl_interpreter) {
#define DEFINE(CEL_NAME, CXX_NAME) \
  { \
    dx_string* cel_name = NULL; \
    if (dx_string_create(&cel_name, CEL_NAME, strlen(CEL_NAME))) { \
      return DX_FAILURE; \
    } \
    if (dx_cl_interpreter_register_function(cl_interpreter, cel_name, &CXX_NAME)) { \
      DX_UNREFERENCE(cel_name); \
      cel_name = NULL; \
      return DX_FAILURE; \
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
  return DX_SUCCESS;
}
