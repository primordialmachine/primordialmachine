# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "wic-plugin")
set(${name}.languages "C")
set(${name}.kind "dynamic-library")
set(${name}.private_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/implementation)
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.doxygen_enabled OFF)

dx_begin_project(${name})

list(APPEND ${name}.sources "implementation/read-image.c")
list(APPEND ${name}.sources "implementation/write-image.c")

list(APPEND ${name}.sources "implementation/common.c")
list(APPEND ${name}.headers "implementation/common.h")
list(APPEND ${name}.headers "implementation/wic-plugin.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources} ${${name}.headers})

dx_end_project(${name})
