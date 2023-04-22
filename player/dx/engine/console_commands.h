#if !defined(DX_CONSOLE_COMMANDS_H_INCLUDED)
#define DX_CONSOLE_COMMANDS_H_INCLUDED

#include "dx/cl_interpreter.h"

dx_result dx_console_commands_print_core_info(dx_application_presenter* application_presenter);

dx_result dx_console_commands_print_assets_info(dx_application_presenter* application_presenter);

dx_result dx_console_commands_print_engine_info(dx_application_presenter* application_presenter);

dx_result dx_console_commands_print_data_definition_language_info(dx_application_presenter* application_presenter);

dx_result dx_console_commands_print_document_definition_language_info(dx_application_presenter* application_presenter);

#endif // DX_CONSOLE_COMMANDS_H_INCLUDED
