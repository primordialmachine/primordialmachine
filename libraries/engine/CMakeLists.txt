# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.engine")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
# "external contains the "wic-plugins" and "font-loader-plugin" header files.
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

# master files.
list(APPEND ${name}.sources "dx/engine.c")
list(APPEND ${name}.headers "dx/engine.h")

# "dx/val.(c|h)"
list(APPEND ${name}.sources "dx/val.c")
list(APPEND ${name}.headers "dx/val.h")

# "dx/console.(c|h)"
list(APPEND ${name}.sources "dx/console.c")
list(APPEND ${name}.headers "dx/console.h")

# "dx/cl_interpreter.(c|h)"
list(APPEND ${name}.sources "dx/cl_interpreter.c")
list(APPEND ${name}.headers "dx/cl_interpreter.h")

# "dx/presenter.(c|h)"
list(APPEND ${name}.sources "dx/presenter.c")
list(APPEND ${name}.headers "dx/presenter.h")

# "dx/scene_presenter.(c|h)"
list(APPEND ${name}.sources "dx/scene_presenter.c")
list(APPEND ${name}.headers "dx/scene_presenter.h")

# "dx/font_manager.(c|h)"
list(APPEND ${name}.sources "dx/font_manager.c")
list(APPEND ${name}.headers "dx/font_manager.h")

# "dx/application_presenter.(c|h)"
list(APPEND ${name}.sources "dx/application_presenter.c")
list(APPEND ${name}.headers "dx/application_presenter.h")

# "dx/font_manager.(c|h)"
list(APPEND ${name}.sources "dx/font_manager.c")
list(APPEND ${name}.headers "dx/font_manager.h")

# "dx/application.(c|h)"
list(APPEND ${name}.sources "dx/application.c")
list(APPEND ${name}.headers "dx/application.h")

# "dx/fps_counter.(c|h)"
list(APPEND ${name}.sources "dx/fps_counter.c")
list(APPEND ${name}.headers "dx/fps_counter.h")

# "aal" folder.
list(APPEND ${name}.sources "dx/aal/system.c")
list(APPEND ${name}.headers "dx/aal/system.h")
list(APPEND ${name}.sources "dx/aal/system_factory.c")
list(APPEND ${name}.headers "dx/aal/system_factory.h")
list(APPEND ${name}.sources "dx/aal/context.c")
list(APPEND ${name}.headers "dx/aal/context.h")

# "val" folder.
list(APPEND ${name}.sources "dx/val/system.c")
list(APPEND ${name}.headers "dx/val/system.h")
list(APPEND ${name}.sources "dx/val/system_factory.c")
list(APPEND ${name}.headers "dx/val/system_factory.h")
list(APPEND ${name}.sources "dx/val/context.c")
list(APPEND ${name}.headers "dx/val/context.h")

list(APPEND ${name}.sources "dx/val/texture.c" "dx/val/material.c" "dx/val/mesh.c" "dx/val/mesh_instance.c" "dx/val/viewer.c")
list(APPEND ${name}.headers "dx/val/texture.h" "dx/val/material.h" "dx/val/mesh.h" "dx/val/mesh_instance.h" "dx/val/viewer.h")
list(APPEND ${name}.sources "dx/val/program_text.c" "dx/val/command.c" "dx/val/program.c" "dx/val/cbinding.c" "dx/val/vbinding.c" "dx/val/buffer.c")
list(APPEND ${name}.headers "dx/val/program_text.h" "dx/val/command.h" "dx/val/program.h" "dx/val/cbinding.h" "dx/val/vbinding.h" "dx/val/buffer.h")

#
list(APPEND ${name}.sources "dx/ui/widget.c")
list(APPEND ${name}.headers "dx/ui/widget.h")
list(APPEND ${name}.sources "dx/ui/group.c")
list(APPEND ${name}.headers "dx/ui/group.h")
list(APPEND ${name}.sources "dx/ui/text.c")
list(APPEND ${name}.headers "dx/ui/text.h")
list(APPEND ${name}.sources "dx/ui/manager.c")
list(APPEND ${name}.headers "dx/ui/manager.h")

list(APPEND ${name}.sources "dx/font_presenter.c")
list(APPEND ${name}.headers "dx/font_presenter.h")
list(APPEND ${name}.sources "dx/rectangle_presenter.c")
list(APPEND ${name}.headers "dx/rectangle_presenter.h")
list(APPEND ${name}.sources "dx/utilities_2d.c")
list(APPEND ${name}.headers "dx/utilities_2d.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "dailies.core" "dailies.assets" "dailies.asset-definition-language" "dailies.data-definition-language" "dailies.document-definition-language")
