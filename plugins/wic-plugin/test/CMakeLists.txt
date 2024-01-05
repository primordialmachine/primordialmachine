# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "wic-plugin-test")
set(${name}.languages "C")
set(${name}.kind "executable")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.doxygen_enabled OFF)
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")

dx_begin_project(${name})

set(${name}.sources "main.c")
set(${name}.headers "wic-plugin.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources} ${${name}.headers})

dx_end_project(${name})

# DLL plugins.
add_dependencies(${name} wic-plugin)
add_custom_command(TARGET ${name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_BINARY_DIR}/../plugin/$<CONFIG>/wic-plugin.dll $<TARGET_FILE_DIR:${name}>
                   COMMAND_EXPAND_LISTS)

target_link_libraries(${name} "Core")
