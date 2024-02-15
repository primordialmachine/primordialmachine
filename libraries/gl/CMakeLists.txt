# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.val.gl")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/external/opengl/includes")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

# "val/gl.(c|h)"
list(APPEND ${name}.sources "dx/val/gl.c")
list(APPEND ${name}.headers "dX/val/gl.h")

# "VAL OpenGL backend"
list(APPEND ${name}.sources "dx/val/gl/context.c" "dx/val/gl/program.c" "dx/val/gl/buffer.c" "dx/val/gl/vbinding.c")
list(APPEND ${name}.headers "dx/val/gl/context.h" "dx/val/gl/program.h" "dx/val/gl/buffer.h" "dx/val/gl/vbinding.h")
list(APPEND ${name}.inlays  "dx/val/gl/functions.i")
list(APPEND ${name}.sources "dx/val/gl/texture.c")
list(APPEND ${name}.headers "dx/val/gl/texture.h")
list(APPEND ${name}.sources "dx/val/gl/system.c")
list(APPEND ${name}.headers "dx/val/gl/system.h")
list(APPEND ${name}.sources "dx/val/gl/system_factory.c")
list(APPEND ${name}.headers "dx/val/gl/system_factory.h")
list(APPEND ${name}.sources "dx/val/gl/window.c")
list(APPEND ${name}.headers "dx/val/gl/window.h")
list(APPEND ${name}.sources "dx/val/gl/gl-includes.i")

# "VAL WGL/OpenGL backend"
list(APPEND ${name}.sources "dx/val/gl/wgl/context.c")
list(APPEND ${name}.headers "dx/val/gl/wgl/context.h")
list(APPEND ${name}.inlays  "dx/val/gl/wgl/functions.i")
list(APPEND ${name}.inlays  "dx/val/gl/wgl/extensions.i")
list(APPEND ${name}.sources "dx/val/gl/wgl/window.c")
list(APPEND ${name}.headers "dx/val/gl/wgl/window.h")
list(APPEND ${name}.sources "dx/val/gl/wgl/system.c")
list(APPEND ${name}.headers "dx/val/gl/wgl/system.h")
list(APPEND ${name}.sources "dx/val/gl/wgl/system_factory.c")
list(APPEND ${name}.headers "dx/val/gl/wgl/system_factory.h")
list(APPEND ${name}.sources "dx/val/gl/wgl/gl-includes.i")

list(APPEND ${name}.sources "dx/val/gl/wgl/Helper.c")
list(APPEND ${name}.headers "dx/val/gl/wgl/Helper.h")
list(APPEND ${name}.sources "dx/val/gl/wgl/WglFunctions.c")
list(APPEND ${name}.headers "dx/val/gl/wgl/WglFunctions.h")
list(APPEND ${name}.sources "dx/val/gl/GlFunctions.c")
list(APPEND ${name}.headers "dx/val/gl/GlFunctions.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "Core.Engine")
