#if !defined(DX_CONFIGURATION_H_INCLUDED)
#define DX_CONFIGURATION_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Environment
/// @brief Symbolic constant for the Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_WINDOWS (1)

/// @ingroup Core_Environment
/// @brief Symbolic constant for an Unix operating system.
#define DX_OPERATING_SYSTEM_UNIX (2)

/// @ingroup Core_Environment
/// @brief Symbolic constant for Linux operating system.
#define DX_OPERATING_SYSTEM_LINUX (3)

/// @ingroup Core_Environment
/// @brief Symbolic constant for a MacOS operating system.
#define DX_OPERATING_SYSTEM_MACOS (4)

/// @ingroup Core_Environment
/// @brief Symbolic constant for a iOS operating system.
#define DX_OPERATING_SYSTEM_IOS (5)

/// @ingroup Core_Environment
/// @brief Symbolic constant for a iOS simulator operating system.
#define DX_OPERATING_SYSTEM_IOSSIMULATOR (6)

/// @ingroup Core_Environment
/// @brief Symbolic constant for the Cygwin/Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_CYGWIN (7)

/// @ingroup Core_Environment
/// @brief Symbolic constant for the MSYS/Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_MSYS (8)

/// @ingroup Core_Environment
/// @brief Symbolic constant for the MinGW/Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_MINGW (9)

/// @ingroup Core_Environment
/// @brief Symbolic constant indicating the operating system.
/// It is defined to a value of one of the DX_OPERATING_SYSTEM_* symbolic constants.
#if !defined(DX_OPERATING_SYSTEM)
  #if defined(_WIN32)
    #define DX_OPERATING_SYSTEM DX_OPERATING_SYSTEM_WINDOWS
  #else
    #error("operating system not (yet) supported")
  #endif
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Environment
/// @brief Symbolic constant for the "MSVC" C compiler.
#define DX_COMPILER_C_MSVC (1)

/// @ingroup Core_Environment
/// @brief Symbolic constant for the "GCC" C compiler.
#define DX_COMPILER_C_GCC (2)

/// @ingroup core
/// @brief Symbolic constant for the "CLANG" C compiler.
#define DX_COMPILER_C_CLANG (3)

/// @ingroup core
/// @brief Symbolic constant for the "MSVC" CPP compiler.
#define DX_COMPILER_CPP_MSVC (4)

/// @ingroup core
/// @brief Symbolic constant for the "GCC" CPP compiler.
#define DX_COMPILER_CPP_GCC (5)

/// @ingroup core
/// @brief Symbolic constant for the "CLANG" CPP compiler.
#define DX_COMPILER_CPP_CLANG (6)

/// @ingroup core
/// @brief Symbolic constant indicating the C compiler.
/// It is defined to a value of one of the DX_COMPILER_C_* symbolic constants.
#if !defined(DX_COMPILER_C)
  #if defined(_MSC_VER)
    #define DX_COMPILER_C DX_COMPILER_C_MSVC
  #elif defined(__GNUC__)
    #define DX_COMPILER_C DX_COMPILER_C_GNUC
  #elif defined(__clang__)
    #define DX_COMPILER_C DX_COMPILER_C_CLANG
  #else
    #error("C compiler not (yet) supported")
  #endif
#endif

/// @ingroup core
/// @brief Symbolic constant indicating the CPP compiler.
/// It is defined to a value of one of the DX_COMPILER_CPP_* symbolic constants.
#if !defined(DX_COMPILER_CPP)
  #if defined(_MSC_VER)
    #define DX_COMPILER_CPP DX_COMPILER_CPP_MSVC
  #elif defined(__GNUC__)
    #define DX_COMPILER_CPP DX_COMPILER_CPP_GNUC
  #elif defined(__clang__)
    #define DX_COMPILER_CPP DX_COMPILER_CPP_CLANG
  #else
    #error("CPP compiler not (yet) supported")
  #endif
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_VISUALS_DIRECT3D12 (1)

#define DX_VISUALS_OPENGL4 (2)

#define DX_VISUALS_VULKAN3D (3)

/// @brief Symbolic constant indicating the visuals backend.
/// It is defined to a value of one of the DX_VISUALS_* symbolic constants.
#define DX_VISUALS DX_VISUALS_OPENGL4

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_AUDIALS_OPENAL (1)

/// @brief Symbolic constant indicating the audials backend.
/// It is defined to a value of one of the DX_AUDIALS_* symbolic constants.
#define DX_AUDIALS DX_AUDIALS_OPENAL

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined an evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for dx_object.
/// A macro DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES is provided.
/// If this function is invoked on a pointer that does not point to a valid dx_object (or derived type) object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define DX_OBJECT_WITH_MAGIC_BYTES (1)

/// If defined and evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for dx_object_dispatch.
/// A macro DX_DEBUG_CHECK_OBJECT_DISPATCH_MAGIC_BYTES is provided.
/// If this function is invoked on a pointer that does not point to a valid dx_object_dispatch (or derived type) object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define DX_OBJECT_DISPATCH_WITH_MAGIC_BYTES (1)

/// If defined and evaluating to @a 1,
/// then atomic reference counting is enabled.
/// Otherwise it is disabled, that is, non-atomic reference counting is performed.
#define DX_ATOMIC_REFERENCE_COUNTING_ENABLED (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// then a function dx_math_tests() is provided
/// which performs tests on functionality provided by "dx/core/math.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_MATH_WITH_TESTS (1)

/// If defined and evaluating to @a 1,
/// the a function dx_safe_add_nx_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_add_nx.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_ADD_NX_WITH_TESTS (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// the a function dx_safe_mul_nx_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_mul_nx.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_MUL_NX_WITH_TESTS (1)

// If defined and evaluating to @a 1, then dx_mul_n8 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_N8_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_n16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_N16_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_n32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_N32_ASM (1)

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

/// If defined and evaluating to @a 1,
/// the a function dx_next_power_of_to_tests() is provided
/// which performs tests on functionality provided by dx/core/next_power_of_two.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_NEXT_POWER_OF_TWO_WITH_TESTS (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CONFIGURATION_H_INCLUDED
