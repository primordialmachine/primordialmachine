#if !defined(CORE_REFERENCECOUNTER_H_INCLUDED)
#define CORE_REFERENCECOUNTER_H_INCLUDED

#include <inttypes.h>
#include "dx/core/configuration.h"

/// @brief This must be aligned to 64-bit boundaries on 64-bit systems and to 32-bit boundaries on 32-bit systems.
#if defined(_WIN64)
  typedef int64_t Core_ReferenceCounter;
#else
  typedef int32_t Core_ReferenceCounter;
#endif

/// @return The resulting incremented value.
Core_ReferenceCounter Core_ReferenceCounter_increment(Core_ReferenceCounter* referenceCounter);

/// @return The resulting decremented value.
Core_ReferenceCounter Core_ReferenceCounter_decrement(Core_ReferenceCounter* referenceCounter);

#endif // CORE_REFERENCECOUNTER_H_INCLUDED
