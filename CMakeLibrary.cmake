
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

macro(dx_begin_project target)

# The target architecture determines which implementation to compile.
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  message( STATUS "detected x64 target architecture" )
  set( infix "x64" )
  set(${name}.architecture 'x86')
elseif( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  message( STATUS "detected x86 target architecture" )
  set( infix "x86" )
  set(${name}.architecture 'x86')
else()
  message( FATAL_ERROR "unable to detect architecture" )
endif()

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

# Define the variables for inlay, source, header, and readme files.
set(${name}.asms "")
set(${name}.headers "")
set(${name}.inlays "")
set(${name}.readmes "")
set(${name}.sources "")

endmacro()

macro(dx_end_project target)

dx_build_documentation(${name})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.asms})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.inlays})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})

endmacro()
