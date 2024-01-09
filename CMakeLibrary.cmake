include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/compilers.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/languages.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/detect_void_pointer_size.cmake)

# Macro which ensures that certain compiler warnings are treated as compiler errors.
macro(dx_configure_warnings target)

if(MSVC)

  set(COMPILE_OPTIONS "")

  # C4090:  'opertion' : different 'modifier' qualifiers
  list(APPEND COMPILE_OPTIONS "/we4090")
  # C4133: 'function' incompatible types - from 'type1' to 'type2'
  list(APPEND COMPILE_OPTIONS "/we4133")
  # C4020_ 'function': too many actual parameters
  list(APPEND COMPILE_OPTIONS "/we4020")
  # C4013: 'function': undefined; assuming extern returning int
  list(APPEND COMPILE_OPTIONS "/we4013")
  # C4024: 'function': different types for formal and actual parameter 'number'
  list(APPEND COMPILE_OPTIONS "/we4024")
  # C4716: 'function' must return a value
  list(APPEND COMPILE_OPTIONS "/we4716")
  # C4013: 'function' undefined; assuming extern returning int
  list(APPEND COMPILE_OPTIONS "/we4013")

  # The quotes around "${COMPILE_OPTIONS}" concatenate the list elements to a single string separated by semicolons.
  set_source_files_properties(${${name}.sources} PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}")
  set_source_files_properties(${${name}.headers} PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}")

endif()

endmacro()

# asserts the following variable are defined
#   - ${target}.languages
# defines the following variables:
#   - ${name}.target_architecture
#     the target architecture. Values: 'x86' or 'x64'.
#   - ${name}.asms
#     the list of assembler files. Initially the empty list.
#   - ${name}.headers
#     the list of C header files. Initially the empty list.
#   - ${name}.inlays
#     the list of C inlays files. Initially the empty list.
#   - ${name}.sources
#     the list of C source files. Initially the empty list.
#
# Furthermore, it defines the public and private include directories of the project.
macro(dx_begin_project target)
  message(${target})
  
  core_detect_void_pointer_size(${target})

  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    #message( STATUS "${target}:\n  ${target}.target_architecture := x64" )
    set( infix "x64" )
    set(${target}.target_architecture "x64")
  elseif( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    #message( STATUS "${target}:\n  ${target}.target_architecture := x86" )
    set( infix "x86" )
    set(${target}.target_architecture "x86")
  else()
    message( FATAL_ERROR "${target}: unable to detect target architecture" )
  endif()

  if (NOT (DEFINED ${target}.languages))
    message ( FATAL_ERROR "${target}: ${target}.languages is not defined" )
  endif()

  project(${target} LANGUAGES ${${target}.languages})
  
  core_detect_compiler(${target} CORE_LANGUAGE_ID_C)
  core_detect_compiler(${target} CORE_LANGUAGE_ID_CPP)
  core_detect_compiler(${target} CORE_LANGUAGE_ID_MASM)

  # Define the variables for assemblers, header, inlay, readme, and source files.
  set(${target}.asms "")
  set(${target}.headers "")
  set(${target}.inlays "")
  set(${target}.readmes "")
  set(${target}.sources "")

endmacro()

# asserts the following variables are defined
# - ${target}.kind must be "dynamic-library", "static-library", or "executable".
# - ${target}.source_directory must be defined and must exist
macro(dx_end_project target)
  # Sanity check.
  if (NOT (DEFINED ${target}.kind))
    message ( FATAL_ERROR "${target}: ${target}.kind is not defined" )
  endif()
  if ("${${target}.kind}" STREQUAL "dynamic-library")
  elseif ("${${target}.kind}" STREQUAL "executable")
  elseif ("${${target}.kind}" STREQUAL "static-library")
  else()
    message ( FATAL_ERROR "${target}: ${target}.kind is `${${target}.kind}`. ${target}.kind must be defined to 'dynamic-library', 'executable', or 'static-library'" )
  endif()
  
  #Sanity check.
  if (NOT (DEFINED ${target}.source_directory))
    message ( FATAL_ERROR "${target}: ${target}.source_directory is not defined" )
  endif()
  
  if ("${${target}.kind}" STREQUAL "dynamic-library")
    add_library(${target} MODULE ${${target}.sources} ${${target}.headers} ${${target}.readmes} ${${target}.inlays} ${${target}.asms})
  elseif ("${${target}.kind}" STREQUAL "executable")
    add_executable(${target} ${${target}.sources} ${${target}.headers} ${${target}.readmes} ${${target}.inlays} ${${target}.asms})
  elseif ("${${target}.kind}" STREQUAL "static-library")
    add_library(${target} ${${target}.sources} ${${target}.headers} ${${target}.readmes} ${${target}.inlays} ${${target}.asms})
  else()
    message ( FATAL_ERROR "${target}: ${target}.kind is `${${target}.kind}`. ${target}.kind must be defined to 'dynamic-library', 'executable', or 'static-library'" )
  endif()

  source_group(TREE ${${target}.source_directory} FILES ${${target}.asms})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.inlays})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.headers})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.readmes})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.sources})

  # Configure warnings for the source files.
  dx_configure_warnings(${target})

  #message( STATUS "${target}:" )
  message(" - kind: ${${target}.kind}" )
  message(" - target_architecture: ${${target}.target_architecture}" )
  message(" - void pointer size: ${${target}.sizeof_void_pointer}" )
  
  if (DEFINED ${target}.private_include_directories)
    message(" - private include directories specified: yes" )
    target_include_directories(${target} PRIVATE ${${target}.private_include_directories})
  else()
    message(" - private include directories specified: no" ) 
  endif()

  if (DEFINED ${target}.public_include_directories)
    message(" - public include directories specified: yes" )
    target_include_directories(${target} PUBLIC ${${target}.public_include_directories})  
  else()
    message(" - public include directories specified: no" )
  endif()

endmacro()
