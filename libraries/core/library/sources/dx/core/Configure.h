// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_CONFIGURE_H_INCLUDED)
#define CORE_CONFIGURE_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Core_OperatingSystem_Unknown (0)

/* http://localhost/core#core-operatingsystem-windows */
#define Core_OperatingSystem_Windows (1)

/* http://localhost/core#core-operatingsystem-unix */
#define Core_OperatingSystem_Unix (2)

/* http://localhost/core#core-operatingsystem-linux */
#define Core_OperatingSystem_Linux (3)

/* http://localhost/core#core-operatingsystem-macos */
#define Core_OperatingSystem_Macos (4)

/* http://localhost/core#core-operatingsystem-ios */
#define Core_OperatingSystem_Ios (5)

/* http://localhost/core#core-operatingsystem-iossimulator */
#define Core_OperatingSystem_IosSimulator (6)

/* http://localhost/core#core-operatingsystem-cygwin */
#define Core_Operatingsystem_Cygwin (7)

/* http://localhost/core#core-operatingsystem-msys */
#define Core_OperatingSystem_Msys (8)

/* http://localhost/core#core-operatingsystem-mingw */
#define Core_OperatingSystem_Mingw (9)

/* http://localhost/core#core-operatingsystem */
#define Core_OperatingSystem Core_OperatingSystem_Windows

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Core_Compiler_C_Unknown (0)

/* http://localhost/core#core-compiler-c-msvc */
#define Core_Compiler_C_Msvc (1)

/* http://localhost/core#core-compiler-c-gcc */
#define Core_Compiler_C_Gcc (2)

/* http://localhost/core#core-compiler-c-clang */
#define Core_Compiler_C_Clang (3)

/* http://localhost/core#core-compiler-c */
#define Core_Compiler_C Core_Compiler_C_Msvc

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Core_Compiler_Cpp_Unknown (0)

/* http://localhost/core#core-compiler-cpp-msvc */
#define Core_Compiler_Cpp_Msvc (1)

/* http://localhost/core#core-compiler-cpp-gcc */
#define Core_Compiler_Cpp_Gcc (2)

/* http://localhost/core#core-compiler-cpp-clang */
#define Core_Compiler_Cpp_Clang (3)

/* http://localhost/core#core-compiler-cpp */
#define Core_Compiler_Cpp Core_Compiler_Cpp_Unknown

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// If defined and evaluating to @a 1, then Core_safeMulN7 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulN8_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulN16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulN16_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulN32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulN32_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulI8 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulI8_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulI16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulI16_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulI32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define Core_safeMulI32_withAsm (1)

// If defined and evaluating to @a 1, then Core_safeMulI64 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is the only implementation so keep this enabled.
#define Core_safeMulI64_withAsm (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_CONFIGURE_H_INCLUDED

