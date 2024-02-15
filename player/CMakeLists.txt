# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.player")
set(${name}.kind "executable")
set(${name}.languages "C")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}/sources" "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}/sources")

dx_begin_project(${name})

# "main.c".
list(APPEND ${name}.sources "sources/main.c")

# "dx/scenes/*".
list(APPEND ${name}.sources "sources/dx/scenes/default_scene_presenter.c")
list(APPEND ${name}.headers "sources/dx/scenes/default_scene_presenter.h")
list(APPEND ${name}.sources "sources/dx/scenes/create_assets.c")
list(APPEND ${name}.headers "sources/dx/scenes/create_assets.h")
list(APPEND ${name}.sources "sources/dx/player/run.c")
list(APPEND ${name}.headers "sources/dx/player/run.h")
list(APPEND ${name}.sources "sources/dx/player/default_application_presenter.c")
list(APPEND ${name}.headers "sources/dx/player/default_application_presenter.h")
list(APPEND ${name}.headers "sources/dx/player/configuration.h")
list(APPEND ${name}.sources "sources/dx/engine/default_console.c")
list(APPEND ${name}.headers "sources/dx/engine/default_console.h")
list(APPEND ${name}.sources "sources/dx/engine/overlay.c")
list(APPEND ${name}.headers "sources/dx/engine/overlay.h")
list(APPEND ${name}.sources "sources/dx/engine/console_commands.c")
list(APPEND ${name}.headers "sources/dx/engine/console_commands.h")

dx_end_project(${name})

# DLL plugins.
add_dependencies(${name} wic-plugin)
add_custom_command(TARGET ${name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_BINARY_DIR}/../plugins/wic-plugin/plugin/$<CONFIG>/wic-plugin.dll $<TARGET_FILE_DIR:${name}>
                   COMMAND_EXPAND_LISTS)
add_dependencies(${name} freetype-plugin)
add_custom_command(TARGET ${name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_BINARY_DIR}/../plugins/freetype-plugin/plugin/$<CONFIG>/freetype-plugin.dll $<TARGET_FILE_DIR:${name}>
                   COMMAND_EXPAND_LISTS)

target_link_libraries(${name} Core.Engine)
target_link_libraries(${name} dailies.val.gl)
target_link_libraries(${name} dailies.aal.al)

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

