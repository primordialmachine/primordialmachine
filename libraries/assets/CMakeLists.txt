# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.assets")
set(${name}.kind "static-library")
set(${name}.languages "C")
# "external" contains the header files of the "wic-plugin".
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

list(APPEND ${name}.sources "dx/assets.c")
list(APPEND ${name}.headers "dx/assets.h")

# "assets" folder.
list(APPEND ${name}.sources "dx/assets/context.c")
list(APPEND ${name}.headers "dx/assets/context.h")
list(APPEND ${name}.sources "dx/assets/system.c")
list(APPEND ${name}.headers "dx/assets/system.h")
list(APPEND ${name}.sources "dx/assets/system_factory.c")
list(APPEND ${name}.headers "dx/assets/system_factory.h")

list(APPEND ${name}.sources "dx/assets/mesh/generators.c")
list(APPEND ${name}.headers "dx/assets/mesh/generators.h")

list(APPEND ${name}.sources "dx/assets/mesh_operation.c")
list(APPEND ${name}.headers "dx/assets/mesh_operation.h")
list(APPEND ${name}.sources "dx/assets/mesh_operations/set_vertex_colors.c")
list(APPEND ${name}.headers "dx/assets/mesh_operations/set_vertex_colors.h")

list(APPEND ${name}.sources "dx/assets/image_operation.c")
list(APPEND ${name}.headers "dx/assets/image_operation.h")
list(APPEND ${name}.sources "dx/assets/image_operations/color_fill.c")
list(APPEND ${name}.headers "dx/assets/image_operations/color_fill.h")

list(APPEND ${name}.sources "dx/assets/image_operations/checkerboard_pattern_fill.c")
list(APPEND ${name}.headers "dx/assets/image_operations/checkerboard_pattern_fill.h")
list(APPEND ${name}.inlays  "dx/assets/image_operations/checkerboard_pattern_fill_impl.i")

list(APPEND ${name}.sources "dx/assets/image_operations/mirror_vertical.c")
list(APPEND ${name}.headers "dx/assets/image_operations/mirror_vertical.h")
list(APPEND ${name}.inlays  "dx/assets/image_operations/mirror_vertical_impl.i")

list(APPEND ${name}.sources "dx/assets/image_operations/mirror_horizontal.c")
list(APPEND ${name}.headers "dx/assets/image_operations/mirror_horizontal.h")
list(APPEND ${name}.inlays  "dx/assets/image_operations/mirror_horizontal_impl.i")

list(APPEND ${name}.sources "dx/assets/scene.c")
list(APPEND ${name}.headers "dx/assets/scene.h")

list(APPEND ${name}.sources "dx/assets/material.c")
list(APPEND ${name}.headers "dx/assets/material.h")
list(APPEND ${name}.sources "dx/assets/material_controller.c")
list(APPEND ${name}.headers "dx/assets/material_controller.h")

list(APPEND ${name}.sources "dx/assets/texture.c")
list(APPEND ${name}.headers "dx/assets/texture.h")

list(APPEND ${name}.sources "dx/assets/optics.c")
list(APPEND ${name}.headers "dx/assets/optics.h")

list(APPEND ${name}.sources "dx/assets/reference.c")
list(APPEND ${name}.headers "dx/assets/reference.h")

# assets - mesh & mesh instance
list(APPEND ${name}.sources "dx/assets/mesh.c")
list(APPEND ${name}.headers "dx/assets/mesh.h")
list(APPEND ${name}.sources "dx/assets/mesh_instance.c")
list(APPEND ${name}.headers "dx/assets/mesh_instance.h")
# assets - viewer & viewer instance & viewer controllers
list(APPEND ${name}.sources "dx/assets/viewer.c")
list(APPEND ${name}.headers "dx/assets/viewer.h")
list(APPEND ${name}.sources "dx/assets/viewer_instance.c")
list(APPEND ${name}.headers "dx/assets/viewer_instance.h")
list(APPEND ${name}.sources "dx/assets/viewer_controller.c")
list(APPEND ${name}.headers "dx/assets/viewer_controller.h")

list(APPEND ${name}.sources "dx/assets/image.c")
list(APPEND ${name}.headers "dx/assets/image.h")

list(APPEND ${name}.sources "dx/assets/matrix_4x4_f32.c")
list(APPEND ${name}.headers "dx/assets/matrix_4x4_f32.h")
list(APPEND ${name}.sources "dx/assets/vector_3_f32.c")
list(APPEND ${name}.headers "dx/assets/vector_3_f32.h")
list(APPEND ${name}.sources "dx/assets/color_rgb_n8.c")
list(APPEND ${name}.headers "dx/assets/color_rgb_n8.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "Core")
