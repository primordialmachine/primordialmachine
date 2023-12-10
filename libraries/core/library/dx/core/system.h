#if !defined(DX_SYSTEM_H_INCLUDED)
#define DX_SYSTEM_H_INCLUDED

#include "dx/core.h"

/// @brief A system is an object with special semantics.
/// A system (dx_system derived type) is created via a system factory (dx_system_factory derived type).
/// 
/// A system is started up and shutted down.
/// - A system has two states, "started up" and "shutted down". The initial state is "shutted down".
/// - A successful call to dx_system_startup on a "shutted down" system puts the system in the "started up" state.
///   The system remains in "shutted down" state if the call fails.
///   A call to dx_system_startup on a "started up" system is undefined behavior
/// - a call to dx_system_shutdown on a "started up" system puts the system in the "shutted down" state.
///   A call to dx_system_shutdown on a "shutted down" system or a failure of this call is undefined behavior.
DX_DECLARE_OBJECT_TYPE("dx.system",
                       dx_system,
                       Core_Object);

static inline dx_system* DX_SYSTEM(void* p) {
  return (dx_system*)p;
}

struct dx_system {
  Core_Object _parent;
};

static inline dx_system_dispatch* DX_SYSTEM_DISPATCH(void* p) {
  return (dx_system_dispatch*)p;
}

struct dx_system_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*startup)(dx_system*);
  Core_Result (*shutdown)(dx_system*);
};

/// @constructor{dx_system}
Core_Result dx_system_construct(dx_system* SELF);

/// @brief Startup this system.
/// @method{dx_system}
/// @pre The system is shutted down.
static inline Core_Result dx_system_startup(dx_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_system, startup, SELF);
}

/// @brief Shutdown this system.
/// @method{dx_system}
/// @pre The system is started up.
static inline Core_Result dx_system_shutdown(dx_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_system, shutdown, SELF);
}

#endif // DX_SYSTEM_H_INCLUDED
