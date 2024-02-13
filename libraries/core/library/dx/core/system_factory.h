#if !defined(CORE_SYSTEMFACTORY_H_INCLUDED)
#define CORE_SYSTEMFACTORY_H_INCLUDED

#include "dx/core.h"
typedef struct Core_System Core_System;

/* http://localhost/core#core-systemfactory */
Core_declareObjectType("Core.SystemFactory",
                       Core_SystemFactory,
                       Core_Object);

static inline Core_SystemFactory* CORE_SYSTEMFACTORY(void* p) {
  return (Core_SystemFactory*)p;
}

struct Core_SystemFactory {
  Core_Object _parent;
};

static inline Core_SystemFactory_Dispatch* CORE_SYSTEMFACTORY_DISPATCH(void* p) {
  return (Core_SystemFactory_Dispatch*)p;
}

struct Core_SystemFactory_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*create_system)(Core_System**, Core_SystemFactory*, Core_MessageQueue*);
};

/// @brief Construct this system factory.
/// @param SELF A pointer to this system factory.
/// @constructor{Core_SystemFactory}
Core_Result Core_SystemFactory_construct(Core_SystemFactory* SELF);

/// @brief Create the system.
/// @param RETURN A pointer to a <code>Core_System*</code> variable.
/// @param SELF A pointer to this system factory.
/// @param msg_queue A pointer to the message queue.
/// @method{Core_SystemFactory}
static inline Core_Result Core_SystemFactory_create_system(Core_System** RETURN, Core_SystemFactory* SELF, Core_MessageQueue* msg_queue) {
  DX_OBJECT_VIRTUALCALL(Core_SystemFactory, create_system, RETURN, SELF, msg_queue);
}

#endif // CORE_SYSTEMFACTORY_H_INCLUDED
