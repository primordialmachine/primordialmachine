#if !defined(DX_CORE_OBJECT_LIBRARY_H_INCLUDED)
#define DX_CORE_OBJECT_LIBRARY_H_INCLUDED

#include "dx/core/Object.h"

/// @brief Initialize the "Runtime" module.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @failure This function sets the by-thread error variable.
/// @undefined
/// - The "Runtime" is initialized.
/// - This function is called from a thread other than the main thread.
/// @todo Should be library scoped.
/// @remarks
/// This ensures
/// - enumeration types, fundamental types, and object types can be defined (e.g., using Core_(define|declare)(Enumeration|Fundamental|Object)Type).
/// - Core.Object or derived type objects (e.g., Core.WeakReference) can be instantiated.
/// - Signals can be added to types.
Core_Result Core_Runtime_initialize();

/// @brief Uninitialize the "Runtime" module.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @failure This function sets the by-thread error variable.
/// @undefined
/// - The "Runtime" module is not initialized.
/// - This function is called from a thread other than the main thread.
/// @todo Should be library scoped.
Core_Result Core_Runtime_uninitialize();

#endif // DX_CORE_OBJECT_LIBRAY_H_INCLUDED
