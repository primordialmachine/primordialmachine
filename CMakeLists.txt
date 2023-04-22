# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

project(dailies LANGUAGES C)
option(BUILD_DOCUMENTATION "Build the documentation" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLibrary.cmake)

add_subdirectory(plugins)
add_subdirectory(libraries)
add_subdirectory(player)
