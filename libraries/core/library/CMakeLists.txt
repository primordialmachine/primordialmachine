# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/idlib-1.0)

set(name "Core")
set(${name}.kind "static-library")
set(${name}.languages "C" "ASM_MASM") # Enable C and MASM.
set(${name}.public_include_directories "${CMAKE_CURRENT_SOURCE_DIR}/sources")
set(${name}.private_include_directories "${CMAKE_CURRENT_SOURCE_DIR}/sources")
set(${name}.source_directory "${CMAKE_CURRENT_SOURCE_DIR}/sources")

dx_begin_project(${name})

if (${${name}.COMPILER_C_ID} EQUAL ${CORE_COMPILER_C_ID_MSVC})
  set("Core_Compiler_C" "Core_Compiler_C_Msvc")
elseif (${${name}.COMPILER_C_ID} EQUAL ${CORE_COMPILER_C_ID_GCC})
  set("Core_Compiler_C" "Core_Compiler_C_Gcc")
elseif (${${name}.COMPILER_C_ID} EQUAL ${CORE_COMPILER_C_ID_UNKNOWN})
  set("Core_Compiler_C" "Core_Compiler_C_Unknown")
else()
  message(FATAL_ERROR "C compiler detection not executed")
endif()

if (${${name}.COMPILER_CPP_ID} EQUAL ${CORE_COMPILER_CPP_ID_MSVC})
  set("Core_Compiler_Cpp" "Core_Compiler_Cpp_Msvc")
elseif (${${name}.COMPILER_CPP_ID} EQUAL ${CORE_COMPILER_CPP_ID_GCC})
  set("Core_Compiler_Cpp" "Core_Compiler_Cpp_Gcc")
elseif (${${name}.COMPILER_CPP_ID} EQUAL ${CORE_COMPILER_CPP_ID_UNKNOWN})
  set("Core_Compiler_Cpp" "Core_Compiler_Cpp_Unknown")
else()
  message(FATAL_ERROR "C++ compiler detection not executed")
endif()

if (${${name}.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${CORE_INSTRUCTION_SET_ARCHITECTURE_ID_X64})
  set("Core_InstructionSetArchitecture" "Core_InstructionSetArchitecture_X64")
elseif (${${name}.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${CORE_INSTRUCTION_SET_ARCHITECTURE_ID_X86})
  set("Core_InstructionSetArchitecture" "Core_InstructionSetArchitecture_X86")
elseif (${${name}.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${CORE_INSTRUCTION_SET_ARCHITECTURE_ID_UNKNOWN})
  set("Core_InstructionSetArchitecture" "Core_InstructionSetArchitecture_Unknown")
else()
  message(FATAL_ERROR "C++ compiler detection not executed")
endif()

if (${${name}.OPERATING_SYSTEM_ID} EQUAL ${CORE_OPERATING_SYSTEM_ID_WINDOWS})
  set("Core_OperatingSystem" "Core_OperatingSystem_Windows")
  set("Core_safeMulN8_withAsm" "(1)")
  set("Core_safeMulN16_withAsm" "(1)")
  set("Core_safeMulN32_withAsm" "(1)")
  set("Core_safeMulI8_withAsm" "(1)")
  set("Core_safeMulI16_withAsm" "(1)")
  set("Core_safeMulI32_withAsm" "(1)")
  set("Core_safeMulI64_withAsm" "(1)")
elseif (${${name}.OPERATING_SYSTEM_ID} EQUAL ${CORE_OPERATING_SYSTEM_ID_LINUX})
  set("Core_OperatingSystem" "Core_OperatingSystem_Linux")
  set("Core_safeMulN8_withAsm" "(0)")
  set("Core_safeMulN16_withAsm" "(0)")
  set("Core_safeMulN32_withAsm" "(0)")
  set("Core_safeMulI8_withAsm" "(0)")
  set("Core_safeMulI16_withAsm" "(0)")
  set("Core_safeMulI32_withAsm" "(0)")
  set("Core_safeMulI64_withAsm" "(0)")
elseif (${${name}.OPERATING_SYSTEM_ID} EQUAL ${CORE_OPERATING_SYSTEM_ID_UNKNOWN})
  set("Core_OperatingSystem" "Core_OperatingSystem_Unknown")
else()
  message(FATAL_ERROR "operating system detection not executed")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sources/dx/core/Configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/sources/dx/core/Configure.h)
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/sources)

# configuration files.
list(APPEND ${name}.sources "sources/dx/core/configuration.c")
list(APPEND ${name}.headers "sources/dx/core/configuration.h")

# master files.
list(APPEND ${name}.sources "sources/dx/core.c")
list(APPEND ${name}.headers "sources/dx/core.h")

# "Core" folder.
list(APPEND ${name}.sources "sources/Core/Context.c")
list(APPEND ${name}.headers "sources/Core/Context.h")
list(APPEND ${name}.sources "sources/Core/System.c")
list(APPEND ${name}.headers "sources/Core/System.h")
list(APPEND ${name}.sources "sources/Core/SystemFactory.c")
list(APPEND ${name}.headers "sources/Core/SystemFactory.h")

list(APPEND ${name}.sources "sources/dx/core/_get_best_array_size.c")
list(APPEND ${name}.headers "sources/dx/core/_get_best_array_size.h")

list(APPEND ${name}.sources "sources/dx/core/signals.c")
list(APPEND ${name}.headers "sources/dx/core/signals.h")
list(APPEND ${name}.sources "sources/dx/core/signals.library.c")
list(APPEND ${name}.headers "sources/dx/core/signals.library.h")
list(APPEND ${name}.sources "sources/dx/core/math.c")
list(APPEND ${name}.headers "sources/dx/core/math.h")

list(APPEND ${name}.sources "sources/dx/core/core.c")
list(APPEND ${name}.headers "sources/dx/core/core.h")
list(APPEND ${name}.sources "sources/dx/core/os.c")
list(APPEND ${name}.headers "sources/dx/core/os.h")

list(APPEND ${name}.sources "sources/dx/core/text/anchor.c")
list(APPEND ${name}.headers "sources/dx/core/text/anchor.h")
list(APPEND ${name}.sources "sources/dx/core/text/bounds_type.c")
list(APPEND ${name}.headers "sources/dx/core/text/bounds_type.h")

list(APPEND ${name}.sources "sources/dx/core/_string_format.c")
list(APPEND ${name}.headers "sources/dx/core/_string_format.h")
list(APPEND ${name}.sources "sources/dx/core/string_buffer.c")
list(APPEND ${name}.headers "sources/dx/core/string_buffer.h")
list(APPEND ${name}.sources "sources/dx/core/_is_utf8_sequence.c")
list(APPEND ${name}.headers "sources/dx/core/_is_utf8_sequence.h")

list(APPEND ${name}.sources "sources/dx/core/inline_object_array.c")
list(APPEND ${name}.headers "sources/dx/core/inline_object_array.h")
list(APPEND ${name}.sources "sources/dx/core/inline_pointer_deque.c")
list(APPEND ${name}.headers "sources/dx/core/inline_pointer_deque.h")
list(APPEND ${name}.sources "sources/dx/core/inline_pointer_hashmap.c")
list(APPEND ${name}.headers "sources/dx/core/inline_pointer_hashmap.h")
list(APPEND ${name}.sources "sources/dx/core/object_array.c")
list(APPEND ${name}.headers "sources/dx/core/object_array.h")
list(APPEND ${name}.sources "sources/dx/core/visuals.c")
list(APPEND ${name}.headers "sources/dx/core/visuals.h")

list(APPEND ${name}.sources "sources/dx/core/keyboard_state.c")
list(APPEND ${name}.headers "sources/dx/core/keyboard_state.h")
list(APPEND ${name}.sources "sources/dx/core/mouse_state.c")
list(APPEND ${name}.headers "sources/dx/core/mouse_state.h")

list(APPEND ${name}.sources "sources/dx/core/Object.c")
list(APPEND ${name}.headers "sources/dx/core/Object.h")
list(APPEND ${name}.sources "sources/dx/core/Object.library.c")
list(APPEND ${name}.headers "sources/dx/core/Object.library.h")

# "Reference Counter".
list(APPEND ${name}.sources "sources/Core/ReferenceCounter.c")
list(APPEND ${name}.headers "sources/Core/ReferenceCounter.h")


# "Type System".
list(APPEND ${name}.sources "sources/Core/TypeSystem/TypeName.c")
list(APPEND ${name}.headers "sources/Core/TypeSystem/TypeName.h")
list(APPEND ${name}.sources "sources/dx/core/TypeSystem.c")
list(APPEND ${name}.headers "sources/dx/core/TypeSystem.h")
list(APPEND ${name}.sources "sources/dx/core/TypeSystem.library.c")
list(APPEND ${name}.headers "sources/dx/core/TypeSystem.library.h")

# "README.md".
list(APPEND ${name}.readmes "sources/README.md")

# Main include file.
list(APPEND ${name}.sources "sources/Core/Include.c")
list(APPEND ${name}.headers "sources/Core/Include.h")

# "Annotations".
list(APPEND ${name}.sources "sources/Core/Annotations.c")
list(APPEND ${name}.headers "sources/Core/Annotations.h")

# "Configurations".
list(APPEND ${name}.sources "sources/Core/Configuration.c")
list(APPEND ${name}.headers "sources/Core/Configuration.h")

# "Numerics".
list(APPEND ${name}.sources "sources/Core/Numerics.c")
list(APPEND ${name}.headers "sources/Core/Numerics.h")

# "Lexical Analyses".
list(APPEND ${name}.sources "sources/Core/Scanner.c")
list(APPEND ${name}.headers "sources/Core/Scanner.h")

# "Strings".
list(APPEND ${name}.sources "sources/Core/String.c")
list(APPEND ${name}.headers "sources/Core/String.h")
list(APPEND ${name}.sources "sources/Core/StringIterator.c")
list(APPEND ${name}.headers "sources/Core/StringIterator.h")

# "UTF8".
list(APPEND ${name}.sources "sources/Core/Utf8.c")
list(APPEND ${name}.headers "sources/Core/Utf8.h")
list(APPEND ${name}.sources "sources/Core/Utf8/DefaultIterator.c")
list(APPEND ${name}.headers "sources/Core/Utf8/DefaultIterator.h")
list(APPEND ${name}.sources "sources/Core/Utf8/DefaultReverseIterator.c")
list(APPEND ${name}.headers "sources/Core/Utf8/DefaultReverseIterator.h")
list(APPEND ${name}.sources "sources/Core/Utf8/Iterator.c")
list(APPEND ${name}.headers "sources/Core/Utf8/Iterator.h")
list(APPEND ${name}.inlays  "sources/Core/Utf8/classify.i")

# "low level operations"
list(APPEND ${name}.sources "sources/Core/CountLeadingZeroes.c")
list(APPEND ${name}.headers "sources/Core/CountLeadingZeroes.h")
list(APPEND ${name}.sources "sources/Core/Hash.c")
list(APPEND ${name}.headers "sources/Core/Hash.h")
list(APPEND ${name}.sources "sources/Core/Memory.c")
list(APPEND ${name}.headers "sources/Core/Memory.h")
list(APPEND ${name}.sources "sources/Core/InlineByteIterator.c")
list(APPEND ${name}.headers "sources/Core/InlineByteIterator.h")
list(APPEND ${name}.sources "sources/Core/NextPowerOfTwo.c")
list(APPEND ${name}.headers "sources/Core/NextPowerOfTwo.h")
list(APPEND ${name}.sources "sources/Core/Bits.c")
list(APPEND ${name}.headers "sources/Core/Bits.h")

# "Messages".
list(APPEND ${name}.sources "sources/Core/Message.c")
list(APPEND ${name}.headers "sources/Core/Message.h")
list(APPEND ${name}.sources "sources/Core/EmitMessage.c")
list(APPEND ${name}.headers "sources/Core/EmitMessage.h")
list(APPEND ${name}.sources "sources/Core/ApplicationMessage.c")
list(APPEND ${name}.headers "sources/Core/ApplicationMessage.h")
list(APPEND ${name}.sources "sources/Core/ApplicationMessageKind.c")
list(APPEND ${name}.headers "sources/Core/ApplicationMessageKind.h")

# "Message Queue".
list(APPEND ${name}.sources "sources/Core/MessageQueue.c")
list(APPEND ${name}.headers "sources/Core/MessageQueue.h")

# "Runtime".
list(APPEND ${name}.sources "sources/Core/Error.c")
list(APPEND ${name}.headers "sources/Core/Error.h")
list(APPEND ${name}.sources "sources/Core/FundamentalTypes.c")
list(APPEND ${name}.headers "sources/Core/FundamentalTypes.h")
list(APPEND ${name}.sources "sources/Core/Result.c")
list(APPEND ${name}.headers "sources/Core/Result.h")

# "Text to number conversion".
list(APPEND ${name}.sources "sources/Core/Convert.c")
list(APPEND ${name}.headers "sources/Core/Convert.h")

# "Time".
list(APPEND ${name}.sources "sources/Core/Time.c")
list(APPEND ${name}.headers "sources/Core/Time.h")

# "Inline Collections".
list(APPEND ${name}.sources "sources/Core/Collections/InlineArrayListN8.c")
list(APPEND ${name}.headers "sources/Core/Collections/InlineArrayListN8.h")
list(APPEND ${name}.sources "sources/Core/Collections/InlineArrayListP.c")
list(APPEND ${name}.headers "sources/Core/Collections/InlineArrayListP.h")

# "Safe arithmetic".
list(APPEND ${name}.sources "sources/Core/safeAddNx.c")
list(APPEND ${name}.headers "sources/Core/safeAddNx.h")
list(APPEND ${name}.sources "sources/Core/safeMulIx.c")
list(APPEND ${name}.headers "sources/Core/safeMulIx.h")
list(APPEND ${name}.sources "sources/Core/safeMulNx.c")
list(APPEND ${name}.headers "sources/Core/safeMulNx.h")

# "File system".
list(APPEND ${name}.sources "sources/Core/FileSystem/createDirectoryFile.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/createDirectoryFile.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/deleteRegularFile.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/deleteRegularFile.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/deleteDirectoryFile.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/deleteDirectoryFile.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/ExistingFilePolicy.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/ExistingFilePolicy.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/FileAccessMode.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/FileAccessMode.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/FileType.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/FileType.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/FileHandle.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/FileHandle.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/FileMapping.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/FileMapping.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/getFileContents.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/getFileContents.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/getFileType.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/getFileType.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/NonExistingFilePolicy.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/NonExistingFilePolicy.h")
list(APPEND ${name}.sources "sources/Core/FileSystem/setFileContents.c")
list(APPEND ${name}.headers "sources/Core/FileSystem/setFileContents.h")

# "Input".
list(APPEND ${name}.sources "sources/Core/Input/InputMessageKind.c")
list(APPEND ${name}.headers "sources/Core/Input/InputMessageKind.h")
list(APPEND ${name}.sources "sources/Core/Input/InputMessage.c")
list(APPEND ${name}.headers "sources/Core/Input/InputMessage.h")
list(APPEND ${name}.sources "sources/Core/Input/KeyboardKey.c")
list(APPEND ${name}.headers "sources/Core/Input/KeyboardKey.h")
list(APPEND ${name}.inlays  "sources/Core/Input/KeyboardKey.i")
list(APPEND ${name}.sources "sources/Core/Input/KeyboardKeyAction.c")
list(APPEND ${name}.sources "sources/Core/Input/KeyboardKeyAction.h")
list(APPEND ${name}.sources "sources/Core/Input/KeyboardKeyMessage.c")
list(APPEND ${name}.headers "sources/Core/Input/KeyboardKeyMessage.h")
list(APPEND ${name}.sources "sources/Core/Input/MouseButton.c")
list(APPEND ${name}.headers "sources/Core/Input/MouseButton.h")
list(APPEND ${name}.inlays  "sources/Core/Input/MouseButton.i")
list(APPEND ${name}.sources "sources/Core/Input/MouseButtonMessage.c")
list(APPEND ${name}.sources "sources/Core/Input/MouseButtonMessage.h")
list(APPEND ${name}.sources "sources/Core/Input/MouseButtonAction.c")
list(APPEND ${name}.sources "sources/Core/Input/MouseButtonAction.h")
list(APPEND ${name}.sources "sources/Core/Input/MousePointerAction.c")
list(APPEND ${name}.sources "sources/Core/Input/MousePointerAction.h")
list(APPEND ${name}.sources "sources/Core/Input/MousePointerMessage.c")
list(APPEND ${name}.sources "sources/Core/Input/MousePointerMessage.h")
list(APPEND ${name}.sources "sources/Core/Input/ModifierKeys.c")
list(APPEND ${name}.headers "sources/Core/Input/ModifierKeys.h")
list(APPEND ${name}.inlays  "sources/Core/Input/ModifierKeys.i")

# "Audials".
list(APPEND ${name}.sources "sources/Core/Audials/Context.c")
list(APPEND ${name}.headers "sources/Core/Audials/Context.h")
list(APPEND ${name}.sources "sources/Core/Audials/System.c")
list(APPEND ${name}.headers "sources/Core/Audials/System.h")
list(APPEND ${name}.sources "sources/Core/Audials/SystemFactory.c")
list(APPEND ${name}.headers "sources/Core/Audials/SystemFactory.h")

# "Visuals".
list(APPEND ${name}.sources "sources/Core/Visuals/CullMode.c")
list(APPEND ${name}.headers "sources/Core/Visuals/CullMode.h")
list(APPEND ${name}.sources "sources/Core/Visuals/DepthCompareMode.c")
list(APPEND ${name}.headers "sources/Core/Visuals/DepthCompareMode.h")
list(APPEND ${name}.sources "sources/Core/Visuals/FillMode.c")
list(APPEND ${name}.headers "sources/Core/Visuals/FillMode.h")
list(APPEND ${name}.sources "sources/Core/Visuals/PixelFormat.c")
list(APPEND ${name}.headers "sources/Core/Visuals/PixelFormat.h")
list(APPEND ${name}.sources "sources/Core/Visuals/TextureAddressMode.c")
list(APPEND ${name}.headers "sources/Core/Visuals/TextureAddressMode.h")
list(APPEND ${name}.sources "sources/Core/Visuals/VertexFormat.c")
list(APPEND ${name}.headers "sources/Core/Visuals/VertexFormat.h")
list(APPEND ${name}.sources "sources/Core/Visuals/WindingMode.c")
list(APPEND ${name}.headers "sources/Core/Visuals/WindingMode.h")
list(APPEND ${name}.sources "sources/Core/Visuals/CanvasMessage.c")
list(APPEND ${name}.headers "sources/Core/Visuals/CanvasMessage.h")
list(APPEND ${name}.sources "sources/Core/Visuals/CanvasMessageKind.c")
list(APPEND ${name}.headers "sources/Core/Visuals/CanvasMessageKind.h")
list(APPEND ${name}.sources "sources/Core/Visuals/CanvasSizeChangedMessage.c")
list(APPEND ${name}.headers "sources/Core/Visuals/CanvasSizeChangedMessage.h")
list(APPEND ${name}.sources "sources/Core/Visuals/CanvasDpiChangedMessage.c")
list(APPEND ${name}.headers "sources/Core/Visuals/CanvasDpiChangedMessage.h")
list(APPEND ${name}.sources "sources/Core/Visuals/CanvasStateChangedMessage.c")
list(APPEND ${name}.headers "sources/Core/Visuals/CanvasStateChangedMessage.h")
list(APPEND ${name}.sources "sources/Core/Visuals/SystemFactory.c")
list(APPEND ${name}.headers "sources/Core/Visuals/SystemFactory.h")
list(APPEND ${name}.sources "sources/Core/Visuals/SoftwarePixelBuffer.c")
list(APPEND ${name}.headers "sources/Core/Visuals/SoftwarePixelBuffer.h")
list(APPEND ${name}.sources "sources/Core/Visuals/TextureFilter.c")
list(APPEND ${name}.headers "sources/Core/Visuals/TextureFilter.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

list(APPEND ${name}.sources "sources/Core/Intrinsic/_umul128/_umul128.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_umul128/_umul128.h")
list(APPEND ${name}.sources "sources/Core/Intrinsic/_umul64/_umul64.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_umul64/_umul64.h")
list(APPEND ${name}.sources "sources/Core/Intrinsic/_umul32/_umul32.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_umul32/_umul32.h")
list(APPEND ${name}.sources "sources/Core/Intrinsic/_umul16/_umul16.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_umul16/_umul16.h")

list(APPEND ${name}.sources "sources/Core/Intrinsic/_imul128/_imul128.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_imul128/_imul128.h")
list(APPEND ${name}.sources "sources/Core/Intrinsic/_imul64/_imul64.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_imul64/_imul64.h")
list(APPEND ${name}.sources "sources/Core/Intrinsic/_imul32/_imul32.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_imul32/_imul32.h")
list(APPEND ${name}.sources "sources/Core/Intrinsic/_imul16/_imul16.c")
list(APPEND ${name}.headers "sources/Core/Intrinsic/_imul16/_imul16.h")

#
if (${${name}.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${CORE_INSTRUCTION_SET_ARCHITECTURE_ID_X64})

  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_umul16_${infix}.asm")
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_umul16_${infix}.h")
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_umul32_${infix}.asm")
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_umul32_${infix}.h")
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_umul64_${infix}.asm")
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_umul64_${infix}.h")

  add_files( ${name}.sources "sources/dx/core/asm/${infix}/_imul128_${infix}.c" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul128_${infix}.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul64_${infix}.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul64_${infix}.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul32_${infix}.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul32_${infix}.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul16_${infix}.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul16_${infix}.h" )

elseif (${${name}.INSTRUCTION_SET_ARCHITECTURE_ID} EQUAL ${CORE_INSTRUCTION_SET_ARCHITECTURE_ID_X86})

  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_umul64_${infix}_fastcall.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_umul64_${infix}_fastcall.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_umul32_${infix}_fastcall.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_umul32_${infix}_fastcall.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_umul16_${infix}_fastcall.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_umul16_${infix}_fastcall.h" )

  add_files( ${name}.sources "sources/dx/core/asm/${infix}/_imul128_${infix}.c" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul128_${infix}.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul64_${infix}_fastcall.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul64_${infix}_fastcall.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul32_${infix}_fastcall.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul32_${infix}_fastcall.h" )
  add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul16_${infix}_fastcall.asm" )
  add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul16_${infix}_fastcall.h" )

  #add_files( ${name}.sources "sources/dx/core/asm/${infix}/_imul*.c" )
  #add_files( ${name}.headers "sources/dx/core/asm/${infix}/_imul*.h" )
  #add_files( ${name}.asms    "sources/dx/core/asm/${infix}/_imul*.asm" )

endif()

#
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set_source_files_properties(
      ${${name}.asms}
      PROPERTIES
      COMPILE_FLAGS "/safeseh"
  )
endif()

dx_end_project(${name})

target_link_libraries(${name} idlib)
