# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(name "Experimental.Service")
set(${name}.kind "executable")
set(${name}.languages "C")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")

dx_begin_project(${name})

# "main.c".
list(APPEND ${name}.sources "main.c")
list(APPEND ${name}.sources "FileSystemRoots.c")
list(APPEND ${name}.headers "FileSystemRoots.h")

dx_end_project(${name})

target_link_libraries(${name} Core)

# Copy the assets to the current binary directory.
file(GLOB_RECURSE files_to_copy RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*" )

foreach (file_to_copy ${files_to_copy})
  add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/assets/${file_to_copy}"
                                                  "${CMAKE_CURRENT_BINARY_DIR}/assets/${file_to_copy}"
    COMMAND_EXPAND_LISTS
  )
endforeach()
