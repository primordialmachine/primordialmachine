# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(name "Core.Tests")
set(${name}.kind "executable")
set(${name}.languages "C")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}/sources")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}/sources")

dx_begin_project(${name})

list(APPEND ${name}.sources "sources/Core/Tests/Main.c")
list(APPEND ${name}.sources "sources/Core/Tests/Signals/ObjectA.c")
list(APPEND ${name}.headers "sources/Core/Tests/Signals/ObjectA.h")
list(APPEND ${name}.sources "sources/Core/Tests/Signals/ObjectB.c")
list(APPEND ${name}.headers "sources/Core/Tests/Signals/ObjectB.h")
list(APPEND ${name}.sources "sources/Core/Tests/Signals/ObjectC.c")
list(APPEND ${name}.headers "sources/Core/Tests/Signals/ObjectC.h")
list(APPEND ${name}.sources "sources/Core/Tests/Signals/Test1.c")
list(APPEND ${name}.headers "sources/Core/Tests/Signals/Test1.h")

list(APPEND ${name}.sources "sources/Core/Tests/countLeadingZeroes.c")
list(APPEND ${name}.headers "sources/Core/Tests/countLeadingZeroes.h")
list(APPEND ${name}.sources "sources/Core/Tests/fundamentalTypes.c")
list(APPEND ${name}.headers "sources/Core/Tests/fundamentalTypes.h")
list(APPEND ${name}.sources "sources/Core/Tests/nextPowerOfTwo.c")
list(APPEND ${name}.headers "sources/Core/Tests/nextPowerOfTwo.h")

list(APPEND ${name}.sources "sources/Core/Tests/safeAddNx.c")
list(APPEND ${name}.headers "sources/Core/Tests/safeAddNx.h")
list(APPEND ${name}.sources "sources/Core/Tests/safeMulIx.c")
list(APPEND ${name}.headers "sources/Core/Tests/safeMulIx.h")
list(APPEND ${name}.sources "sources/Core/Tests/safeMulNx.c")
list(APPEND ${name}.headers "sources/Core/Tests/safeMulNx.h")
list(APPEND ${name}.sources "sources/Core/Tests/clearBits.c")
list(APPEND ${name}.headers "sources/Core/Tests/clearBits.h")

dx_end_project(${name})

target_link_libraries(${name} Core)
