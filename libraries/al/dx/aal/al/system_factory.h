#if !defined(DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/aal/system_factory.h"

DX_DECLARE_OBJECT_TYPE("dx.aal.al.system_factory",
                       dx_aal_al_system_factory,
                       dx_aal_system_factory);

static inline dx_aal_al_system_factory* DX_AAL_AL_SYSTEM_FACTORY(void* p) {
  return (dx_aal_al_system_factory*)p;
}

struct dx_aal_al_system_factory {
  dx_aal_system_factory _parent;
};

static inline dx_aal_al_system_factory_dispatch* DX_AAL_AL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_aal_al_system_factory_dispatch*)p;
}

struct dx_aal_al_system_factory_dispatch {
  dx_aal_system_factory_dispatch _parent;
};

/// @brief Construct this AAL AL system factory.
/// @param SELF A pointer to this AAL AL system factory.
/// @method-call
dx_result dx_aal_al_system_factory_construct(dx_aal_al_system_factory* SELF);

dx_result dx_aal_al_system_factory_create(dx_aal_al_system_factory** RETURN);

#endif // DX_AAL_AL_SYSTEM_FACTORY_H_INCLUDED