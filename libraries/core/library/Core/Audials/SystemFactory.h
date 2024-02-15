#if !defined(CORE_AUDIALS_SYSTEMFACTORY_H_INCLUDED)
#define CORE_AUDIALS_SYSTEMFACTORY_H_INCLUDED

#include "Core/SystemFactory.h"

Core_declareObjectType("Core.Audials.SystemFactory",
                       Core_Audials_SystemFactory,
                       Core_SystemFactory);

static inline Core_Audials_SystemFactory* CORE_AUDIALS_SYSTEMFACTORY(void* p) {
  return (Core_Audials_SystemFactory*)p;
}

struct Core_Audials_SystemFactory {
  Core_SystemFactory _parent;
};

static inline Core_Audials_SystemFactory_Dispatch* CORE_AUDIALS_SYSTEMFACTORY_DISPATCH(void* p) {
  return (Core_Audials_SystemFactory_Dispatch*)p;
}

struct Core_Audials_SystemFactory_Dispatch {
  Core_SystemFactory_Dispatch _parent;
};

/// @brief Construct this audials system factory.
/// @constructor{Core_Audials_SystemFactory}
Core_Result Core_Audials_SystemFactory_construct(Core_Audials_SystemFactory* SELF);

#endif // CORE_AUDIALS_SYSTEMFACTORY_H_INCLUDED
