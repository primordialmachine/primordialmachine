# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "libpng-plugin-test")

set(${name}.sources "main.c")
set(${name}.headers "libpng-plugin.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources} ${${name}.headers})

project(${name} LANGUAGES C)

add_executable(${name} ${${name}.sources} ${${name}.headers})

# DLL plugins.
add_dependencies(${name} libpng-plugin)
add_custom_command(TARGET ${name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_BINARY_DIR}/../plugin/$<CONFIG>/libpng-plugin.dll $<TARGET_FILE_DIR:${name}>
                   COMMAND_EXPAND_LISTS)

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

target_link_libraries(${name} "dailies.core")

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
