# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.val.gl")
dx_begin_project(${name})
project(${name} LANGUAGES C)

# "README.md".
list(APPEND ${name}.readmes "README.md")

# "VAL OpenGL backend"
list(APPEND ${name}.sources "dx/gl/context.c" "dx/gl/program.c" "dx/gl/buffer.c" "dx/gl/vbinding.c")
list(APPEND ${name}.headers "dx/gl/context.h" "dx/gl/program.h" "dx/gl/buffer.h" "dx/gl/vbinding.h")
list(APPEND ${name}.inlays "dx/gl/functions.i")
list(APPEND ${name}.sources "dx/gl/texture.c")
list(APPEND ${name}.headers "dx/gl/texture.h")
list(APPEND ${name}.sources "dx/gl/application.c")
list(APPEND ${name}.headers "dx/gl/application.h")

# "VAL WGL/OpenGL backend"
list(APPEND ${name}.sources "dx/gl/wgl/context.c")
list(APPEND ${name}.headers "dx/gl/wgl/context.h")
list(APPEND ${name}.sources "dx/gl/wgl/wm.c")
list(APPEND ${name}.headers "dx/gl/wgl/wm.h" )
list(APPEND ${name}.inlays  "dx/gl/wgl/functions.i")
list(APPEND ${name}.sources "dx/gl/wgl/application.c")
list(APPEND ${name}.headers "dx/gl/wgl/application.h")
list(APPEND ${name}.sources "dx/gl/wgl/window.c")
list(APPEND ${name}.headers "dx/gl/wgl/window.h")
list(APPEND ${name}.sources "dx/gl/wgl/messages.c")
list(APPEND ${name}.headers "dx/gl/wgl/messages.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

add_library(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

# OpenGL-specific header files.
# TODO: Do not defined as "PUBLIC". Define as "PRIVATE.
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${CMAKE_CURRENT_SOURCE_DIR}/external/opengl/includes)

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${name} "dailies.engine")

dx_end_project(${name})
