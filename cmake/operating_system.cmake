# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

# PROBLEMS: 
# - "MINGW" and/or "MSYS" is a "toolchain". The "operating system" is still "windows".
# - We are not aware of a reliable procedure to detect what Apple "operating system" is used.
#   This configuration script detects "Mac OS" even if the "operating system" is "iOS", "iOS Simulator", "watchOS", ... from within CMake?

set(CORE_OPERATING_SYSTEM_ID_UNKNOWN 0)
set(CORE_OPERATING_SYSTEM_STRING_UNKNOWN "<unknown operating system>")

set(CORE_OPERATING_SYSTEM_ID_WINDOWS 1)
set(CORE_OPERATING_SYSTEM_STRING_WINDOWS "WINDOWS")

set(CORE_OPERATING_SYSTEM_ID_UNIX 2)
set(CORE_OPERATING_SYSTEM_STRING_UNIX "UNIX")

set(CORE_OPERATING_SYSTEM_ID_LINUX 3)
set(CORE_OPERATING_SYSTEM_STRING_LINUX "LINUX")

set(CORE_OPERATING_SYSTEM_ID_MACOS 4)
set(CORE_OPERATING_SYSTEM_STRING_MACOS "MACOS")

set(CORE_OPERATING_SYSTEM_ID_CYGWIN 5)
set(CORE_OPERATING_SYSTEM_STRING_CYGWIN "CYGWIN")

set(CORE_OPERATING_SYSTEM_ID_MSYS 6)
set(CORE_OPERATING_SYSTEM_STRING_MSYS "MSYS")

set(CORE_OPERATING_SYSTEM_ID_MINGW 7)
set(CORE_OPERATING_SYSTEM_STRING_MINGW "MINGW")

set(CORE_OPERATING_SYSTEM_ID_IOS 8)
set(CORE_OPERATING_SYSTEM_STRING_IOS "IOS")

set(CORE_OPERATING_SYSTEM_ID_IOSSIMULATOR 9)
set(CORE_OPERATING_SYSTEM_STRING_IOSSIMULATOR "IOS SIMULATOR")


# Detect the operating system for the specified target by defining
macro(core_detect_operating_system target)
  if (NOT DEFINED ${target}.OPERATING_SYSTEM_ID)  
      set(${target}.OPERATING_SYSTEM_ID ${CORE_OPERATING_SYSTEM_ID_UNKNOWN})
      set(${target}.OPERATING_SYSTEM_STRING ${CORE_OPERATING_SYSTEM_STRING_UNKNOWN})
      if (WIN32)
        set(${target}.OPERATING_SYSTEM_ID ${CORE_OPERATING_SYSTEM_ID_WINDOWS})
        set(${target}.OPERATING_SYSTEM_STRING ${CORE_OPERATING_SYSTEM_STRING_WINDOWS})
      elseif (CYGWIN)
        set(${target}.OPERATING_SYSTEM_ID ${CORE_OPERATING_SYSTEM_ID_CYGWIN})
        set(${target}.OPERATING_SYSTEM_STRING ${CORE_OPERATING_SYSTEM_STRING_CYGWIN})  
      elseif (MSYS)
        set(${target}.OPERATING_SYSTEM_ID ${CORE_OPERATING_SYSTEM_ID_MSYS})
        set(${target}.OPERATING_SYSTEM_STRING ${CORE_OPERATING_SYSTEM_STRING_MSYS})
      endif()

      if (${${target}.OPERATING_SYSTEM_ID} EQUAL ${CORE_OPERATING_SYSTEM_ID_UNKNOWN})
        if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
          set(${target}.OPERATING_SYSTEM_ID ${CORE_OPERATING_SYSTEM_ID_LINUX})
          set(${target}.OPERATING_SYSTEM_STRING ${CORE_OPERATING_SYSTEM_STRING_LINUX})
        elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
          set(${target}.OPERATING_SYSTEM_ID ${CORE_OPERATING_SYSTEM_ID_MACOS})
          set(${target}.OPERATING_SYSTEM_STRING ${CORE_OPERATING_SYSTEM_STRING_MACOS})
        endif()
      endif()
  endif()
  message(STATUS " - operating system: ${${target}.OPERATING_SYSTEM_STRING}")
endmacro()
