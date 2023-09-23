# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.aal.al")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

# "val/aal/al.(c|h)"
list(APPEND ${name}.sources "dx/aal/al.c")
list(APPEND ${name}.headers "dX/aal/al.h")

# The folder "val/aal/al".
list(APPEND ${name}.sources "dx/aal/al/context.c")
list(APPEND ${name}.headers "dx/aal/al/context.h")
list(APPEND ${name}.sources "dx/aal/al/system.c")
list(APPEND ${name}.headers "dx/aal/al/system.h")
list(APPEND ${name}.sources "dx/aal/al/system_factory.c")
list(APPEND ${name}.headers "dx/aal/al/system_factory.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

dx_end_project(${name})

target_link_libraries(${name} "dailies.engine")
