# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.asset-definition-language")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
# "external contains the "wic-plugins" and "font-loader-plugin" header files.
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

# "adl.(c|h)" include.
list(APPEND ${name}.sources "dx/asset_definition_language.c")
list(APPEND ${name}.headers "dx/asset_definition_language.h")

# "adl" folder.
list(APPEND ${name}.sources "dx/adl/diagnostics.c")
list(APPEND ${name}.headers "dx/adl/diagnostics.h")

# symbols.
list(APPEND ${name}.sources "dx/adl/symbols.c")
list(APPEND ${name}.headers "dx/adl/symbols.h")

# phases.
list(APPEND ${name}.sources "dx/adl/enter.c" "dx/adl/resolve.c")
list(APPEND ${name}.headers "dx/adl/enter.h" "dx/adl/resolve.h")

list(APPEND ${name}.sources "dx/adl/type_handler.c")
list(APPEND ${name}.headers "dx/adl/type_handler.h")
list(APPEND ${name}.sources "dx/adl/context.c")
list(APPEND ${name}.headers "dx/adl/context.h")

list(APPEND ${name}.sources "dx/adl/names.c")
list(APPEND ${name}.headers "dx/adl/names.h")
list(APPEND ${name}.inlays  "dx/adl/names.i")
list(APPEND ${name}.sources "dx/adl/parser.c")
list(APPEND ${name}.headers "dx/adl/parser.h")

list(APPEND ${name}.sources "dx/adl/type_handlers/viewer.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/viewer.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/viewer_instance.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/viewer_instance.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/viewer_controllers_rotate_y.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/viewer_controllers_rotate_y.h")

list(APPEND ${name}.sources "dx/adl/type_handlers/material.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/material.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/material_controllers.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/material_controllers.h")

list(APPEND ${name}.sources "dx/adl/type_handlers/texture.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/texture.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/mesh.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/mesh.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/image.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/image.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/mesh_instance.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/mesh_instance.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/mesh_operations.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/mesh_operations.h")

list(APPEND ${name}.sources "dx/adl/type_handlers/color.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/color.h")

list(APPEND ${name}.sources "dx/adl/type_handlers/image_operations_checkerboard_pattern_fill.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/image_operations_checkerboard_pattern_fill.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/image_operations_color_fill.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/image_operations_color_fill.h")

list(APPEND ${name}.sources "dx/adl/type_handlers/optics_perspective.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/optics_perspective.h")
list(APPEND ${name}.sources "dx/adl/type_handlers/optics_orthographic.c")
list(APPEND ${name}.headers "dx/adl/type_handlers/optics_orthographic.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "dailies.core" "dailies.assets" "dailies.data-definition-language" "dailies.document-definition-language")
