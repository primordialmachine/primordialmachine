# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "libpng-plugin")

set (${name}.sources "")
set (${name}.headers "")

list(APPEND ${name}.sources "implementation/read-image.c")
list(APPEND ${name}.sources "implementation/write-image.c")

list(APPEND ${name}.sources "implementation/common.c")
list(APPEND ${name}.headers "implementation/common.h")
list(APPEND ${name}.headers "implementation/libpng-plugin.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources} ${${name}.headers})

project(${name} LANGUAGES C)

add_library(${name} MODULE ${${name}.sources} ${${name}.headers})

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

set(${name}.architecture "")

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  message( STATUS "detected x64 target architecture" )
  set(${name}.architecture "x64")
elseif( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  message( STATUS "detected x86 target architecture" )
  set(${name}.architecture "x86")
else()
  message( FATAL_ERROR "unable to detect architecture" )
endif()


##################################################################################################################################################################################
# libpng
add_library(libpnglibrary STATIC IMPORTED)

set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-minsizerel/libpng/lib/libpng.lib)
set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-release/libpng/lib/libpng.lib)
set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-debug/libpng/lib/libpng.lib)
set_target_properties(libpnglibrary PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-relwithdebinfo/libpng/lib/libpng.lib)

set_target_properties(libpnglibrary PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-$<CONFIG>/libpng/include)
##################################################################################################################################################################################


##################################################################################################################################################################################
# zlib
add_library(zliblibrary STATIC IMPORTED)

set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-minsizerel/zlib/lib/zlib.lib)
set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-release/zlib/lib/zlib.lib)
set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-debug/zlib/lib/zlib.lib)
set_target_properties(zliblibrary PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-relwithdebinfo/zlib/lib/zlib.lib)

set_target_properties(zliblibrary PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/${${name}.architecture}-$<CONFIG>/zlib/include")
##################################################################################################################################################################################

target_link_libraries(${name} libpnglibrary zliblibrary)
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/implementation)
