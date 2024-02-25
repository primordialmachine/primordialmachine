# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "Core.Assets")
set(${name}.kind "static-library")
set(${name}.languages "C")
# "external" contains the header files of the "wic-plugin".
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/../external")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

list(APPEND ${name}.sources "dx/assets.c")
list(APPEND ${name}.headers "dx/assets.h")

# "assets" folder.
list(APPEND ${name}.sources "dx/assets/mesh/generators.c")
list(APPEND ${name}.headers "dx/assets/mesh/generators.h")

list(APPEND ${name}.sources "dx/assets/mesh_operation.c")
list(APPEND ${name}.headers "dx/assets/mesh_operation.h")
list(APPEND ${name}.sources "dx/assets/mesh_operations/set_vertex_colors.c")
list(APPEND ${name}.headers "dx/assets/mesh_operations/set_vertex_colors.h")

list(APPEND ${name}.sources "Core/Assets/ColorRgbN8.c")
list(APPEND ${name}.headers "Core/Assets/ColorRgbN8.h")
list(APPEND ${name}.sources "Core/Assets/Context.c")
list(APPEND ${name}.headers "Core/Assets/Context.h")
list(APPEND ${name}.sources "Core/Assets/Def.c")
list(APPEND ${name}.headers "Core/Assets/Def.h")
list(APPEND ${name}.sources "Core/Assets/Image.c")
list(APPEND ${name}.headers "Core/Assets/Image.h")
list(APPEND ${name}.sources "Core/Assets/ImageOperation.c")
list(APPEND ${name}.headers "Core/Assets/ImageOperation.h")
list(APPEND ${name}.sources "Core/Assets/ImageOperations/CheckerboardPatternFill.c")
list(APPEND ${name}.headers "Core/Assets/ImageOperations/CheckerboardPatternFill.h")
list(APPEND ${name}.inlays  "Core/Assets/ImageOperations/CheckerboardPatternFillImpl.i")
list(APPEND ${name}.sources "Core/Assets/ImageOperations/ColorFill.c")
list(APPEND ${name}.headers "Core/Assets/ImageOperations/ColorFill.h")
list(APPEND ${name}.sources "Core/Assets/ImageOperations/MirrorHorizontal.c")
list(APPEND ${name}.headers "Core/Assets/ImageOperations/MirrorHorizontal.h")
list(APPEND ${name}.inlays  "Core/Assets/ImageOperations/MirrorHorizontalImpl.i")
list(APPEND ${name}.sources "Core/Assets/ImageOperations/MirrorVertical.c")
list(APPEND ${name}.headers "Core/Assets/ImageOperations/MirrorVertical.h")
list(APPEND ${name}.inlays  "Core/Assets/ImageOperations/MirrorVerticalImpl.i")
list(APPEND ${name}.sources "Core/Assets/Optics.c")
list(APPEND ${name}.headers "Core/Assets/Optics.h")
list(APPEND ${name}.sources "Core/Assets/PerspectiveOptics.c")
list(APPEND ${name}.headers "Core/Assets/PerspectiveOptics.h")
list(APPEND ${name}.sources "Core/Assets/OrthographicOptics.c")
list(APPEND ${name}.headers "Core/Assets/OrthographicOptics.h")
list(APPEND ${name}.sources "Core/Assets/Ref.c")
list(APPEND ${name}.headers "Core/Assets/Ref.h")
list(APPEND ${name}.sources "Core/Assets/System.c")
list(APPEND ${name}.headers "Core/Assets/System.h")
list(APPEND ${name}.sources "Core/Assets/SystemFactory.c")
list(APPEND ${name}.headers "Core/Assets/SystemFactory.h")
list(APPEND ${name}.sources "Core/Assets/Matrix4x4R32.c")
list(APPEND ${name}.headers "Core/Assets/Matrix4x4R32.h")
list(APPEND ${name}.sources "Core/Assets/Vector3R32.c")
list(APPEND ${name}.headers "Core/Assets/Vector3R32.h")
list(APPEND ${name}.sources "Core/Assets/Material.c")
list(APPEND ${name}.headers "Core/Assets/Material.h")
list(APPEND ${name}.sources "Core/Assets/MaterialController.c")
list(APPEND ${name}.headers "Core/Assets/Materialcontroller.h")
list(APPEND ${name}.sources "Core/Assets/MaterialControllers/AmbientColor.c")
list(APPEND ${name}.headers "Core/Assets/Materialcontrollers/AmbientColor.h")
list(APPEND ${name}.sources "Core/Assets/Viewer.c")
list(APPEND ${name}.headers "Core/Assets/Viewer.h")
list(APPEND ${name}.sources "Core/Assets/ViewerController.c")
list(APPEND ${name}.headers "Core/Assets/ViewerController.h")
list(APPEND ${name}.sources "Core/Assets/ViewerControllers/RotateY.c")
list(APPEND ${name}.headers "Core/Assets/ViewerControllers/RotateY.h")

list(APPEND ${name}.sources "dx/assets/scene.c")
list(APPEND ${name}.headers "dx/assets/scene.h")


list(APPEND ${name}.sources "dx/assets/texture.c")
list(APPEND ${name}.headers "dx/assets/texture.h")

# assets - mesh & mesh instance
list(APPEND ${name}.sources "dx/assets/mesh.c")
list(APPEND ${name}.headers "dx/assets/mesh.h")
list(APPEND ${name}.sources "dx/assets/mesh_instance.c")
list(APPEND ${name}.headers "dx/assets/mesh_instance.h")
# assets - viewer instance
list(APPEND ${name}.sources "dx/assets/viewer_instance.c")
list(APPEND ${name}.headers "dx/assets/viewer_instance.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "Core")
