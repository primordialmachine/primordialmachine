// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_SYSTEMFACTORY_H_INCLUDED)
#define CORE_ASSETS_SYSTEMFACTORY_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("Core.Assets.SystemFactory",
                       Core_Assets_SystemFactory,
                       Core_SystemFactory);

static inline Core_Assets_SystemFactory* CORE_ASSETS_SYSTEMFACTORY(void* p) {
  return (Core_Assets_SystemFactory*)p;
}

struct Core_Assets_SystemFactory {
  Core_SystemFactory _parent;
};

static inline Core_Assets_SystemFactory_Dispatch* CORE_ASSETS_SYSTEMFACTORY_DISPATCH(void* p) {
  return (Core_Assets_SystemFactory_Dispatch*)p;
}

struct Core_Assets_SystemFactory_Dispatch {
  Core_SystemFactory_Dispatch _parent;
};

/// @constructor{Core_Assets_SystemFactory}
Core_Result Core_Assets_SystemFactory_construct(Core_Assets_SystemFactory* SELF);

/// @create-operator{Core_Assets_SystemFactory}
Core_Result Core_Assets_SystemFactory_create(Core_Assets_SystemFactory** RETURN);

#endif // CORE_ASSETS_SYSTEMFACTORY_H_INCLUDED
