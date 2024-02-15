#if !defined(DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED

#include "Core/Audials/SystemFactory.h"

Core_declareObjectType("Core.Audials.Al.SystemFactory",
                       Core_Audials_Al_SystemFactory,
                       Core_Audials_SystemFactory);

static inline Core_Audials_Al_SystemFactory* DX_AAL_AL_SYSTEM_FACTORY(void* p) {
  return (Core_Audials_Al_SystemFactory*)p;
}

struct Core_Audials_Al_SystemFactory {
  Core_Audials_SystemFactory _parent;
};

static inline Core_Audials_Al_SystemFactory_Dispatch* DX_AAL_AL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (Core_Audials_Al_SystemFactory_Dispatch*)p;
}

struct Core_Audials_Al_SystemFactory_Dispatch {
  Core_Audials_SystemFactory_Dispatch _parent;
};

/// @brief Construct this audials AL system factory.
/// @constructor{Core_Audials_Al_SystemFactory}
Core_Result Core_Audials_Al_SystemFactory_construct(Core_Audials_Al_SystemFactory* SELF);

Core_Result Core_Audials_Al_SystemFactory_create(Core_Audials_Al_SystemFactory** RETURN);

#endif // CORE_AUDIALS_AL_SYSTEMFACTORY_H_INCLUDED
