#if !defined(DX_AAL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_AAL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.aal.system_factory",
                       dx_aal_system_factory,
                       dx_system_factory);

static inline dx_aal_system_factory* DX_AAL_SYSTEM_FACTORY(void* p) {
  return (dx_aal_system_factory*)p;
}

struct dx_aal_system_factory {
  dx_system_factory _parent;
};

static inline dx_aal_system_factory_dispatch* DX_AAL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_aal_system_factory_dispatch*)p;
}

struct dx_aal_system_factory_dispatch {
  dx_system_factory_dispatch _parent;
};

/// @brief Construct this AAL system factory_.
/// @param SELF A pointer to this AAL system factory object.
/// @method-call
Core_Result dx_aal_system_factory_construct(dx_aal_system_factory* SELF);

#endif // DX_AAL_SYSTEM_FACTORY_H_INCLUDED
