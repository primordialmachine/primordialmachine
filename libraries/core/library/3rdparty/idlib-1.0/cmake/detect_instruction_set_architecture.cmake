# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

# Enumeration of known/instruction set architectures.

# Unknown instruction set architecture.
set(IDLIB_INSTRUCTION_SET_ARCHITECTURE_STRING_UNKNOWN "<unknown instruction set architecture>")
set(IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_UNKNOWN 0)

# x86.
set(IDLIB_INSTRUCTION_SET_ARCHITECTURE_STRING_X86 "x86")
set(IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_X86 1)

# x64.
set(IDLIB_INSTRUCTION_SET_ARCHITECTURE_STRING_X64 "x64")
set(IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_X64 2)

# Macro to define ${target}.instruction_set_architecture.
# @param target The target.
macro(idlib_detect_instruction_set_architecture target)
  # Initialize if not yet initialized.
  if (NOT DEFINED ${target_name}.COMPILER_C_ID)
    set(${target}.INSTRUCTION_SET_ARCHITECTURE_ID ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_UNKNOWN})
    set(${target}.INSTRUCTION_SET_ARCHITECTURE_STRING ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_STRING_UNKNOWN})
  endif()
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(${target}.INSTRUCTION_SET_ARCHITECTURE_ID ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_X64})
	  set(${target}.INSTRUCTION_SET_ARCHITECTURE_STRING ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_STRING_X64})
  elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(${target}.INSTRUCTION_SET_ARCHITECTURE_ID ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_ID_X86})
	  set(${target}.INSTRUCTION_SET_ARCHITECTURE_STRING ${IDLIB_INSTRUCTION_SET_ARCHITECTURE_STRING_X86})
  endif()
  message( STATUS " - instruction set architecture: ${${target}.INSTRUCTION_SET_ARCHITECTURE_STRING}")
endmacro()
