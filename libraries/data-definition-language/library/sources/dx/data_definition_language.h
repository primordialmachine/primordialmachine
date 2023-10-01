/// @file dx/data_definition_language.h
/// @brief "Data Definition Language" library master include file.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)

#if !defined(DX_DATA_DEFINITION_LANGUAGE_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_H_INCLUDED


#include "dx/data_definition_language/diagnostics.h"
#include "dx/data_definition_language/linemap.h"
#include "dx/data_definition_language/node.h"
#include "dx/data_definition_language/parser.h"
#include "dx/data_definition_language/word.h"


/// @brief The name of this library.
#define DX_DATA_DEFINITION_LANGUAGE_LIBRARY_NAME "Data Definition Language"

/// @brief The major version of this library.
#define DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MAJOR_VERSION (0)

/// @brief The minor version of this library.
#define DX_DATA_DEFINITION_LANGUAGE_LIBRARY_MINOR_VERSION (1)


dx_ddl_node* dx_ddl_compile(char const* p, dx_size n);


#endif // DX_DATA_DEFINITION_LANGUAGE_H_INCLUDED
