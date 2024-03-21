# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

# Macro to define ${target}.sizeof_void_pointer.
# @param target The target.
macro(idlib_detect_void_pointer_size target)
  set(${target}.sizeof_void_pointer ${CMAKE_SIZEOF_VOID_P})
endmacro()
