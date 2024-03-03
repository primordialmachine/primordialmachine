#if !defined(CORE_SIGNALS_LIBRARY_H_INCLUDED)
#define CORE_SIGNALS_LIBRARY_H_INCLUDED

#include "dx/core/signals.h"

/// @internal
/// @undefined
/// - Invoked from any other function other than "Core_Runtime_initialize".
/// - The "Signals" module is initialized.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result Core_Signals_initialize();

/// @internal
/// @undefined
/// - Invoked from any other function other than "Core_Runtime_uninitialize".
/// - The "Signals" module is uninitialized.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result Core_Signals_uninitialize();

/// @brief Remove all connections of a specific context.
/// @param instance The instance to remove all connections from.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
/// @remarks This functions fails only if @a instance is a null pointer.
Core_Result Core_Signals_disconnectAll(Core_Object* instance);

#endif // CORE_SIGNALS_LIBRARY_H_INCLUDED
