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
list(APPEND ${name}.sources "sources/Core/Tests/Signals/Receiver.c")
list(APPEND ${name}.headers "sources/Core/Tests/Signals/Receiver.h")
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

list(APPEND ${name}.sources "sources/Core/Tests/Utf8/InlineUtf8IteratorTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/Utf8/InlineUtf8IteratorTests.h")
list(APPEND ${name}.sources "sources/Core/Tests/Utf8/InlineByteIteratorTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/Utf8/InlineByteIteratorTests.h")

list(APPEND ${name}.sources "sources/Core/Tests/FileSystem/getFileContentsTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/FileSystem/getFileContentsTests.h")
list(APPEND ${name}.sources "sources/Core/Tests/FileSystem/getFileContentsCallbackTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/FileSystem/getFileContentsCallbackTests.h")
list(APPEND ${name}.sources "sources/Core/Tests/FileSystem/getFileContentsMemoryMappedTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/FileSystem/getFileContentsMemoryMappedTests.h")
list(APPEND ${name}.sources "sources/Core/Tests/FileSystem/getFileContentsMemoryMappedCallbackTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/FileSystem/getFileContentsMemoryMappedCallbackTests.h")

list(APPEND ${name}.sources "sources/Core/Tests/FileSystem/setFileContentsTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/FileSystem/setFileContentsTests.h")
list(APPEND ${name}.sources "sources/Core/Tests/FileSystem/setFileContentsMemoryMappedTests.c")
list(APPEND ${name}.headers "sources/Core/Tests/FileSystem/setFileContentsMemoryMappedTests.h")


dx_end_project(${name})

target_link_libraries(${name} Core)

# Copy the assets to the current binary directory.
file(GLOB_RECURSE files_to_copy RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*" )

foreach (file_to_copy ${files_to_copy})
  add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/assets/${file_to_copy}"
                                                  "${CMAKE_CURRENT_BINARY_DIR}/assets/${file_to_copy}"
    COMMAND_EXPAND_LISTS
  )
endforeach()

