#if !defined(CORE_VISUALS_SYSTEMFACTORY_H_INCLUDED)
#define CORE_VISUALS_SYSTEMFACTORY_H_INCLUDED

#include "Core/SystemFactory.h"

Core_declareObjectType("Core.Visuals.SystemFactory",
                       Core_Visuals_SystemFactory,
                       Core_SystemFactory);

static inline Core_Visuals_SystemFactory* CORE_VISUALS_SYSTEMFACTORY(void* p) {
  return (Core_Visuals_SystemFactory*)p;
}

struct Core_Visuals_SystemFactory {
  Core_SystemFactory _parent;
};

static inline Core_Visuals_SystemFactory_Dispatch* CORE_VISUALS_SYSTEMFACTORY_DISPATCH(void* p) {
  return (Core_Visuals_SystemFactory_Dispatch*)p;
}

struct Core_Visuals_SystemFactory_Dispatch {
  Core_SystemFactory_Dispatch _parent;
};

/// @brief Construct this visuals system factory.
/// @constructor{Core_Visuals_SystemFactory}
Core_Result Core_Visuals_SystemFactory_construct(Core_Visuals_SystemFactory* SELF);

#endif // CORE_VISUALS_SYSTEMFACTORY_H_INCLUDED
