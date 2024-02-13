#if !defined(CORE_SYSTEM_H_INCLUDED)
#define CORE_SYSTEM_H_INCLUDED

#include "dx/core.h"

/* http://localhost/core#core-system */
Core_declareObjectType("Core.System",
                       Core_System,
                       Core_Object);

static inline Core_System* CORE_SYSTEM(void* p) {
  return (Core_System*)p;
}

struct Core_System {
  Core_Object _parent;
};

static inline Core_System_Dispatch* CORE_SYSTEM_DISPATCH(void* p) {
  return (Core_System_Dispatch*)p;
}

struct Core_System_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*startup)(Core_System*);
  Core_Result (*shutdown)(Core_System*);
};

/// @constructor{Core_System}
Core_Result Core_System_construct(Core_System* SELF);

/// @brief Startup this system.
/// @method{Core_System}
/// @pre The system is shutted down.
static inline Core_Result Core_System_startup(Core_System* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_System, startup, SELF);
}

/// @brief Shutdown this system.
/// @method{Core_System}
/// @pre The system is started up.
static inline Core_Result Core_System_shutdown(Core_System* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_System, shutdown, SELF);
}

#endif // CORE_SYSTEM_H_INCLUDED
