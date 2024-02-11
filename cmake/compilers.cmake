# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

# Enumeration of known/supported compilers.
set(CORE_C_COMPILER_STRING_UNKNOWN "<unknown compiler>")
set(CORE_C_COMPILER_ID_UNKNOWN 0)

set(CORE_C_COMPILER_STRING_CLANG "CLANG")
set(CORE_C_COMPILER_ID_CLANG 1)

set(CORE_C_COMPILER_STRING_MSVC "MSVC")
set(CORE_C_COMPILER_ID_MSVC 2)

set(CORE_C_COMPILER_STRING_GCC "GCC")
set(CORE_C_COMPILER_ID_GCC 3)

set(CORE_CPP_COMPILER_STRING_UNKNOWN "<unknown>")
set(CORE_CPP_COMPILER_ID_UNKNOWN 0)

set(CORE_CPP_COMPILER_STRING_CLANG "CLANG")
set(CORE_CPP_COMPILER_ID_CLANG 1)

set(CORE_CPP_COMPILER_STRING_MSVC "MSVC")
set(CORE_CPP_COMPILER_ID_MSVC 2)

set(CORE_CPP_COMPILER_STRING_GCC "GCC")
set(CORE_CPP_COMPILER_ID_GCC 3)

# Macro to define ${target_name}_(C|CPP|MASM)_(ID|STRING) based depdning on the specified language.
# @param target The target.
# @param language CORE_LANGUAGE_ID_(C|CPP|MASM)
macro(core_detect_compiler target language)
  
  set(${target}_C_COMPILER_ID ${CORE_C_COMPILER_ID_UNKNOWN})
  set(${target}_C_COMPILER_STRING ${CORE_C_COMPILER_STRING_UNKNOWN})
  
  set(${target}_CPP_COMPILER_ID ${CORE_CPP_COMPILER_ID_UNKNOWN})
  set(${target}_CPP_COMPILER_STRING ${CORE_CPP_COMPILER_STRING_UNKNOWN})
  
  set(${target}_MASM_COMPILER_ID ${CORE_CPP_COMPILER_ID_UNKNOWN})
  set(${target}_MASM_COMPILER_STRING ${CORE_CPP_COMPILER_STRING_UNKNOWN})
  
  if (${language} EQUAL ${CORE_LANGUAGE_ID_C})
    if (CMAKE_C_COMPILER_ID)
      if (CMAKE_C_COMPILER MATCHES ".*clang")
        set(${target}_C_COMPILER_STRING ${CORE_C_COMPILER_STRING_CLANG})
        set(${target}_C_COMPILER_ID ${CORE_C_COMPILER_ID_CLANG})
      endif()
      if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(${target}_C_COMPILER_STRING ${CORE_C_COMPILER_STRING_GCC})
        set(${target}_C_COMPILER_ID ${CORE_C_COMPILER_ID_GCC})
      endif()
      if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        set(${target}_C_COMPILER_STRING ${CORE_C_COMPILER_STRING_MSVC})
        set(${target}_C_COMPILER_ID ${CORE_C_COMPILER_ID_MSVC})
      endif()
    endif()
    message( STATUS " - C compiler: ${${target}_C_COMPILER_STRING}")
  elseif (${language} EQUAL ${CORE_LANGUAGE_ID_CPP})
    if (CMAKE_CPP_COMPILER_ID)
      if (CMAKE_CPP_COMPILER MATCHES ".*clang")
        set(${target}_CPP_COMPILER_STRING ${CORE_CPP_COMPILER_STRING_CLANG})
        set(${target}_CPP_COMPILER_ID ${CORE_CPP_COMPILER_ID_CLANG})
      endif()
      if (CMAKE_CPP_COMPILER_ID STREQUAL "GNU")
        set(${target}_CPP_COMPILER_STRING ${CORE_CPP_COMPILER_STRING_GCC})
        set(${target}_CPP_COMPILER_ID ${CORE_CPP_COMPILER_ID_GCC})
      endif()
      if (CMAKE_CPP_COMPILER_ID STREQUAL "MSVC")
        set(${target}_CPP_COMPILER_STRING ${CORE_CPP_COMPILER_STRING_MSVC})
        set(${target}_CPP_COMPILER_ID ${CORE_CPP_COMPILER_ID_MSVC})
      endif()
    endif()
    message( STATUS " - C++ compiler: ${${target}_CPP_COMPILER_STRING}")
  elseif (${language} EQUAL ${CORE_LANGUAGE_ID_MASM})
    message( STATUS " - MASM compiler: ${${target}_MASM_COMPILER_STRING}")
  endif()
endmacro()
