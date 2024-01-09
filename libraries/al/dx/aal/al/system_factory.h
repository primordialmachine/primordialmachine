#if !defined(DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/aal/system_factory.h"

Core_declareObjectType("dx.aal.al.system_factory",
                       dx_aal_al_system_factory,
                       dx_aal_system_factory);

static inline dx_aal_al_system_factory* DX_AAL_AL_SYSTEM_FACTORY(void* p) {
  return (dx_aal_al_system_factory*)p;
}

struct dx_aal_al_system_factory {
  dx_aal_system_factory _parent;
};

static inline dx_aal_al_system_factory_Dispatch* DX_AAL_AL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_aal_al_system_factory_Dispatch*)p;
}

struct dx_aal_al_system_factory_Dispatch {
  dx_aal_system_factory_Dispatch _parent;
};

/// @brief Construct this AAL AL system factory.
/// @param SELF A pointer to this AAL AL system factory.
/// @method-call
Core_Result dx_aal_al_system_factory_construct(dx_aal_al_system_factory* SELF);

Core_Result dx_aal_al_system_factory_create(dx_aal_al_system_factory** RETURN);

#endif // DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED
