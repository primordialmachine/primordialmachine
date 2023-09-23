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

dx_result dx_console_commands_print_core_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_CORE_LIBRARY_NAME, DX_CORE_LIBRARY_MAJOR_VERSION, DX_CORE_LIBRARY_MINOR_VERSION);
}

dx_result dx_console_commands_print_assets_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_ASSETS_LIBRARY_NAME, DX_ASSETS_LIBRARY_MAJOR_VERSION, DX_ASSETS_LIBRARY_MINOR_VERSION);
}

dx_result dx_console_commands_print_engine_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_ENGINE_LIBRARY_NAME, DX_ENGINE_LIBRARY_MAJOR_VERSION, DX_ENGINE_LIBRARY_MINOR_VERSION);
}

dx_result dx_console_commands_print_data_definition_language_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_NAME, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION, DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION);
}

dx_result dx_console_commands_print_document_definition_language_info(dx_application_presenter* application_presenter) {
  return print_library_info(application_presenter, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_NAME, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION, DX_DOCUMENT_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION);
}
