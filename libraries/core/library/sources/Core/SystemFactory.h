// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

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
  Core_Result(*createSystem)(Core_System**, Core_SystemFactory*, Core_MessageQueue*);
};

/// @brief Construct this system factory.
/// @param SELF A pointer to this system factory.
/// @constructor{Core_SystemFactory}
Core_Result Core_SystemFactory_construct(Core_SystemFactory* SELF);

/// @brief Create the system.
/// @param RETURN A pointer to a <code>Core_System*</code> variable.
/// @param SELF A pointer to this system factory.
/// @param messageQueue A pointer to the message queue.
/// The system acquires a reference to that message queue.
/// @method{Core_SystemFactory}
static inline Core_Result Core_SystemFactory_createSystem(Core_System** RETURN, Core_SystemFactory* SELF, Core_MessageQueue* messageQueue) {
  DX_OBJECT_VIRTUALCALL(Core_SystemFactory, createSystem, RETURN, SELF, messageQueue);
}

#endif // CORE_SYSTEMFACTORY_H_INCLUDED
