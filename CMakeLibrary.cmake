
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

# Macro which ensures that certain compiler warnings are treated as compiler errors.
macro(dx_build_documentation target)

if (BUILD_DOCUMENTATION)

  find_package(Doxygen)

  if (DOXYGEN_FOUND)
    # Copy the doxygen configuration file to the out of source build directory.
    # Also adjust the copied configuration file.
    #set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    #set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/doxygen/DoxyfileCommon ${CMAKE_CURRENT_BINARY_DIR}/DoxyfileCommon @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/doxygen/doxygen-awesome.css  ${CMAKE_CURRENT_BINARY_DIR}/doxygen-awesome-css/doxygen-awesome.css @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/doxygen/doxygen-awesome-sidebar-only.css ${CMAKE_CURRENT_BINARY_DIR}/doxygen-awesome-css/doxygen-awesome-sidebar-only.css @ONLY)

    # Additional dependencies.
    file(GLOB_RECURSE ${name}.doxygen_files "${CMAKE_CURRENT_SOURCE_DIR}/documentation/*.*" )
    list(APPEND ${name}.doxygen_files ${CMAKE_SOURCE_DIR}/doxygen/doxygen-awesome.css)
    list(APPEND ${name}.doxygen_files ${CMAKE_SOURCE_DIR}/doxygen/doxygen-awesome-sidebar-only.css)
    list(APPEND ${name}.doxygen_files ${CMAKE_SOURCE_DIR}/doxygen/DoxyfileCommon)
    list(APPEND ${name}.doxygen_files ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp
      DEPENDS ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms} ${${name}.doxygen_files}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND "doxygen" Doxyfile
      COMMAND cmake -E touch ${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp
      COMMENT "generating API documentation with Doxygen"
      VERBATIM)

    add_custom_target(${name}.documentation ALL
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp)
  endif (DOXYGEN_FOUND)

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

  if (DEFINED ${target}.doxygen_enabled)
    if ("${${target}.doxygen_enabled}" STREQUAL ON)
    elseif ("${${target}.doxygen_enabled}" STREQUAL OFF)
    else()
      message ( FATAL_ERROR "${target}: ${target}.doxygen_enabled is `${${target}.doxygen_enabled}`. ${target}.doxygen_enabled must be undefined or defined to 'ON' or 'OFF'" )
    endif()
  else()
    set(${target}.doxygen_enabled ON)
  endif()
  if (${${target}.doxygen_enabled})
    dx_build_documentation(${target})
  endif()

  source_group(TREE ${${target}.source_directory} FILES ${${target}.asms})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.inlays})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.headers})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.readmes})
  source_group(TREE ${${target}.source_directory} FILES ${${target}.sources})

  # Configure warnings for the source files.
  dx_configure_warnings(${target})

  message( STATUS "${target}:" )
  message( STATUS "  ${target}.kind                := ${${target}.kind}" )
  message( STATUS "  ${target}.target_architecture := ${${target}.target_architecture}" )
  
  if (DEFINED ${target}.private_include_directories)
    message( STATUS "  private include directories specified" )
    target_include_directories(${target} PRIVATE ${${target}.private_include_directories})
  endif()

  if (DEFINED ${target}.public_include_directories)
    message( STATUS "  public include directories specified" )
    target_include_directories(${target} PUBLIC ${${target}.public_include_directories})  
  endif()

endmacro()
