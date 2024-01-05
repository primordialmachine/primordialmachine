#if !defined(DX_CONFIGURATION_H_INCLUDED)
#define DX_CONFIGURATION_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core
/// @brief The major version of this library.
#define Core_Version_Major (2)

/// @ingroup Core
/// @brief The minor version of this library.
#define Core_Version_Minor (0)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core
/// @brief Symbolic constant for the Microsoft Windows operating system.
#define Core_OperatingSystem_Windows (1)

/// @ingroup Core
/// @brief Symbolic constant for an Unix operating system.
#define Core_OperatingSystem_Unix (2)

/// @ingroup Core
/// @brief Symbolic constant for Linux operating system.
#define Core_OperatingSystem_Linux (3)

/// @ingroup Core
/// @brief Symbolic constant for a MacOS operating system.
#define Core_OperatingSystem_Macos (4)

/// @ingroup Core
/// @brief Symbolic constant for a iOS operating system.
#define Core_OperatingSystem_Ios (5)

/// @ingroup Core
/// @brief Symbolic constant for a iOS simulator operating system.
#define Core_OperatingSystem_IosSimulator (6)

/// @ingroup Core
/// @brief Symbolic constant for the Cygwin/Microsoft Windows operating system.
#define Core_Operatingsystem_Cygwin (7)

/// @ingroup Core
/// @brief Symbolic constant for the MSYS/Microsoft Windows operating system.
#define Core_OperatingSystem_Msys (8)

/// @ingroup Core
/// @brief Symbolic constant for the MinGW/Microsoft Windows operating system.
#define Core_OperatingSystem_Mingw (9)

/// @ingroup Core
/// @brief Symbolic constant indicating the operating system.
/// It is defined to a value of one of the Core_OperatingSystem_* symbolic constants.
#if !defined(Core_OperatingSystem)
  #if defined(_WIN32)
    #define Core_OperatingSystem Core_OperatingSystem_Windows
  #else
    #error("operating system not (yet) supported")
  #endif
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core
/// @brief Symbolic constant for the "MSVC" C compiler.
#define Core_Compiler_C_Msvc (1)

/// @ingroup Core
/// @brief Symbolic constant for the "GCC" C compiler.
#define Core_Compiler_C_Gcc (2)

/// @ingroup Core
/// @brief Symbolic constant for the "CLANG" C compiler.
#define Core_Compiler_C_Clang (3)

/// @ingroup Core
/// @brief Symbolic constant for the "MSVC" CPP compiler.
#define Core_Compiler_Cpp_Msvc (4)

/// @ingroup Core
/// @brief Symbolic constant for the "GCC" CPP compiler.
#define Core_Compiler_Cpp_Gcc (5)

/// @ingroup Core
/// @brief Symbolic constant for the "CLANG" CPP compiler.
#define Core_Compiler_Cpp_Clang (6)

/// @ingroup Core
/// @brief Symbolic constant indicating the C compiler.
/// It is defined to a value of one of the Core_Compiler_C_* symbolic constants.
#if !defined(Core_Compiler_C)
  #if defined(_MSC_VER)
    #define Core_Compiler_C Core_Compiler_C_Msvc
  #elif defined(__GNUC__)
    #define Core_Compiler_C Core_Compiler_C_Gcc_GNUC
  #elif defined(__clang__)
    #define Core_Compiler_C Core_Compiler_C_Clang
  #else
    #error("C compiler not (yet) supported")
  #endif
#endif

/// @ingroup Core
/// @brief Symbolic constant indicating the CPP compiler.
/// It is defined to a value of one of the Core_Compiler_CPP_* symbolic constants.
#if !defined(Core_Compiler_Cpp)
  #if defined(_MSC_VER)
    #define Core_Compiler_Cpp Core_Compiler_Cpp_Msvc
  #elif defined(__GNUC__)
    #define Core_Compiler_Cpp Core_Compiler_Cpp_Gcc
  #elif defined(__clang__)
    #define Core_Compiler_Cpp Core_Compiler_Cpp_Clang
  #else
    #error("C++ compiler not (yet) supported")
  #endif
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core
/// @brief Symbolic constant for the "Direct3D 12" visuals backend.
#define Core_VisualsBackend_Direct3d12 (1)

/// @ingroup Core
/// @brief Symbolic constant for the "OpenGL 4" visuals backend.
#define Core_VisualsBackend_OpenGl4 (2)

/// @ingroup Core
/// @brief Symbolic constant for the "Vulkan" visuals backend.
#define Core_VisualsBackend_Vulkan (3)

/// @ingroup Core
/// @brief Symbolic constant indicating the visuals backend.
/// It is defined to a value of one of the Core_VisualsBackend_* symbolic constants.
#define Core_VisualsBackend Core_VisualsBackend_OpenGl4

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core
/// @brief Symbolic constant for the "OpenAL" audials backend.
#define Core_AudialsBackend_OpenAl (1)

/// @ingroup Core
/// @brief Symbolic constant indicating the audials backend.
/// It is defined to a value of one of the Core_AudialsBackend_* symbolic constants.
#define Core_AudialsBackend Core_AudialsBackend_OpenAl

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined an evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for Core_Object.
/// A macro DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES is provided.
/// If this function is invoked on a pointer that does not point to a valid Core_Object (or derived type) object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define DX_OBJECT_WITH_MAGIC_BYTES (1)

/// If defined and evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for Core_Object_Dispatch.
/// A macro DX_DEBUG_CHECK_OBJECT_DISPATCH_MAGIC_BYTES is provided.
/// If this function is invoked on a pointer that does not point to a valid Core_Object_Dispatch (or derived type) object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define DX_OBJECT_DISPATCH_WITH_MAGIC_BYTES (1)

/// If defined and evaluating to @a 1,
/// then atomic reference counting is enabled.
/// Otherwise it is disabled, that is, non-atomic reference counting is performed.
#define DX_ATOMIC_REFERENCE_COUNTING_ENABLED (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1, then memory blocks allocated with Core_Memory_allocate are prefixed with their sizes.
#define Core_Memory_withTagging (1)

/// If defined and evaluating to @a 1 and if tagging is enabled (see Core_Memory_withTagging), then the allocator maintains statistics on allocations.
#define Core_Memory_withStatistics (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// then a function dx_math_tests() is provided
/// which performs tests on functionality provided by "dx/core/math.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_MATH_WITH_TESTS (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// If defined and evaluating to @a 1, then Core_safeMulN7 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulN8_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulN16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulN16_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulN32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulN32_withAsm (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// the a function dx_safe_mul_ix_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_mul_ix.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_MUL_IX_WITH_TESTS (1)

// If defined and evaluating to @a 1, then dx_mul_i8 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_I8_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_i16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_I16_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_i32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_I32_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_i64 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is the only implementation so keep this enabled.
#define DX_MUL_I64_ASM (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CONFIGURATION_H_INCLUDED
