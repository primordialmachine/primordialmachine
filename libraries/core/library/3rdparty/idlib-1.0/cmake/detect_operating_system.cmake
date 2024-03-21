# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

# PROBLEMS: 
# - "MINGW" and/or "MSYS" is a "toolchain". The "operating system" is still "windows".
# - We are not aware of a reliable procedure to detect what Apple "operating system" is used.
#   This configuration script detects "Mac OS" even if the "operating system" is "iOS", "iOS Simulator", "watchOS", ... from within CMake?

set(IDLIB_OPERATING_SYSTEM_ID_UNKNOWN 0)
set(IDLIB_OPERATING_SYSTEM_STRING_UNKNOWN "<unknown operating system>")

set(IDLIB_OPERATING_SYSTEM_ID_WINDOWS 1)
set(IDLIB_OPERATING_SYSTEM_STRING_WINDOWS "WINDOWS")

set(IDLIB_OPERATING_SYSTEM_ID_UNIX 2)
set(IDLIB_OPERATING_SYSTEM_STRING_UNIX "UNIX")

set(IDLIB_OPERATING_SYSTEM_ID_LINUX 3)
set(IDLIB_OPERATING_SYSTEM_STRING_LINUX "LINUX")

set(IDLIB_OPERATING_SYSTEM_ID_MACOS 4)
set(IDLIB_OPERATING_SYSTEM_STRING_MACOS "MACOS")

set(IDLIB_OPERATING_SYSTEM_ID_CYGWIN 5)
set(IDLIB_OPERATING_SYSTEM_STRING_CYGWIN "CYGWIN")

set(IDLIB_OPERATING_SYSTEM_ID_MSYS 6)
set(IDLIB_OPERATING_SYSTEM_STRING_MSYS "MSYS")

set(IDLIB_OPERATING_SYSTEM_ID_MINGW 7)
set(IDLIB_OPERATING_SYSTEM_STRING_MINGW "MINGW")

set(IDLIB_OPERATING_SYSTEM_ID_IOS 8)
set(IDLIB_OPERATING_SYSTEM_STRING_IOS "IOS")

set(IDLIB_OPERATING_SYSTEM_ID_IOS_SIMULATOR 9)
set(IDLIB_OPERATING_SYSTEM_STRING_IOS_SIMULATOR "IOS SIMULATOR")


# Detect the operating system for the specified target by defining
macro(idlib_detect_operating_system target)
  if (NOT DEFINED ${target}.OPERATING_SYSTEM_ID)  
    set(${target}.OPERATING_SYSTEM_ID ${IDLIB_OPERATING_SYSTEM_ID_UNKNOWN})
    set(${target}.OPERATING_SYSTEM_STRING ${IDLIB_OPERATING_SYSTEM_STRING_UNKNOWN})
    if (WIN32)
      set(${target}.OPERATING_SYSTEM_ID ${IDLIB_OPERATING_SYSTEM_ID_WINDOWS})
      set(${target}.OPERATING_SYSTEM_STRING ${IDLIB_OPERATING_SYSTEM_STRING_WINDOWS})
    elseif (CYGWIN)
      set(${target}.OPERATING_SYSTEM_ID ${IDLIB_OPERATING_SYSTEM_ID_CYGWIN})
      set(${target}.OPERATING_SYSTEM_STRING ${IDLIB_OPERATING_SYSTEM_STRING_CYGWIN})  
    elseif (MSYS)
      set(${target}.OPERATING_SYSTEM_ID ${IDLIB_OPERATING_SYSTEM_ID_MSYS})
      set(${target}.OPERATING_SYSTEM_STRING ${IDLIB_OPERATING_SYSTEM_STRING_MSYS})
    endif()

    if (${${target}.OPERATING_SYSTEM_ID} EQUAL ${IDLIB_OPERATING_SYSTEM_ID_UNKNOWN})
      if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        set(${target}.OPERATING_SYSTEM_ID ${IDLIB_OPERATING_SYSTEM_ID_LINUX})
        set(${target}.OPERATING_SYSTEM_STRING ${IDLIB_OPERATING_SYSTEM_STRING_LINUX})
      elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
        set(${target}.OPERATING_SYSTEM_ID ${IDLIB_OPERATING_SYSTEM_ID_MACOS})
        set(${target}.OPERATING_SYSTEM_STRING ${IDLIB_OPERATING_SYSTEM_STRING_MACOS})
      endif()
    endif()
  endif()
  message(STATUS " - operating system: ${${target}.OPERATING_SYSTEM_STRING}")
endmacro()
