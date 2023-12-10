#if !defined(DX_CORE_SIGNALS_LIBRARY_H_INCLUDED)
#define DX_CORE_SIGNALS_LIBRARY_H_INCLUDED

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

/// @internal
/// @brief Invoked when an instance is about to be destroyed.
/// @param instance The instance that is about to be destroyed.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result Core_Signals_disconnectAll(Core_Object* instance);

#endif // DX_CORE_SIGNALS_LIBRARY_H_INCLUDED
