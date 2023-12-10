#if !defined(DX_CORE_OS_H_INCLUDED)
#define DX_CORE_OS_H_INCLUDED

#include "dx/core/string.h"

/// @ingroup os
/// @brief Get the page size.
/// @param RETURN A pointer to a Core_Size variable.
/// @procedure
/// @success <code>*RETURN</code> was assigned the page size.
Core_Result Core_getPageSize(Core_Size* RETURN);

/// @ingroup os
/// @brief Get the number of CPU cores.
/// @procedure
/// @success <code>*RETURN</code> was assigned the number of cores.
Core_Result Core_getNumberOfCores(Core_Size* RETURN);

/// @ingroup os
/// @brief Get the executable path.
/// @return A pointer to the executable path dx_string object. The null pointer on failure.
/// @success The caller acquired a reference to the string.
Core_Result Core_getExecutablePath(Core_String** RETURN);

#endif // DX_CORE_OS_H_INCLUDED
