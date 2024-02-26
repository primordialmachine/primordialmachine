# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

project(dailies LANGUAGES C)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLibrary.cmake)

add_subdirectory(plugins)
add_subdirectory(libraries)
add_subdirectory(player)
add_subdirectory(tools)
#add_subdirectory(website)
#add_subdirectory(experimental/pixie)
#add_subdirectory(experimental/service)
add_subdirectory(experimental/Machine)
