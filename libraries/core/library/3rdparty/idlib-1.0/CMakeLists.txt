# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# This file is licensed under MIT license.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/language.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/detect_instruction_set_architecture.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/detect_operating_system.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/detect_void_pointer_size.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/detect_compiler.cmake)

project(idlib LANGUAGES C)

add_library(idlib STATIC)
set(idlib-headers "")
set(idlib-sources "")

idlib_detect_compiler(idlib IDLIB_LANGUAGE_ID_C)

idlib_detect_instruction_set_architecture(idlib)

idlib_detect_operating_system(idlib)

###############################################################################################################################################################

if (${idlib.COMPILER_C_ID} EQUAL ${IDLIB_COMPILER_C_ID_MSVC})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_MSVC")
elseif (${idlib.COMPILER_C_ID} EQUAL ${IDLIB_COMPILER_C_ID_GCC})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_GCC")
elseif (${idlib.COMPILER_C_ID} EQUAL ${IDLIB_COMPILER_C_ID_UNKNOWN})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_UNKNOWN")
else()
  message(FATAL_ERROR "C compiler detection not executed")
endif()

if (${idlib.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_X64})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_X64")
elseif (${idlib.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_X86})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_X86")
elseif (${idlib.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_UNKNOWN})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_UNKNOWN")
else()
  message(FATAL_ERROR "instruction set architecture not executed")
endif()

if (${idlib.OPERATING_SYSTEM_ID} EQUAL ${IDLIB_OPERATING_SYSTEM_ID_WINDOWS})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_WINDOWS")
elseif (${idlib.OPERATING_SYSTEM_ID} EQUAL ${IDLIB_OPERATING_SYSTEM_ID_LINUX})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_LINUX")
elseif (${idlib.OPERATING_SYSTEM_ID} EQUAL ${IDLIB_OPERATING_SYSTEM_ID_UNKNOWN})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_UNKNOWN")
else()
  message(FATAL_ERROR "operating system detection not executed")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includes/configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h)

target_include_directories(idlib PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/includes)
list(APPEND idlib-headers "${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h")

###############################################################################################################################################################

list(APPEND idlib-headers "${CMAKE_CURRENT_SOURCE_DIR}/includes/idlib.h")
list(APPEND idlib-sources "${CMAKE_CURRENT_SOURCE_DIR}/sources/idlib.c")

list(APPEND idlib-headers "${CMAKE_CURRENT_SOURCE_DIR}/includes/scalar.h")
list(APPEND idlib-sources "${CMAKE_CURRENT_SOURCE_DIR}/sources/scalar.c")

list(APPEND idlib-headers "${CMAKE_CURRENT_SOURCE_DIR}/includes/matrix_4x4.h")
list(APPEND idlib-sources "${CMAKE_CURRENT_SOURCE_DIR}/sources/matrix_4x4.c")

list(APPEND idlib-headers "${CMAKE_CURRENT_SOURCE_DIR}/includes/vector_2.h")
list(APPEND idlib-sources "${CMAKE_CURRENT_SOURCE_DIR}/sources/vector_2.c")

list(APPEND idlib-headers "${CMAKE_CURRENT_SOURCE_DIR}/includes/vector_3.h")
list(APPEND idlib-sources "${CMAKE_CURRENT_SOURCE_DIR}/sources/vector_3.c")

list(APPEND idlib-headers "${CMAKE_CURRENT_SOURCE_DIR}/includes/version.h")
list(APPEND idlib-sources "${CMAKE_CURRENT_SOURCE_DIR}/sources/version.c")

# For MSVC: Turn on several warnings.
if(MSVC)

  set(idlib_compile_options "")

  # C4090:  'opertion' : different 'modifier' qualifiers
  list(APPEND idlib_compile_options "/we4090")
  # C4133: 'function' incompatible types - from 'type1' to 'type2'
  list(APPEND idlib_compile_options "/we4133")
  # C4020_ 'function': too many actual parameters
  list(APPEND idlib_compile_options "/we4020")
  # C4013: 'function': undefined; assuming extern returning int
  list(APPEND idlib_compile_options "/we4013")
  # C4024: 'function': different types for formal and actual parameter 'number'
  list(APPEND idlib_compile_options "/we4024")
  # C4716: 'function' must return a value
  list(APPEND idlib_compile_options "/we4716")
  # C4013: 'function' undefined; assuming extern returning int
  list(APPEND idlib_compile_options "/we4013")

  # The quotes around "${idlib_compile_options}" concatenate the list elements to a single string separated by semicolons.
  set_source_files_properties(${idlib-sources} PROPERTIES COMPILE_OPTIONS "${idlib_compile_options}")
  set_source_files_properties(${idlib-headers} PROPERTIES COMPILE_OPTIONS "${idlib_compile_options}")

endif()

target_include_directories(idlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/includes")
target_sources(idlib PRIVATE ${idlib-headers} ${idlib-sources})
