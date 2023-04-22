# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.core")
dx_begin_project(${name})
project(${name} LANGUAGES C)

# "README.md".
list(APPEND ${name}.readmes "README.md")

# configuration files.
list(APPEND ${name}.sources "dx/core/configuration.c")
list(APPEND ${name}.headers "dx/core/configuration.h")

# master files.
list(APPEND ${name}.sources "dx/core.c")
list(APPEND ${name}.headers "dx/core.h")

# "core" folder.
list(APPEND ${name}.sources "dx/core/annotations.c")
list(APPEND ${name}.headers "dx/core/annotations.h")
list(APPEND ${name}.sources "dx/core/builtin_types.c")
list(APPEND ${name}.headers "dx/core/builtin_types.h")

list(APPEND ${name}.sources "dx/core/_get_best_array_size.c")
list(APPEND ${name}.headers "dx/core/_get_best_array_size.h")

list(APPEND ${name}.sources "dx/core/signals.c")
list(APPEND ${name}.headers "dx/core/signals.h")

list(APPEND ${name}.sources "dx/core/error.c")
list(APPEND ${name}.headers "dx/core/error.h")

list(APPEND ${name}.sources "dx/core/core.c" "dx/core/os.c")
list(APPEND ${name}.headers "dx/core/core.h" "dx/core/os.h")

list(APPEND ${name}.sources "dx/core/_string_format.c" "dx/core/string.c")
list(APPEND ${name}.headers "dx/core/_string_format.h" "dx/core/string.h")
list(APPEND ${name}.sources "dx/core/string_buffer.c")
list(APPEND ${name}.headers "dx/core/string_buffer.h")
list(APPEND ${name}.sources "dx/core/_is_utf8_sequence.c")
list(APPEND ${name}.headers "dx/core/_is_utf8_sequence.h")

list(APPEND ${name}.sources "dx/core/scanner.c")
list(APPEND ${name}.headers "dx/core/scanner.h")

list(APPEND ${name}.sources "dx/core/math.c")
list(APPEND ${name}.headers "dx/core/math.h")
list(APPEND ${name}.sources "dx/core/memory.c")
list(APPEND ${name}.headers "dx/core/memory.h")
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
list(APPEND ${name}.sources "dx/core/count_leading_zeroes.c")
list(APPEND ${name}.headers "dx/core/count_leading_zeroes.h")
list(APPEND ${name}.sources "dx/core/next_power_of_two.c")
list(APPEND ${name}.headers "dx/core/next_power_of_two.h")
list(APPEND ${name}.sources "dx/core/convert.c")
list(APPEND ${name}.headers "dx/core/convert.h")

list(APPEND ${name}.sources "dx/core/canvas_msgs.c")
list(APPEND ${name}.headers "dx/core/canvas_msgs.h")

list(APPEND ${name}.sources "dx/core/hapticals.c")
list(APPEND ${name}.headers "dx/core/hapticals.h")
list(APPEND ${name}.inlays "dx/core/keyboard_keys.i")
list(APPEND ${name}.inlays "dx/core/mouse_buttons.i")
list(APPEND ${name}.sources "dx/core/keyboard_state.c")
list(APPEND ${name}.headers "dx/core/keyboard_state.h")
list(APPEND ${name}.sources "dx/core/mouse_state.c")
list(APPEND ${name}.headers "dx/core/mouse_state.h")

list(APPEND ${name}.sources "dx/core/object.c")
list(APPEND ${name}.headers "dx/core/object.h")

list(APPEND ${name}.sources "dx/core/msgs.c")
list(APPEND ${name}.headers "dx/core/msgs.h")

# "core/asm" folder.
enable_language(ASM_MASM) # Enable ASM provided by MASM.

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

#
add_files( ${name}.inlays "dx/core/asm/${infix}/_umul*.i")
add_files( ${name}.sources "dx/core/asm/${infix}/_umul*.c" )
add_files( ${name}.headers "dx/core/asm/${infix}/_umul*.h" )
add_files( ${name}.asms "dx/core/asm/${infix}/_umul*.asm" )
#
add_files( ${name}.inlays "dx/core/asm/${infix}/_imul*.i")
add_files( ${name}.sources "dx/core/asm/${infix}/_imul*.c" )
add_files( ${name}.headers "dx/core/asm/${infix}/_imul*.h" )
add_files( ${name}.asms "dx/core/asm/${infix}/_imul*.asm" )
#
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set_source_files_properties(
      ${${name}.asms}
      PROPERTIES
      COMPILE_FLAGS "/safeseh"
  )
endif()

add_library(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

dx_end_project(${name})
