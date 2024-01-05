# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.data-definition-language")
set(${name}.kind "static-library")
set(${name}.languages "C")

set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}/sources")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}/sources")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}/sources")

set(${name}.documentation_directory "${CMAKE_CURRENT_SOURCE_DIR}/documentation")

dx_begin_project(${name})

list(APPEND ${name}.documentation "documentation/index.css")
list(APPEND ${name}.documentation "documentation/index.php")
list(APPEND ${name}.documentation "documentation/includes/1-introduction.php")
list(APPEND ${name}.documentation "documentation/includes/2-grammars.php")
list(APPEND ${name}.documentation "documentation/includes/3-lexical-structure.php")
list(APPEND ${name}.documentation "documentation/includes/4-syntactical-structure.php")
list(APPEND ${name}.documentation "documentation/includes/5-types-and-values.php")

# "README.md"
list(APPEND ${name}.readmes "sources/README.md")

# "dx/ddl/configuration.(c|h)".
list(APPEND ${name}.sources "sources/dx/data_definition_language/configuration.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/configuration.h")

# "data_definition_language.(c|h)".
list(APPEND ${name}.sources "sources/dx/data_definition_language.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language.h")

# "data_definition_language" folder.
list(APPEND ${name}.sources "sources/dx/data_definition_language/word_kind.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/word_kind.h")
list(APPEND ${name}.inlays  "sources/dx/data_definition_language/word_kind.i")
list(APPEND ${name}.sources "sources/dx/data_definition_language/node_kind.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/node_kind.h")
list(APPEND ${name}.inlays  "sources/dx/data_definition_language/node_kind.i")

# "ddl" folder.
list(APPEND ${name}.sources "sources/dx/data_definition_language/word.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/word.h")
list(APPEND ${name}.sources "sources/dx/data_definition_language/scanner.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/scanner.h")
list(APPEND ${name}.sources "sources/dx/data_definition_language/node.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/node.h")
list(APPEND ${name}.sources "sources/dx/data_definition_language/parser.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/parser.h")
list(APPEND ${name}.sources "sources/dx/data_definition_language/diagnostics.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/diagnostics.h")
list(APPEND ${name}.sources "sources/dx/data_definition_language/linemap.c")
list(APPEND ${name}.headers "sources/dx/data_definition_language/linemap.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "Core")
