# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.core")
set(${name}.kind "static-library")
set(${name}.languages "C" "ASM_MASM") # Enable C and MASM.
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}")
dx_begin_project(${name})

# "README.md".
list(APPEND ${name}.readmes "README.md")

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

list(APPEND ${name}.sources "dx/core/error.c")
list(APPEND ${name}.headers "dx/core/error.h")

list(APPEND ${name}.sources "dx/core/core.c")
list(APPEND ${name}.headers "dx/core/core.h")
list(APPEND ${name}.sources "dx/core/os.c")
list(APPEND ${name}.headers "dx/core/os.h")

list(APPEND ${name}.sources "dx/core/string.c")
list(APPEND ${name}.headers "dx/core/string.h")
list(APPEND ${name}.sources "dx/core/string_iterator.c")
list(APPEND ${name}.headers "dx/core/string_iterator.h")
list(APPEND ${name}.sources "dx/core/_string_format.c")
list(APPEND ${name}.headers "dx/core/_string_format.h")
list(APPEND ${name}.sources "dx/core/string_buffer.c")
list(APPEND ${name}.headers "dx/core/string_buffer.h")
list(APPEND ${name}.sources "dx/core/_is_utf8_sequence.c")
list(APPEND ${name}.headers "dx/core/_is_utf8_sequence.h")

list(APPEND ${name}.sources "dx/core/scanner.c")
list(APPEND ${name}.headers "dx/core/scanner.h")

list(APPEND ${name}.sources "dx/core/math.c")
list(APPEND ${name}.headers "dx/core/math.h")
list(APPEND ${name}.sources "dx/core/byte_array.c")
list(APPEND ${name}.headers "dx/core/byte_array.h")
list(APPEND ${name}.sources "dx/core/inline_object_array.c")
list(APPEND ${name}.headers "dx/core/inline_object_array.h")
list(APPEND ${name}.sources "dx/core/inline_pointer_array.c")
list(APPEND ${name}.headers "dx/core/inline_pointer_array.h")
list(APPEND ${name}.sources "dx/core/inline_pointer_deque.c")
list(APPEND ${name}.headers "dx/core/inline_pointer_deque.h")
list(APPEND ${name}.sources "dx/core/inline_pointer_hashmap.c")
list(APPEND ${name}.headers "dx/core/inline_pointer_hashmap.h")
list(APPEND ${name}.sources "dx/core/object_array.c")
list(APPEND ${name}.headers "dx/core/object_array.h")
list(APPEND ${name}.sources "dx/core/file_system.c")
list(APPEND ${name}.headers "dx/core/file_system.h")
list(APPEND ${name}.sources "dx/core/visuals.c")
list(APPEND ${name}.headers "dx/core/visuals.h")
list(APPEND ${name}.sources "dx/core/safe_add_nx.c" "dx/core/safe_mul_ix.c" "dx/core/safe_mul_nx.c")
list(APPEND ${name}.headers "dx/core/safe_add_nx.h" "dx/core/safe_mul_ix.h" "dx/core/safe_mul_nx.h")
list(APPEND ${name}.sources "dx/core/convert.c")
list(APPEND ${name}.headers "dx/core/convert.h")

list(APPEND ${name}.sources "dx/core/canvas_msgs.c")
list(APPEND ${name}.headers "dx/core/canvas_msgs.h")

list(APPEND ${name}.sources "dx/core/hapticals.c")
list(APPEND ${name}.headers "dx/core/hapticals.h")
list(APPEND ${name}.inlays  "dx/core/keyboard_keys.i")
list(APPEND ${name}.inlays  "dx/core/mouse_buttons.i")
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

list(APPEND ${name}.sources "dx/core/msgs.c")
list(APPEND ${name}.headers "dx/core/msgs.h")

list(APPEND ${name}.sources "Core/FundamentalTypes.c")
list(APPEND ${name}.headers "Core/FundamentalTypes.h")

list(APPEND ${name}.sources "Core/CountLeadingZeroes.c")
list(APPEND ${name}.headers "Core/CountLeadingZeroes.h")
list(APPEND ${name}.sources "Core/Hash.c")
list(APPEND ${name}.headers "Core/Hash.h")
list(APPEND ${name}.sources "Core/Memory.c")
list(APPEND ${name}.headers "Core/Memory.h")
list(APPEND ${name}.sources "Core/NextPowerOfTwo.c")
list(APPEND ${name}.headers "Core/NextPowerOfTwo.h")

# Folder "dx/core/visuals".
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

# Folder "dx/core/text".
list(APPEND ${name}.sources "dx/core/text/anchor.c")
list(APPEND ${name}.headers "dx/core/text/anchor.h")
list(APPEND ${name}.sources "dx/core/text/bounds_type.c")
list(APPEND ${name}.headers "dx/core/text/bounds_type.h")

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
