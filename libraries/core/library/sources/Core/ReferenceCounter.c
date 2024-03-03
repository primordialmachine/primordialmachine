#include "Core/ReferenceCounter.h"

#if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
  
  // InterlockedIncrement64, InterlockedDecrement64
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem

#else
  
  #error("operating system not (yet) supported")

#endif

Core_ReferenceCounter Core_ReferenceCounter_increment(Core_ReferenceCounter* referenceCounter) {
#if 1 == DX_ATOMIC_REFERENCE_COUNTING_ENABLED
  #if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
    #if defined(_WIN64)
      return InterlockedIncrement64(referenceCounter);
    #else
      return InterlockedIncrement(referenceCounter);
    #endif
  #elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem
    return __sync_add_and_fetch(&referenceCounter, 1);
  #else
    #error("operating system not (yet) supported")
  #endif
#else
  return ++(*referenceCounter);
#endif
}

Core_ReferenceCounter Core_ReferenceCounter_decrement(Core_ReferenceCounter* referenceCounter) {
#if 1 == DX_ATOMIC_REFERENCE_COUNTING_ENABLED
  #if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
    #if defined(_WIN64)
      return InterlockedDecrement64(referenceCounter);
    #else
      return InterlockedDecrement(referenceCounter);
    #endif
  #elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem
    return __sync_subtract_and_fetch(&referenceCounter, 1);
  #else
    #error("operating system not (yet) supported")
  #endif
#else
  return --(*referenceCounter);
#endif
}
