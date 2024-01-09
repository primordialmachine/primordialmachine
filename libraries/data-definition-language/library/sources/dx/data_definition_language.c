#include "dx/data_definition_language.h"

static dx_ddl_parser* _create_parser() {
  dx_data_definition_language_diagnostics* diagnostics = NULL;
  if (dx_data_definition_language_diagnostics_create(&diagnostics)) {
    return NULL;
  }
  dx_data_definition_language_scanner* scanner = NULL;
  if (dx_data_definition_language_scanner_create(&scanner, diagnostics)) {
    CORE_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  dx_ddl_parser* parser = NULL;
  if (dx_ddl_parser_create(&parser, scanner, diagnostics)) {
    CORE_UNREFERENCE(scanner);
    scanner = NULL;
    CORE_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  CORE_UNREFERENCE(scanner);
  scanner = NULL;
  CORE_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  return parser;
}

dx_ddl_node* dx_ddl_compile(char const* p, Core_Size n) {
  dx_ddl_parser* parser = _create_parser();
  if (!parser) {
    return NULL;
  }
  if (dx_ddl_parser_set(parser, p, n)) {
    CORE_UNREFERENCE(parser);
    parser = NULL;
    return NULL;
  }
  dx_ddl_node* node = NULL;
  if (dx_ddl_parser_run(&node, parser)) {
    CORE_UNREFERENCE(parser);
    parser = NULL;
    return NULL;
  }
  CORE_UNREFERENCE(parser);
  parser = NULL;
  return node;
}
