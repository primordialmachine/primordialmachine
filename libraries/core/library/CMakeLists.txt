# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "Core")
set(${name}.kind "static-library")
set(${name}.languages "C" "ASM_MASM") # Enable C and MASM.
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# configuration files.
list(APPEND ${name}.sources "dx/core/configuration.c")
list(APPEND ${name}.headers "dx/core/configuration.h")

# master files.
list(APPEND ${name}.sources "dx/core.c")
list(APPEND ${name}.headers "dx/core.h")

# "core" folder.
list(APPEND ${name}.sources "dx/core/context.c")
list(APPEND ${name}.headers "dx/core/context.h")
list(APPEND ${name}.sources "dx/core/system.c")
list(APPEND ${name}.headers "dx/core/system.h")
list(APPEND ${name}.sources "dx/core/system_factory.c")
list(APPEND ${name}.headers "dx/core/system_factory.h")

list(APPEND ${name}.sources "dx/core/annotations.c")
list(APPEND ${name}.headers "dx/core/annotations.h")
list(APPEND ${name}.sources "dx/core/_get_best_array_size.c")
list(APPEND ${name}.headers "dx/core/_get_best_array_size.h")

list(APPEND ${name}.sources "dx/core/signals.c")
list(APPEND ${name}.headers "dx/core/signals.h")
list(APPEND ${name}.sources "dx/core/signals.library.c")
list(APPEND ${name}.headers "dx/core/signals.library.h")
list(APPEND ${name}.sources "dx/core/math.c")
list(APPEND ${name}.headers "dx/core/math.h")

list(APPEND ${name}.sources "dx/core/core.c")
list(APPEND ${name}.headers "dx/core/core.h")
list(APPEND ${name}.sources "dx/core/os.c")
list(APPEND ${name}.headers "dx/core/os.h")

list(APPEND ${name}.sources "dx/core/text/anchor.c")
list(APPEND ${name}.headers "dx/core/text/anchor.h")
list(APPEND ${name}.sources "dx/core/text/bounds_type.c")
list(APPEND ${name}.headers "dx/core/text/bounds_type.h")

list(APPEND ${name}.sources "dx/core/_string_format.c")
list(APPEND ${name}.headers "dx/core/_string_format.h")
list(APPEND ${name}.sources "dx/core/string_buffer.c")
list(APPEND ${name}.headers "dx/core/string_buffer.h")
list(APPEND ${name}.sources "dx/core/_is_utf8_sequence.c")
list(APPEND ${name}.headers "dx/core/_is_utf8_sequence.h")

list(APPEND ${name}.sources "dx/core/inline_object_array.c")
list(APPEND ${name}.headers "dx/core/inline_object_array.h")
list(APPEND ${name}.sources "dx/core/inline_pointer_deque.c")
list(APPEND ${name}.headers "dx/core/inline_pointer_deque.h")
list(APPEND ${name}.sources "dx/core/inline_pointer_hashmap.c")
list(APPEND ${name}.headers "dx/core/inline_pointer_hashmap.h")
list(APPEND ${name}.sources "dx/core/object_array.c")
list(APPEND ${name}.headers "dx/core/object_array.h")
list(APPEND ${name}.sources "dx/core/visuals.c")
list(APPEND ${name}.headers "dx/core/visuals.h")

list(APPEND ${name}.sources "dx/core/keyboard_state.c")
list(APPEND ${name}.headers "dx/core/keyboard_state.h")
list(APPEND ${name}.sources "dx/core/mouse_state.c")
list(APPEND ${name}.headers "dx/core/mouse_state.h")

list(APPEND ${name}.sources "dx/core/Object.c")
list(APPEND ${name}.headers "dx/core/Object.h")
list(APPEND ${name}.sources "dx/core/Object.library.c")
list(APPEND ${name}.headers "dx/core/Object.library.h")

list(APPEND ${name}.sources "dx/core/TypeSystem.c")
list(APPEND ${name}.headers "dx/core/TypeSystem.h")
list(APPEND ${name}.sources "dx/core/TypeSystem.library.c")
list(APPEND ${name}.headers "dx/core/TypeSystem.library.h")

list(APPEND ${name}.sources "dx/core/MessageQueue.c")
list(APPEND ${name}.headers "dx/core/MessageQueue.h")

# "README.md".
list(APPEND ${name}.readmes "README.md")

# Main include file.
list(APPEND ${name}.sources "Core/Include.c")
list(APPEND ${name}.headers "Core/Include.h")

# "Configurations".
list(APPEND ${name}.sources "Core/Configuration.c")
list(APPEND ${name}.headers "Core/Configuration.h")

# "Numerics".
list(APPEND ${name}.sources "Core/Numerics.c")
list(APPEND ${name}.headers "Core/Numerics.h")

# "Lexical Analyses".
list(APPEND ${name}.sources "Core/Scanner.c")
list(APPEND ${name}.headers "Core/Scanner.h")

# "Strings".
list(APPEND ${name}.sources "Core/String.c")
list(APPEND ${name}.headers "Core/String.h")
list(APPEND ${name}.sources "Core/StringIterator.c")
list(APPEND ${name}.headers "Core/StringIterator.h")
list(APPEND ${name}.sources "Core/InlineByteIterator.c")
list(APPEND ${name}.headers "Core/InlineByteIterator.h")
list(APPEND ${name}.sources "Core/InlineUtf8Iterator.c")
list(APPEND ${name}.headers "Core/InlineUtf8Iterator.h")

# "low level operations"
list(APPEND ${name}.sources "Core/CountLeadingZeroes.c")
list(APPEND ${name}.headers "Core/CountLeadingZeroes.h")
list(APPEND ${name}.sources "Core/Hash.c")
list(APPEND ${name}.headers "Core/Hash.h")
list(APPEND ${name}.sources "Core/Memory.c")
list(APPEND ${name}.headers "Core/Memory.h")
list(APPEND ${name}.sources "Core/NextPowerOfTwo.c")
list(APPEND ${name}.headers "Core/NextPowerOfTwo.h")
list(APPEND ${name}.sources "Core/Bits.c")
list(APPEND ${name}.headers "Core/Bits.h")

# "Messages".
list(APPEND ${name}.sources "Core/Message.c")
list(APPEND ${name}.headers "Core/Message.h")
list(APPEND ${name}.sources "Core/EmitMessage.c")
list(APPEND ${name}.headers "Core/EmitMessage.h")
list(APPEND ${name}.sources "Core/ApplicationMessage.c")
list(APPEND ${name}.headers "Core/ApplicationMessage.h")
list(APPEND ${name}.sources "Core/ApplicationMessageKind.c")
list(APPEND ${name}.headers "Core/ApplicationMessageKind.h")

# "Runtime".
list(APPEND ${name}.sources "Core/Error.c")
list(APPEND ${name}.headers "Core/Error.h")
list(APPEND ${name}.sources "Core/FundamentalTypes.c")
list(APPEND ${name}.headers "Core/FundamentalTypes.h")
list(APPEND ${name}.sources "Core/Result.c")
list(APPEND ${name}.headers "Core/Result.h")

# "Text to number conversion".
list(APPEND ${name}.sources "Core/Convert.c")
list(APPEND ${name}.headers "Core/Convert.h")

# "Time".
list(APPEND ${name}.sources "Core/Time.c")
list(APPEND ${name}.headers "Core/Time.h")

# "Inline Collections".
list(APPEND ${name}.sources "Core/Collections/InlineArrayListN8.c")
list(APPEND ${name}.headers "Core/Collections/InlineArrayListN8.h")
list(APPEND ${name}.sources "Core/Collections/InlineArrayListP.c")
list(APPEND ${name}.headers "Core/Collections/InlineArrayListP.h")

# "Safe arithmetic".
list(APPEND ${name}.sources "Core/safeAddNx.c")
list(APPEND ${name}.headers "Core/safeAddNx.h")
list(APPEND ${name}.sources "Core/safeMulIx.c")
list(APPEND ${name}.headers "Core/safeMulIx.h")
list(APPEND ${name}.sources "Core/safeMulNx.c")
list(APPEND ${name}.headers "Core/safeMulNx.h")

# "File system".
list(APPEND ${name}.sources "Core/FileSystem/createDirectoryFile.c")
list(APPEND ${name}.headers "Core/FileSystem/createDirectoryFile.h")
list(APPEND ${name}.sources "Core/FileSystem/deleteRegularFile.c")
list(APPEND ${name}.headers "Core/FileSystem/deleteRegularFile.h")
list(APPEND ${name}.sources "Core/FileSystem/deleteDirectoryFile.c")
list(APPEND ${name}.headers "Core/FileSystem/deleteDirectoryFile.h")
list(APPEND ${name}.sources "Core/FileSystem/ExistingFilePolicy.c")
list(APPEND ${name}.headers "Core/FileSystem/ExistingFilePolicy.h")
list(APPEND ${name}.sources "Core/FileSystem/FileAccessMode.c")
list(APPEND ${name}.headers "Core/FileSystem/FileAccessMode.h")
list(APPEND ${name}.sources "Core/FileSystem/FileType.c")
list(APPEND ${name}.headers "Core/FileSystem/FileType.h")
list(APPEND ${name}.sources "Core/FileSystem/getFileContents.c")
list(APPEND ${name}.headers "Core/FileSystem/getFileContents.h")
list(APPEND ${name}.sources "Core/FileSystem/getFileType.c")
list(APPEND ${name}.headers "Core/FileSystem/getFileType.h")
list(APPEND ${name}.sources "Core/FileSystem/NonExistingFilePolicy.c")
list(APPEND ${name}.headers "Core/FileSystem/NonExistingFilePolicy.h")

# "Input".
list(APPEND ${name}.sources "Core/Input/InputMessageKind.c")
list(APPEND ${name}.headers "Core/Input/InputMessageKind.h")
list(APPEND ${name}.sources "Core/Input/InputMessage.c")
list(APPEND ${name}.headers "Core/Input/InputMessage.h")
list(APPEND ${name}.sources "Core/Input/KeyboardKey.c")
list(APPEND ${name}.headers "Core/Input/KeyboardKey.h")
list(APPEND ${name}.inlays  "Core/Input/KeyboardKey.i")
list(APPEND ${name}.sources "Core/Input/KeyboardKeyAction.c")
list(APPEND ${name}.sources "Core/Input/KeyboardKeyAction.h")
list(APPEND ${name}.sources "Core/Input/KeyboardKeyMessage.c")
list(APPEND ${name}.headers "Core/Input/KeyboardKeyMessage.h")
list(APPEND ${name}.sources "Core/Input/MouseButton.c")
list(APPEND ${name}.headers "Core/Input/MouseButton.h")
list(APPEND ${name}.inlays  "Core/Input/MouseButton.i")
list(APPEND ${name}.sources "Core/Input/MouseButtonMessage.c")
list(APPEND ${name}.sources "Core/Input/MouseButtonMessage.h")
list(APPEND ${name}.sources "Core/Input/MouseButtonAction.c")
list(APPEND ${name}.sources "Core/Input/MouseButtonAction.h")
list(APPEND ${name}.sources "Core/Input/MousePointerAction.c")
list(APPEND ${name}.sources "Core/Input/MousePointerAction.h")
list(APPEND ${name}.sources "Core/Input/MousePointerMessage.c")
list(APPEND ${name}.sources "Core/Input/MousePointerMessage.h")
list(APPEND ${name}.sources "Core/Input/ModifierKeys.c")
list(APPEND ${name}.headers "Core/Input/ModifierKeys.h")
list(APPEND ${name}.inlays  "Core/Input/ModifierKeys.i")

# "Visuals".
list(APPEND ${name}.sources "Core/Visuals/CullMode.c")
list(APPEND ${name}.headers "Core/Visuals/CullMode.h")
list(APPEND ${name}.sources "Core/Visuals/DepthCompareFunction.c")
list(APPEND ${name}.headers "Core/Visuals/DepthCompareFunction.h")
list(APPEND ${name}.sources "Core/Visuals/PixelFormat.c")
list(APPEND ${name}.headers "Core/Visuals/PixelFormat.h")
list(APPEND ${name}.sources "Core/Visuals/TextureAddressMode.c")
list(APPEND ${name}.headers "Core/Visuals/TextureAddressMode.h")
list(APPEND ${name}.sources "Core/Visuals/VertexFormat.c")
list(APPEND ${name}.headers "Core/Visuals/VertexFormat.h")
list(APPEND ${name}.sources "Core/Visuals/WindingMode.c")
list(APPEND ${name}.headers "Core/Visuals/WindingMode.h")
list(APPEND ${name}.sources "Core/Visuals/CanvasMessage.c")
list(APPEND ${name}.headers "Core/Visuals/CanvasMessage.h")
list(APPEND ${name}.sources "Core/Visuals/CanvasMessageKind.c")
list(APPEND ${name}.headers "Core/Visuals/CanvasMessageKind.h")
list(APPEND ${name}.sources "Core/Visuals/CanvasSizeChangedMessage.c")
list(APPEND ${name}.headers "Core/Visuals/CanvasSizeChangedMessage.h")
list(APPEND ${name}.sources "Core/Visuals/CanvasDpiChangedMessage.c")
list(APPEND ${name}.headers "Core/Visuals/CanvasDpiChangedMessage.h")
list(APPEND ${name}.sources "Core/Visuals/CanvasStateChangedMessage.c")
list(APPEND ${name}.headers "Core/Visuals/CanvasStateChangedMessage.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

#
add_files( ${name}.inlays  "dx/core/asm/${infix}/_umul*.i")
add_files( ${name}.sources "dx/core/asm/${infix}/_umul*.c" )
add_files( ${name}.headers "dx/core/asm/${infix}/_umul*.h" )
add_files( ${name}.asms    "dx/core/asm/${infix}/_umul*.asm" )
#
add_files( ${name}.inlays  "dx/core/asm/${infix}/_imul*.i")
add_files( ${name}.sources "dx/core/asm/${infix}/_imul*.c" )
add_files( ${name}.headers "dx/core/asm/${infix}/_imul*.h" )
add_files( ${name}.asms    "dx/core/asm/${infix}/_imul*.asm" )
#
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set_source_files_properties(
      ${${name}.asms}
      PROPERTIES
      COMPILE_FLAGS "/safeseh"
  )
endif()

dx_end_project(${name})
