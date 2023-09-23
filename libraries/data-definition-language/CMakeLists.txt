# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.data-definition-language")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md"
list(APPEND ${name}.readmes "README.md")

# "dx/ddl/configuration.(c|h)".
list(APPEND ${name}.sources "dx/ddl/configuration.c")
list(APPEND ${name}.headers "dx/ddl/configuration.h")

# "data_definition_language.(c|h)".
list(APPEND ${name}.sources "dx/data_definition_language.c")
list(APPEND ${name}.headers "dx/data_definition_language.h")

# "data_definition_language" folder.
list(APPEND ${name}.sources "dx/data_definition_language/word_kind.c")
list(APPEND ${name}.headers "dx/data_definition_language/word_kind.h")
list(APPEND ${name}.inlays  "dx/data_definition_language/word_kind.i")
list(APPEND ${name}.sources "dx/data_definition_language/node_kind.c")
list(APPEND ${name}.headers "dx/data_definition_language/node_kind.h")
list(APPEND ${name}.inlays  "dx/data_definition_language/node_kind.i")

# "ddl" folder.
list(APPEND ${name}.sources "dx/ddl/word.c")
list(APPEND ${name}.headers "dx/ddl/word.h")
list(APPEND ${name}.sources "dx/ddl/scanner.c")
list(APPEND ${name}.headers "dx/ddl/scanner.h")
list(APPEND ${name}.sources "dx/ddl/node.c")
list(APPEND ${name}.headers "dx/ddl/node.h")
list(APPEND ${name}.sources "dx/ddl/parser.c")
list(APPEND ${name}.headers "dx/ddl/parser.h")
list(APPEND ${name}.sources "dx/ddl/diagnostics.c")
list(APPEND ${name}.headers "dx/ddl/diagnostics.h")
list(APPEND ${name}.sources "dx/ddl/linemap.c")
list(APPEND ${name}.headers "dx/ddl/linemap.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "dailies.core")
