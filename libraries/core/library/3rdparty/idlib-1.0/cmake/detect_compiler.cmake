# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/language.cmake)

# Enumeration of known/supported C compilers.

# An unknown compiler.
set(IDLIB_COMPILER_C_STRING_UNKNOWN "<unknown compiler>")
set(IDLIB_COMPILER_C_ID_UNKNOWN 0)

# CLANG.
set(IDLIB_COMPILER_C_STRING_CLANG "CLANG")
set(IDLIB_COMPILER_C_ID_CLANG 1)

# MSVC.
set(IDLIB_COMPILER_C_STRING_MSVC "MSVC")
set(IDLIB_COMPILER_C_ID_MSVC 2)

# GCC.
set(IDLIB_COMPILER_C_STRING_GCC "GCC")
set(IDLIB_COMPILER_C_ID_GCC 3)

# Macro to define ${target_name}_(C|CPP|MASM)_(ID|STRING) based depending on the specified language.
# @param target The target.
# @param language IDLIB_LANGUAGE_ID_(C|CPP|MASM)
macro(idlib_detect_compiler target language)

  if (${language} EQUAL ${IDLIB_LANGUAGE_ID_C})
    # Initialize if not yet initialized.
    if (NOT DEFINED ${target_name}.COMPILER_C_ID)
      set(${target}.COMPILER_C_ID ${IDLIB_COMPILER_C_ID_UNKNOWN})
      set(${target}.COMPILER_C_STRING ${IDLIB_COMPILER_C_STRING_UNKNOWN})
    endif()
    # Perform detection.
    if (CMAKE_C_COMPILER_ID)
      if (CMAKE_C_COMPILER_ID MATCHES ".*clang")
        set(${target}.COMPILER_C_STRING ${IDLIB_COMPILER_C_STRING_CLANG})
        set(${target}.COMPILER_C_ID ${IDLIB_COMPILER_C_ID_CLANG})
      endif()
      if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(${target}.COMPILER_C_STRING ${IDLIB_COMPILER_C_STRING_GCC})
        set(${target}.COMPILER_C_ID ${IDLIB_COMPILER_C_ID_GCC})
      endif()
      if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        set(${target}.COMPILER_C_STRING ${IDLIB_COMPILER_C_STRING_MSVC})
        set(${target}.COMPILER_C_ID ${IDLIB_COMPILER_C_ID_MSVC})
      endif()
    endif()
    message( STATUS " - C compiler: ${${target}.COMPILER_C_STRING}")
  else()
    message( FATAL_ERROR " - compiler detection: unsupported language")
  endif()

endmacro()
