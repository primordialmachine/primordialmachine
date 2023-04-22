# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.document-definition-language")
dx_begin_project(${name})
project(${name} LANGUAGES C)

# "README.md"
list(APPEND ${name}.readmes "README.md")

# "dx/text/configuration.(c|h)"
list(APPEND ${name}.sources "dx/document_definition_language/configuration.c")
list(APPEND ${name}.headers "dx/document_definition_language/configuration.h")

# "text.(c|h)"
list(APPEND ${name}.sources "dx/document_definition_language.c")
list(APPEND ${name}.headers "dx/document_definition_language.h")

# "text" folder
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

add_library(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${name} "dailies.core")

dx_end_project(${name})
