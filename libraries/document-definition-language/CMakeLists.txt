# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.document-definition-language")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md"
list(APPEND ${name}.readmes "README.md")

# "dx/document_definition_language/configuration.(c|h)"
list(APPEND ${name}.sources "dx/document_definition_language/configuration.c")
list(APPEND ${name}.headers "dx/document_definition_language/configuration.h")

# "document_definition_language.(c|h)"
list(APPEND ${name}.sources "dx/document_definition_language.c")
list(APPEND ${name}.headers "dx/document_definition_language.h")

# "document_definition_language" folder
list(APPEND ${name}.sources "dx/document_definition_language/scanner.c")
list(APPEND ${name}.headers "dx/document_definition_language/scanner.h")
list(APPEND ${name}.sources "dx/document_definition_language/word_kind.c")
list(APPEND ${name}.headers "dx/document_definition_language/word_kind.h")
list(APPEND ${name}.inlays  "dx/document_definition_language/word_kind.i")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "dailies.core")
