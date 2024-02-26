# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

# Enumeration of known/supported C compilers.
set(CORE_COMPILER_C_STRING_UNKNOWN "<unknown compiler>")
set(CORE_COMPILER_C_ID_UNKNOWN 0)

set(CORE_COMPILER_C_STRING_CLANG "CLANG")
set(CORE_COMPILER_C_ID_CLANG 1)

set(CORE_COMPILER_C_STRING_MSVC "MSVC")
set(CORE_COMPILER_C_ID_MSVC 2)

set(CORE_COMPILER_C_STRING_GCC "GCC")
set(CORE_COMPILER_C_ID_GCC 3)

# Enumeration of known/supported C++ compilers.
set(CORE_COMPILER_CPP_STRING_UNKNOWN "<unknown>")
set(CORE_COMPILER_CPP_ID_UNKNOWN 0)

set(CORE_COMPILER_CPP_STRING_CLANG "CLANG")
set(CORE_COMPILER_CPP_ID_CLANG 1)

set(CORE_COMPILER_CPP_STRING_MSVC "MSVC")
set(CORE_COMPILER_CPP_ID_MSVC 2)

set(CORE_COMPILER_CPP_STRING_GCC "GCC")
set(CORE_COMPILER_CPP_ID_GCC 3)

# Enumeration of known/supported MASM compilers
set(CORE_COMPILER_MASM_STRING_UNKNOWN "<unknown>")
set(CORE_COMPILER_MASM_ID_UNKNOWN 0)

set(CORE_COMPILER_MASM_STRING_MASM "MASM")
set(CORE_COMPILER_MASM_ID_MASM 1)

# Macro to define ${target_name}_(C|CPP|MASM)_(ID|STRING) based depdning on the specified language.
# @param target The target.
# @param language CORE_LANGUAGE_ID_(C|CPP|MASM)
macro(core_detect_compiler target language)

  
  if (${language} EQUAL ${CORE_LANGUAGE_ID_C})
    if (NOT DEFINED ${target_name}.COMPILER_C_ID)
      set(${target}.COMPILER_C_ID ${CORE_COMPILER_C_ID_UNKNOWN})
      set(${target}.COMPILER_C_STRING ${CORE_COMPILER_C_STRING_UNKNOWN})
    endif()
    if (CMAKE_C_COMPILER_ID)
      if (CMAKE_C_COMPILER_ID MATCHES ".*clang")
        set(${target}.COMPILER_C_STRING ${CORE_COMPILER_C_STRING_CLANG})
        set(${target}.COMPILER_C_ID ${CORE_COMPILER_C_ID_CLANG})
      endif()
      if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(${target}.COMPILER_C_STRING ${CORE_COMPILER_C_STRING_GCC})
        set(${target}.COMPILER_C_ID ${CORE_COMPILER_C_ID_GCC})
      endif()
      if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        set(${target}.COMPILER_C_STRING ${CORE_COMPILER_C_STRING_MSVC})
        set(${target}.COMPILER_C_ID ${CORE_COMPILER_C_ID_MSVC})
      endif()
    endif()
    message( STATUS " - C compiler: ${${target}.COMPILER_C_STRING}")
  elseif (${language} EQUAL ${CORE_LANGUAGE_ID_CPP})
    if (NOT DEFINED ${target_name}.COMPILER_CPP_ID)
      set(${target}.COMPILER_CPP_ID ${CORE_COMPILER_CPP_ID_UNKNOWN})
      set(${target}.COMPILER_CPP_STRING ${CORE_COMPILER_CPP_STRING_UNKNOWN})
    endif()  
    if (CMAKE_CXX_COMPILER_ID)
      if (CMAKE_CXX_COMPILER_ID MATCHES ".*clang")
        set(${target}.COMPILER_CPP_STRING ${CORE_COMPILER_CPP_STRING_CLANG})
        set(${target}.COMPILER_CPP_ID ${CORE_COMPILER_CPP_ID_CLANG})
      endif()
      if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(${target}.COMPILER_CPP_STRING ${CORE_COMPILER_CPP_STRING_GCC})
        set(${target}.COMPILER_CPP_ID ${CORE_COMPILER_CPP_ID_GCC})
      endif()
      if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(${target}.COMPILER_CPP_STRING ${CORE_COMPILER_CPP_STRING_MSVC})
        set(${target}.COMPILER_CPP_ID ${CORE_COMPILER_CPP_ID_MSVC})
      endif()
    endif()
    message( STATUS " - C++ compiler: ${${target}.COMPILER_CPP_STRING}")
  elseif (${language} EQUAL ${CORE_LANGUAGE_ID_MASM})
    if (NOT DEFINED ${target_name}.COMPILER_MASM_ID)
      set(${target}.COMPILER_MASM_ID ${CORE_COMPILER_MASM_ID_UNKNOWN})
      set(${target}.COMPILER_MASM_STRING ${CORE_COMPILER_MASM_STRING_UNKNOWN})
    endif()
    message( STATUS " - MASM compiler: ${${target}.COMPILER_MASM_STRING}")
  endif()
endmacro()

