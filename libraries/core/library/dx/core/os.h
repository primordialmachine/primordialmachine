#if !defined(DX_CORE_OS_H_INCLUDED)
#define DX_CORE_OS_H_INCLUDED

#include "Core/String.h"

/* http://localhost/core#core-getpagesize */
Core_Result Core_getPageSize(Core_Size* RETURN);

/* http://localhost/core#core-getnumberofcores */
Core_Result Core_getNumberOfCores(Core_Size* RETURN);

/// @ingroup os
/// @brief Get the executable path.
/// @return A pointer to the executable path dx_string object. The null pointer on failure.
/// @success The caller acquired a reference to the string.
Core_Result Core_getExecutablePath(Core_String** RETURN);

#endif // DX_CORE_OS_H_INCLUDED
