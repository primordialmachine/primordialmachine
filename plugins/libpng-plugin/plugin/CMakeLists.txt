# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "libpng-plugin")
set(${name}.languages "C")
set(${name}.kind "dynamic-library")
set(${name}.doxygen_enabled OFF)
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")

dx_begin_project(${name})

list(APPEND ${name}.sources "implementation/read-image.c")
list(APPEND ${name}.sources "implementation/write-image.c")

list(APPEND ${name}.sources "implementation/common.c")
list(APPEND ${name}.headers "implementation/common.h")
list(APPEND ${name}.headers "implementation/libpng-plugin.h")

source_group(TREE ${${name}.source_directory} FILES ${${name}.sources} ${${name}.headers} ${${name}.inlays} ${${name}.readmes} ${${name}.asms})

dx_end_project(${name})

##################################################################################################################################################################################
# libpng
add_library(libpnglibrary STATIC IMPORTED)

set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-minsizerel/libpng/lib/libpng.lib)
set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-release/libpng/lib/libpng.lib)
set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-debug/libpng/lib/libpng.lib)
set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-relwithdebinfo/libpng/lib/libpng.lib)

set_target_properties(libpnglibrary PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-$<CONFIG>/libpng/include)
##################################################################################################################################################################################


##################################################################################################################################################################################
# zlib
add_library(zliblibrary STATIC IMPORTED)

set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-minsizerel/zlib/lib/zlib.lib)
set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-release/zlib/lib/zlib.lib)
set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-debug/zlib/lib/zlib.lib)
set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-relwithdebinfo/zlib/lib/zlib.lib)

set_target_properties(zliblibrary PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.target_architecture}-$<CONFIG>/zlib/include")
##################################################################################################################################################################################

target_link_libraries(${name} libpnglibrary zliblibrary)
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/implementation)
