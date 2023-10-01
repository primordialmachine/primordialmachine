# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.aal.al")
set(${name}.kind "static-library")
set(${name}.languages "C")
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/external/openal/includes")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

add_library(openallibrary STATIC IMPORTED)

set_target_properties(openallibrary PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-minsizerel/openal/lib/OpenAL32.lib)
set_target_properties(openallibrary PROPERTIES IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-release/openal/lib/OpenAL32.lib)
set_target_properties(openallibrary PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-debug/openal/lib/OpenAL32.lib)
set_target_properties(openallibrary PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-relwithdebinfo/openal/lib/OpenAL32.lib)

set_target_properties(openallibrary PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-$<CONFIG>/openal/include/)

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

if (WIN32)
  target_link_libraries(${name} openallibrary winmm.lib "dailies.engine")
else()
  target_link_libraries(${name} openallibrary "dailies.engine")
endif()
