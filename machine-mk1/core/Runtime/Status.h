/// @file Runtime/Status.h
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#if !defined(MACHINE_RUNTIME_STATUS_H_INCLUDED)
#define MACHINE_RUNTIME_STATUS_H_INCLUDED

#if !defined(MACHINE_RUNTIME_PRIVATE)
#error("Do not include this file directly, include `_Runtime.h` instead.")
#endif

/// @brief Type of a status variable values.
typedef int Machine_StatusValue;

/// @brief Symbolic constant for a status variable value.
/// That value indicates success. Guaranteed to be @a 0.
#define Machine_Status_Success (0)

/// @brief Symbolic constant for a status variable value.
/// That value indicates an allocation failed.
#define Machine_Status_AllocationFailed (1)

/// @brief Symbolic constant for a status variable value.
/// That value indicates an invalid argument.
#define Machine_Status_InvalidArgument (2)

/// @brief Symbolic constant for a status variable value.
/// That value indicates an invalid operation.
#define Machine_Status_InvalidOperation (3)

/// @brief Symbolic constant for a status variable value.
/// That value indicates an index is out of bounds.
#define Machine_Status_IndexOutOfBounds (4)

/// @brief Symbolic constant for a status variable value.
/// That value indicates a capacity is exhausted.
#define Machine_Status_CapacityExhausted (5)

/// @brief Symbolic constant for a status variable value.
/// That value indicates that something is not yet implemented.
#define Machine_Status_NotYetImplemented (6)

/// @brief Symbolic constant for a status variable value.
/// The value indicates that the environment failed.
#define Machine_Status_EnvironmentFailed (7)

/// @brief Symbolic constant for a status variable value.
/// That value indicates something is too short.
#define Machine_Status_TooShort (8)

/// @brief Symbolic constant for a status variable value.
/// That value indicates something is too long.
#define Machine_Status_TooLong (9)

/// @brief Symbolic constant for a status variable value.
/// That value indicates the occurrence of a lexical error.
#define Machine_Status_LexicalError (10)

/// @brief Symbolic constant for a status variable value.
/// That value indicates the occurrence of a syntactical error.
#define Machine_Status_SyntacticalError (11)

/// @brief Symbolic constant for a status variable value.
/// That value indicates the occurrence of a semantical error.
#define Machine_Status_SemanticalError (12)

/// @brief Symbolic constant for a status variable value.
/// The value indicates a program point was reached which should not be reached.
#define Machine_Status_UnreachableProgramPointReached (13)

/// @brief Symbolic constant for a status variable value.
/// That value indicates an pointer argument is null.
#define Machine_Status_ArgumentNull (14)

/// @brief Symbolic constant for a status variable value.
/// That value indicates an invalid number of arguments.
#define Machine_Status_InvalidNumberOfArguments (15)

/// @brief Symbolic constant for a status variable value.
/// That value indicates a conversion failed.
#define Machine_Status_ConversionFailed (16)


/// @brief Symbolic constant for a status variable value.
/// That value indicates a conversion failed.
#define Machine_Status_ConversionFailed (16)


/// @brief Get the value of the status variable.
/// @return The value.
/// @default #Machine_Status_Success
Machine_StatusValue Machine_getStatus();

/// @brief Set the value of the status variable.
/// @param status The value.
void Machine_setStatus(Machine_StatusValue status);

#endif // MACHINE_RUNTIME_STATUS_H_INCLUDED
