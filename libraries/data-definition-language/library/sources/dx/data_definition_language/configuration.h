#if !defined(DX_DATA_DEFINITION_LANGUAGE_CONFIGURATION_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_CONFIGURATION_H_INCLUDED

/// If defined and evaluating to @a 1,
/// then a function dx_ddl_parser_tests() is provided
/// which performs tests on functionality provided by "dx/ddl/parser.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS (1)

/// If defined and evaluating to @a 1,
/// then a function dx_ddl_linemap_tests() is provided
/// which performs tests on functionality provided by "dx/ddl/linemap.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS (1)

#endif // DX_DATA_DEFINITION_LANGUAGE_CONFIGURATION_H_INCLUDED
